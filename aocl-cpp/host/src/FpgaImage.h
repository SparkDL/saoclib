//
// Created by pcz on 18-4-18.
//

#ifndef VECTOR_ADD_KERNEL_H
#define VECTOR_ADD_KERNEL_H

#include <cassert>
#include <utility>
#include "AOCLUtils/aocl_utils.h"
#include "CL/opencl.h"

using namespace aocl_utils;

struct Data {
    const void *value;
    size_t size;
};

typedef float T;

class FpgaImage {
public:
    FpgaImage(const std::string &image_name,
              const std::vector<size_t> &input_size_list)
            : image_name(image_name),
              input_buf_sizes(input_size_list) {

    }

    ~FpgaImage() {
        // release kernel
    }

    void cleanup() {
        if (kernel) {
            clReleaseKernel(kernel);
        }
        // release queue
        if (queue) {
            clReleaseCommandQueue(queue);
        }
        // release input buffer
        for (auto &input_buf:input_bufs) {
            if (input_buf) {
                clReleaseMemObject(input_buf);
            }
        }
        // release program
        if (program) {
            clReleaseProgram(program);
        }
        // release context
        if (context) {
            clReleaseContext(context);
        }
    }

    bool init_opencl() {
        cl_int status;

        printf("Initializing OpenCL\n");

        // Get the OpenCL platform.
        platform = findPlatform("Intel");
        if (platform == NULL) {
            printf("ERROR: Unable to find Intel FPGA OpenCL platform.\n");
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
        std::string binary_file = getBoardBinaryFile(image_name.c_str(), devices[0]);
        printf("Using AOCX: %s\n", binary_file.c_str());
        program = createProgramFromBinary(context, binary_file.c_str(), devices, num_devices);

        // Build the program that was just created.
        status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
        checkError(status, "Failed to build program");

        // create command queue for device
        this->queue = clCreateCommandQueue(context, devices[0],
                                           CL_QUEUE_PROFILING_ENABLE, &status);
        checkError(status, "Failed to create command queue");

        // create input buffer for every input
        for (size_t size : input_buf_sizes) {
            this->input_bufs.push_back(clCreateBuffer(context, CL_MEM_READ_WRITE,
                                                      size, NULL, &status));
            checkError(status, "Failed to create buffer for input");
        }

        return true;

    }

    size_t get_param_number() {
        return input_buf_sizes.size();
    }

    void call(const std::string &kernel_name,
              const size_t *global_work_size,
              const size_t *local_work_size,
              cl_uint work_dim,
              const std::vector<const Data *> &primitive_args,
              const std::vector<scoped_aligned_ptr<T> *> &mem_args) {
        // assure given parameters number is right
        auto primitive_args_count = primitive_args.size();
        auto mem_args_count = mem_args.size();
        assert(mem_args_count == this->input_buf_sizes.size());

        cl_int status;
        cl_event kernel_event;

        // create kernel
        kernel = clCreateKernel(this->program, kernel_name.c_str(), &status);
        checkError(status, "Failed to create kernel");

        const double start_time = getCurrentTimestamp();

        // Transfer inputs to each device. Each of the host buffers supplied to
        // clEnqueueWriteBuffer here is already aligned to ensure that DMA is used
        // for the host-to-device transfer.
        for (unsigned i = 0; i < mem_args_count; i++) {
            status = clEnqueueWriteBuffer(queue, input_bufs[i], CL_FALSE,
                                          0, input_buf_sizes[i],
                                          mem_args[i], 0, NULL, NULL);
            checkError(status, "Failed to transfer input %d", i);
        }
        // wait for queue to finish
        clFinish(queue);

        // set kernel input arguments.
        unsigned i = 0;
        for (i = 0; i < primitive_args_count; i++) {
            auto data = primitive_args[i];
            status = clSetKernelArg(kernel, i, data->size, data->value);
            checkError(status, "Failed to set primitive argument %d", i);
        }
        for (i = 0; i < mem_args_count; i++) {
            status = clSetKernelArg(kernel, i + primitive_args_count, sizeof(cl_mem), &input_bufs[i]);
            checkError(status, "Failed to set memory object argument %d", i);
        }


        // Enqueue kernel.
        status = clEnqueueNDRangeKernel(queue, kernel, work_dim, NULL,
                                        global_work_size, local_work_size, 0,
                                        NULL, &kernel_event);

        checkError(status, "Failed to launch kernel");

        // Read the result. This the final operation.
        status = clEnqueueReadBuffer(queue, input_bufs[mem_args_count - 1], CL_FALSE,
                                     0, input_buf_sizes[mem_args_count - 1], mem_args[mem_args_count - 1],
                                     0, NULL, &kernel_event);
        const double end_time = getCurrentTimestamp();

        // Wall-clock time taken.
        printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

        // Get kernel times using the OpenCL event profiling API.
        cl_ulong time_ns = getStartEndTime(kernel_event);
        printf("Kernel time (device %d): %0.3f ms\n", i, double(time_ns) * 1e-6);

        // Release all events.
        clReleaseEvent(kernel_event);
    }

private:
    const std::string image_name;
    const std::vector<size_t> input_buf_sizes; // the input buffer size for the image entry

    // OpenCL runtime configuration
    cl_platform_id platform = NULL;
    unsigned num_devices = 0;
    scoped_array<cl_device_id> devices; // num_devices elements
    cl_context context = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_device_id device_id = NULL;
    cl_command_queue queue;
    std::vector<cl_mem> input_bufs;
};


#endif //VECTOR_ADD_KERNEL_H
