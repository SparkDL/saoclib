//
// Created by pcz on 18-5-17.
//

#include <iostream>
#include "NDRangeKernel.h"

namespace saoclib {

    NDRangeKernel::NDRangeKernel(const CLBinary *binary,
                                 const cl_device_id device,
                                 const std::string &kernel_name,
                                 const KernelArgSignature *signatures,
                                 unsigned num_args)
            : Kernel(binary, device, kernel_name, signatures, num_args) {}

    NDRangeKernel::~NDRangeKernel() {}

    bool NDRangeKernel::call(cl_uint work_dim,
                             const size_t *global_work_size_list,
                             const size_t *local_work_size_list,
                             KernelArg **args,
                             unsigned numArgs) {
        log("\nCalling %s\n", kernelName.c_str());
        cl_command_queue queue = binary->getDeviceQueue(device);

        /*
         * Check kernel arguments signature
         */
        assert(numArgs == this->numArgs && "Wrong arguments count");
        // checkArgs(args);

        const double start_time = getCurrentTimestamp();

        /*
         * Transfer inputs to device. Each of the host buffers supplied to
         * clEnqueueWriteBuffer here is already aligned to ensure that DMA is used
         * for the host-to-device transfer.
         */
        executeTime(
                [=]() {
                    this->setInputs(args, numArgs);
                },
                (kernelName + "-setInputs").c_str()
        );

        /*
         * Enqueue kernel.
         */
        executeTime(
                [=]() {
                    this->callKernel(work_dim, global_work_size_list, local_work_size_list);
                },
                (kernelName + "-callKernel").c_str()
        );


        /*
         * Read the result. This the final operation.
         */
        executeTime(
                [=]() {
                    this->getOutputs(args, numArgs);
                },
                (kernelName + "-getOutputs").c_str()
        );

        const double end_time = getCurrentTimestamp();
        // Wall-clock time taken.
        log("Total time(%s): %0.3f ms\n\n", kernelName.c_str(), (end_time - start_time) * 1e3);
    }

    void NDRangeKernel::setInputs(KernelArg **args, int numArgs) {
        cl_int status;
        std::vector<cl_event> write_events;
        for (unsigned i = 0; i < numArgs; i++) {
            cl_event event = NULL;
            const KernelArg *arg = args[i];
            auto &sig = arg->getSignature();
            if (sig.isArray()) {
                createBuffer(i, sig.getMode(), sig.getSize());
                if (sig.isInput()) {
                    writeBuffer(i, arg, &event);
                    write_events.push_back(event);
                }
            }
        }
        clWaitForEvents(write_events.size(), write_events.data());
        for (auto &e:write_events) {
            clReleaseEvent(e);
        }

        for (unsigned i = 0; i < numArgs; i++) {
            KernelArg *arg = args[i];
            if (arg->getSignature().isArray()) {
                status = clSetKernelArg(kernel, i, sizeof(cl_mem), &buffers[i]);
                checkError(status, "Failed to set argument %d", i);
            } else {
                const void *p_data = arg->getReadonlyDataPtr();
                size_t size = arg->getSignature().getSize();
                status = clSetKernelArg(kernel, i, size, p_data);
                checkError(status, "Failed to set argument %d", i);
            }
        }
    }

    void NDRangeKernel::callKernel(cl_uint work_dim,
                                   const size_t *global_work_size_list,
                                   const size_t *local_work_size_list) {
        auto queue = this->binary->getDeviceQueue(device);
        cl_int status;
        cl_event kernel_event;
        status = clEnqueueNDRangeKernel(queue,
                                        kernel,
                                        work_dim,
                                        NULL,
                                        global_work_size_list,
                                        local_work_size_list,
                                        0,
                                        NULL, &kernel_event);
        checkError(status, "Failed to launch kernel");
        // Wait for kernel to finish.
        clWaitForEvents(1, &kernel_event);

        // Get kernel times using the OpenCL event profiling API.
        // cl_ulong time_ns = getStartEndTime(kernel_event);
        // printf("Kernel time (device %p): %0.3f ms\n", device, double(time_ns) * 1e-6);

        clReleaseEvent(kernel_event);
    }

    void NDRangeKernel::getOutputs(KernelArg **args, int numArgs) {
        cl_int status;
        auto queue = this->binary->getDeviceQueue(device);
        std::vector<cl_event> read_events;
        for (unsigned i = 0; i < numArgs; i++) {
            KernelArg *arg = args[i];
            auto &sig = arg->getSignature();
            if (sig.isArray() && sig.isOutput()) {
                cl_event event;
                status = clEnqueueReadBuffer(queue,
                                             buffers[i],
                                             CL_FALSE,
                                             0,
                                             sig.getSize(),
                                             arg->getWriteableDataPtr(),
                                             0,
                                             NULL,
                                             &event);
                read_events.push_back(event);
                checkError(status, "Failed to read buffer for output");
            }
        }
        clWaitForEvents(read_events.size(), read_events.data());
        for (auto &e:read_events) {
            clReleaseEvent(e);
        }
    }


    void NDRangeKernel::createBuffer(int i, KernelArgMode mode, size_t size) {
        cl_int status;
        auto context = this->binary->getEnv()->getContext();
        // Only create new buffer when the old buffer is not sufficient
        if (size > this->bufferSizes[i]) {
            switch (mode) {
                case KernelArgMode::input:
                    this->buffers[i] = clCreateBuffer(context,
                                                      CL_MEM_READ_ONLY,
                                                      size,
                                                      NULL,
                                                      &status);
                    break;
                case KernelArgMode::output:
                    this->buffers[i] = clCreateBuffer(context,
                                                      CL_MEM_WRITE_ONLY,
                                                      size,
                                                      NULL,
                                                      &status);
                    break;
                case KernelArgMode::input_output:
                    this->buffers[i] = clCreateBuffer(context,
                                                      CL_MEM_READ_WRITE,
                                                      size,
                                                      NULL,
                                                      &status);
                    break;
            }
            this->bufferSizes[i] = size;
        }
    }

    void NDRangeKernel::writeBuffer(int i, const KernelArg *arg, cl_event *event) {
        auto queue = this->binary->getDeviceQueue(device);
        cl_int status;
        status = clEnqueueWriteBuffer(queue,
                                      buffers[i],
                                      CL_FALSE,
                                      0,
                                      arg->getSignature().getSize(),
                                      arg->getReadonlyDataPtr(),
                                      0,
                                      NULL,
                                      event);
        checkError(status, "Failed to transfer input %d", i);
    }

    bool NDRangeKernel::checkArgs(KernelArg **args) {
        for (int i = 0; i < numArgs; i++) {
            auto limit = signatures[i];
            auto arg = args[i];
            if (!arg->verify(limit)) {
                return false;
            }
        }
    }


}