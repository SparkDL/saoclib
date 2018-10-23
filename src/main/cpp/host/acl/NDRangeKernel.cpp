//
// Created by pcz on 18-5-17.
//

#include <iostream>
#include <fstream>
#include "NDRangeKernel.h"

namespace acl {

    NDRangeKernel::NDRangeKernel(const Program *binary,
                                 const cl_device_id device,
                                 const cl_command_queue queue,
                                 const std::string &kernel_name,
                                 const KernelArgSignature *signatures,
                                 unsigned num_args)
            : Kernel(binary, device, queue, kernel_name, signatures, num_args) {}

    NDRangeKernel::~NDRangeKernel() {
        fileLock.unlock();
    }

    bool NDRangeKernel::refreshBenchmarkResultFile() {
#ifdef BENCHMARK
        fileLock.lock();
        std::ofstream f("/tmp/time.txt");
        auto l = this->kernelTime.size();
        assert(l == this->totalTime.size());
        for (int i = 0; i < l; i++) {
            f << this->kernelTime[i] << "," << this->totalTime[i] << std::endl;
        }
        f.close();

        this->kernelTime.resize(0);
        this->totalTime.resize(0);
        fileLock.unlock();
#endif
        return true;
    }

    bool NDRangeKernel::

    call(cl_uint work_dim,
         const size_t *global_work_size_list,
         const size_t *local_work_size_list,
         KernelArg **args,
         unsigned numArgs) {
        log("\nCalling %s\n", kernelName.c_str());
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
//         */
//        executeTime(
//                [=]() {
//                    this->setInputs(args, numArgs);
//                },
//                (kernelName + "-setInputs").c_str()
//        );
//
//        /*
//         * Enqueue kernel.
//         */
//        executeTime(
//                [=]() {
//                    this->callKernel(work_dim, global_work_size_list, local_work_size_list);
//                },
//                (kernelName + "-callKernel").c_str()
//        );
//
//
//        /*
//         * Read the result. This the final operation.
//         */
//        executeTime(
//                [=]() {
//                    this->getOutputs(args, numArgs);
//                },
//                (kernelName + "-getOutputs").c_str()
//        );

        this->setInputs(args, numArgs);
        this->callKernel(work_dim, global_work_size_list, local_work_size_list);
        this->getOutputs(args, numArgs);
        const double end_time = getCurrentTimestamp();
        const float time_us = float(end_time - start_time) * 1e6f;
        this->totalTime.push_back(time_us);
        // Wall-clock time taken.
        log("Total time(%s): %0.3f us\n\n", kernelName.c_str(), time_us);
    }


    void NDRangeKernel::callKernel(cl_uint work_dim,
                                   const size_t *global_work_size_list,
                                   const size_t *local_work_size_list) {
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
        cl_ulong time_ns = getStartEndTime(kernel_event);
        float time_us = time_ns * 1e-3f;
        log("Kernel time (device %p): %0.3f us\n", device, time_us);

        this->kernelTime.push_back(time_us);
        clReleaseEvent(kernel_event);
    }

    void NDRangeKernel::setInputs(KernelArg **args, int numArgs) {
        std::vector<cl_event> write_events;
        int bufferi = 0;
        for (unsigned i = 0; i < numArgs; i++) {
            cl_event event = NULL;
            KernelArg *arg = args[i];
            auto &sig = arg->getSignature();
            if (sig.isArray()) {
                createBuffer(i, bufferi, sig.getMode(), arg->bufferSize());
                bufferi++;
                arg->bindBuffer(&buffers[i]);
                if (sig.isInput()) {
                    // arg->syncWrite(i, queue);
                    arg->asyncWrite(i, queue, &event);
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
            arg->setArg(kernel, i);
        }
    }

    void NDRangeKernel::getOutputs(KernelArg **args, int numArgs) {
        for (unsigned i = 0; i < numArgs; i++) {
            KernelArg *arg = args[i];
            auto &sig = arg->getSignature();
            if (sig.isArray() && sig.isOutput()) {
                arg->syncRead(i, queue);
            }
        }
    }


    void NDRangeKernel::createBuffer(int argi, int bufferi, KernelArgMode mode, size_t size) {
        cl_int status;
        auto context = this->binary->getContext()->getRawContext();
        auto mem_flag = CL_CHANNEL_AUTO_INTELFPGA;
        switch (bufferi) {
            case 0:
                mem_flag = CL_CHANNEL_1_INTELFPGA;
                break;
            case 1:
                mem_flag = CL_CHANNEL_2_INTELFPGA;
                break;
            case 2:
                mem_flag = CL_CHANNEL_3_INTELFPGA;
                break;
            case 3:
                mem_flag = CL_CHANNEL_4_INTELFPGA;
                break;
            case 4:
                mem_flag = CL_CHANNEL_5_INTELFPGA;
                break;
            case 5:
                mem_flag = CL_CHANNEL_6_INTELFPGA;
                break;
            case 6:
                mem_flag = CL_CHANNEL_7_INTELFPGA;
                break;
        }
        // Only create new buffer when the old buffer is not sufficient
        if (size > this->bufferSizes[argi]) {
            // Release the old buffer first
            if (buffers[argi]) {
                clReleaseMemObject(buffers[argi]);
            }
            switch (mode) {
                case KernelArgMode::input:
                    this->buffers[argi] = clCreateBuffer(context,
                                                         CL_MEM_READ_ONLY | mem_flag,
                                                         size,
                                                         NULL,
                                                         &status);
                    break;
                case KernelArgMode::output:
                    this->buffers[argi] = clCreateBuffer(context,
                                                         CL_MEM_WRITE_ONLY | mem_flag,
                                                         size,
                                                         NULL,
                                                         &status);
                    break;
                case KernelArgMode::input_output:
                    this->buffers[argi] = clCreateBuffer(context,
                                                         CL_MEM_READ_WRITE | mem_flag,
                                                         size,
                                                         NULL,
                                                         &status);
                    break;
            }
            this->bufferSizes[argi] = size;
        }
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