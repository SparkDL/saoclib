#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CL/opencl.h"
#include "AOCLUtils/aocl_utils.h"
#include "utils.h"

using namespace aocl_utils;

cl_int status = 0;

// OpenCL runtime configuration
cl_platform_id platform = NULL;
unsigned num_devices = 0;

cl_context context = NULL;
cl_command_queue queue; // num_devices elements
cl_program program = NULL;
cl_kernel kernel; // num_devices elements

scoped_array<cl_device_id> devices; // num_devices elements
cl_mem a_buf, b_buf, c_buf;

float *a = NULL;
float *b = NULL;
float *c = NULL;
float *d = NULL;

const int BLOCK_SIZE = 64;
const int M = 128;
const int K = 64;
const int N = 64;
const int a_size = M * N;
const int b_size = N * K;
const int c_size = M * N;

const float alpha = 1;
const float beta = 0;

float rand_float() {
    return float(rand()) / float(RAND_MAX) * 20.0f - 10.0f;
}

bool init_opencl() {
    printf("Initializing OpenCL\n");

    if (!setCwdToExeDir()) {
        return false;
    }

    // Get the OpenCL platform.
    platform = findPlatform("Intel(R) FPGA SDK for OpenCL(TM)");
    if (platform == NULL) {
        printf("ERROR: Unable to find Intel(R) FPGA OpenCL platform.\n");
        return false;
    }

    // Query the available OpenCL device.
    devices.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));
    printf("Platform: %s\n", getPlatformName(platform).c_str());
    printf("Using %d device(s)\n", num_devices);
    for (unsigned i = 0; i < num_devices; ++i) {
        printf("  %s\n", getDeviceName(devices[i]).c_str());
    }

    // Create the context.
    context = clCreateContext(NULL, num_devices, devices, &oclContextCallback, NULL, &status);
    checkError(status, "Failed to create context");

    // Create the program for all device. Use the first device as the
    // representative device (assuming all device are of the same type).
    std::string binary_file = getBoardBinaryFile("/home/pcz/develop/saoclib/target/sgemm.aocx",
                                                 devices[0]);
    printf("Using AOCX: %s\n", binary_file.c_str());
    program = createProgramFromBinary(context, binary_file.c_str(), devices, num_devices);

    // Build the program that was just created.
    status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
    checkError(status, "Failed to build program");


    // Command queue.
    queue = clCreateCommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE, &status);
    checkError(status, "Failed to create command queue");

    // Kernel.
    const char *kernel_name = "sgemm";
    kernel = clCreateKernel(program, kernel_name, &status);
    checkError(status, "Failed to create kernel");


    a_buf = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_CHANNEL_1_INTELFPGA,
                           M * N * sizeof(float), NULL, &status);
    checkError(status, "Failed to create buffer for input A");

    // For matrix B, each device needs the whole matrix. We specifically
    // assign this buffer to the second bank of global memory.
    b_buf = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_CHANNEL_2_INTELFPGA,
                           N * K * sizeof(float), NULL, &status);
    checkError(status, "Failed to create buffer for input B");

    // Output buffer. This is matrix C, for the rows that are computed by this
    // device. We assign this buffer to the first bank of global memory,
    // although it is not material to performance to do so because
    // the reads from the input matrices are far more frequent than the
    // write to the output matrix.
    c_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_CHANNEL_1_INTELFPGA,
                           M * N * sizeof(float), NULL, &status);
    checkError(status, "Failed to create buffer for output");

    return true;
}

bool init_problem() {
    a = (float *) alignedMalloc(a_size * sizeof(float));

    for (int i = 0; i < a_size; i++) {
        a[i] = rand_float();
    }
    b = (float *) alignedMalloc(b_size * sizeof(float));
    for (int i = 0; i < b_size; i++) {
        b[i] = rand_float();
    }
    c = (float *) alignedMalloc(c_size * sizeof(float));
    d = (float *) alignedMalloc(c_size * sizeof(float));

    for (int i = 0; i < c_size; i++) {
        c[i] = 0;
        d[i] = 0;
    }

}

