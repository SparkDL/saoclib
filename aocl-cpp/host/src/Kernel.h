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

typedef float T;

class Kernel {
public:
    Kernel(const std::string &name,
           const std::vector<size_t> &input_size_list,
           const cl_platform_id platform,
           const cl_context context,
           const cl_program program,
           const cl_device_id device_id)
            : name(name),
              input_size_list(input_size_list),
              platform(platform),
              context(context),
              program(program),
              device_id(device_id) {
        cl_int status;
        // create command queue for device
        this->queue = clCreateCommandQueue(context, device_id,
                                           CL_QUEUE_PROFILING_ENABLE, &status);
        checkError(status, "Failed to create command queue");

        // create input buffer for every input
        for (size_t size : input_size_list) {
            this->input_buf_list.push_back(clCreateBuffer(context, CL_MEM_READ_ONLY,
                                                          size, NULL, &status));
            checkError(status, "Failed to create buffer for input");
        }

        // create kernel
        this->kernel = clCreateKernel(this->program, name.c_str(), &status);
        checkError(status, "Failed to create kernel");
    }

    ~Kernel() {
        // release kernel
        if (kernel) {
            clReleaseKernel(kernel);
        }
        // release queue
        if (queue) {
            clReleaseCommandQueue(queue);
        }
        // release input buffer
        for (auto &input_buf:input_buf_list) {
            if (input_buf) {
                clReleaseMemObject(input_buf);
            }
        }
    }

    size_t get_param_number() {
        return input_size_list.size();
    }

    void call(const scoped_aligned_ptr<T> *input_list, size_t size) {
        // assure given parameters number is right
        assert(size == this->input_size_list.size());

        cl_int status;
        cl_event kernel_event;
        cl_event finish_event;

        const double start_time = getCurrentTimestamp();

        // Transfer inputs to each device. Each of the host buffers supplied to
        // clEnqueueWriteBuffer here is already aligned to ensure that DMA is used
        // for the host-to-device transfer.
        size_t input_number = this->get_param_number();
        scoped_array<cl_event> write_event(input_number);
        for (unsigned i = 0; i < input_number; i++) {
            status = clEnqueueWriteBuffer(queue, input_buf_list[i], CL_FALSE,
                                          0, input_size_list[i],
                                          input_list[i], 0, NULL, &write_event[i]);
            checkError(status, "Failed to transfer input %d", i);
        }

        // set kernel input arguments.
        unsigned i = 0;
        for (i = 0; i < input_number; i++) {
            status = clSetKernelArg(kernel, i, sizeof(cl_mem), &input_buf_list[i]);
            checkError(status, "Failed to set argument %d", i);
        }


        // Enqueue kernel.
        // Use a global work size corresponding to the number of elements to add
        // for this device.
        //
        // We don't specify a local work size and let the runtime choose
        // (it'll choose to use one work-group with the same size as the global
        // work-size).
        //
        // Events are used to ensure that the kernel is not launched until
        // the writes to the input buffers have completed.
        const size_t global_work_size = input_size_list[0];
        printf("Launching for device %d (%zd elements)\n", i, global_work_size);

        status = clEnqueueNDRangeKernel(queue,
                                        kernel,
                                        1,
                                        NULL,
                                        &global_work_size,
                                        NULL,
                                        input_number,
                                        write_event,
                                        &kernel_event);

        checkError(status, "Failed to launch kernel");


        // Read the result. This the final operation.
        status = clEnqueueReadBuffer(queue,
                                     input_buf_list[input_number - 1],
                                     CL_FALSE,
                                     0,
                                     input_size_list[input_number - 1],
                                     input_list[input_number - 1],
                                     1,
                                     &kernel_event,
                                     &finish_event);

        // Release local events.
        for (unsigned i = 0; i < input_number; i++) {
            clReleaseEvent(write_event[i]);
        }

        // Wait for all devices to finish.
        clWaitForEvents(1, &finish_event);

        const double end_time = getCurrentTimestamp();

        // Wall-clock time taken.
        printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

        // Get kernel times using the OpenCL event profiling API.

        cl_ulong time_ns = getStartEndTime(kernel_event);
        printf("Kernel time (device %d): %0.3f ms\n", i, double(time_ns) * 1e-6);

        // Release all events.
        clReleaseEvent(kernel_event);
        clReleaseEvent(finish_event);
    }

private:
    const std::string name;
    const std::vector<size_t> input_size_list;

    const cl_platform_id platform;
    const cl_context context;
    const cl_program program;
    const cl_device_id device_id;
    cl_kernel kernel = NULL;

    cl_command_queue queue;
    std::vector<cl_mem> input_buf_list;
};


#endif //VECTOR_ADD_KERNEL_H