void run() {

    // Transfer inputs to each device. Each of the host buffers supplied to
    // clEnqueueWriteBuffer here is already aligned to ensure that DMA is used
    // for the host-to-device transfer.

    status = clEnqueueWriteBuffer(queue, a_buf, CL_FALSE,
                                  0, a_size * sizeof(float), a, 0, NULL, NULL);
    checkError(status, "Failed to transfer input A");

    status = clEnqueueWriteBuffer(queue, b_buf, CL_FALSE,
                                  0, b_size * sizeof(float), b, 0, NULL, NULL);
    checkError(status, "Failed to transfer input B");

    status = clEnqueueWriteBuffer(queue, c_buf, CL_FALSE,
                                  0, c_size * sizeof(float), c, 0, NULL, NULL);
    checkError(status, "Failed to transfer input C");


    clFinish(queue);
    // Launch kernels.
    // This is the portion of time that we'll be measuring for throughput
    // benchmarking.
    cl_event kernel_event;

    const double start_time = getCurrentTimestamp();

    // Set kernel arguments.
    unsigned argi = 0;

    status = clSetKernelArg(kernel, argi++, sizeof(K), &K);
    checkError(status, "Failed to set argument %d", argi - 1);

    status = clSetKernelArg(kernel, argi++, sizeof(alpha), &alpha);
    checkError(status, "Failed to set argument %d", argi - 1);

    status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &a_buf);
    checkError(status, "Failed to set argument %d", argi - 1);

    status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &b_buf);
    checkError(status, "Failed to set argument %d", argi - 1);

    status = clSetKernelArg(kernel, argi++, sizeof(beta), &beta);
    checkError(status, "Failed to set argument %d", argi - 1);

    status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &c_buf);
    checkError(status, "Failed to set argument %d", argi - 1);

    // Enqueue kernel.
    // Use a global work size corresponding to the size of the output matrix.
    // Each work-item computes the result for one value of the output matrix,
    // so the global work size has the same dimensions as the output matrix.
    //
    // The local work size is one block, so BLOCK_SIZE x BLOCK_SIZE.
    //
    // Events are used to ensure that the kernel is not launched until
    // the writes to the input buffers have completed.
    const size_t global_work_size[2] = {M, N};
    const size_t local_work_size[2] = {BLOCK_SIZE, BLOCK_SIZE};

    status = clEnqueueNDRangeKernel(queue, kernel, 2, NULL,
                                    global_work_size, local_work_size, 0, NULL, &kernel_event);
    checkError(status, "Failed to launch kernel");

// Wait for all kernels to finish.
    clWaitForEvents(1, &kernel_event);

    const double end_time = getCurrentTimestamp();
    const double total_time = end_time - start_time;

// Wall-clock time taken.
    printf("\nTime: %0.3f ms\n", total_time * 1e3);

// Get kernel times using the OpenCL event profiling API.
    cl_ulong time_ns = getStartEndTime(kernel_event);
    printf("Kernel time (device %d): %0.3f ms\n", 0, double(time_ns) * 1e-6);

    // Compute the throughput (GFLOPS).
    // There are C_width * C_height output values, with each value
    // computed using A_width multiplies and adds.
    printf("\nTotal time: %0.2f ms\n\n", total_time);

    clReleaseEvent(kernel_event);

    status = clEnqueueReadBuffer(queue, c_buf, CL_TRUE,
                                 0, c_size * sizeof(float), c, 0, NULL, NULL);
    checkError(status, "Failed to read output matrix");

}

void clean() {
    if (context) {
        clReleaseContext(context);
    }
    if (queue) {
        clReleaseCommandQueue(queue);
    }
    if (kernel) {
        clReleaseKernel(kernel);
    }
}

#define pos(lda, row, col) (lda*col+row)

void mm(float *a, float *b, float *c, int m, int n, int k) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0;
            for (int e = 0; e < k; e++) {
                sum += a[pos(m, i, e)] * b[pos(k, e, j)];
            }
            c[pos(m, i, j)] = sum;
        }
    }
}

void sgemm() {
    run();
    clean();
}

// Entry point.
int main(int argc, char **argv) {
    init_opencl();
    init_problem();
    mm(a, b, d, M, N, K);
    sgemm();
    acl::printMatrix("a", a, M, K);
    acl::printMatrix("b", b, K, N);
    acl::printMatrix("c", c, M, N);
    acl::printMatrix("d", d, M, N);
}




