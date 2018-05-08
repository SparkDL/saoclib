//
// Created by pcz on 18-5-7.
//

#ifndef AOCL_CPP_KERNEL_HPP
#define AOCL_CPP_KERNEL_HPP

#include <vector>
#include <cassert>
#include "AOCLUtils/aocl_utils.h"
#include "FImage.hpp"
#include "KernelArg.hpp"


namespace saoclib {
    using namespace aocl_utils;

    class Kernel {
    public:
        Kernel(const FImage *fimage,
               const std::string &kernel_name,
               const std::vector<KernelDataLimit> &input_limits,
               const KernelDataLimit output_limit,
               const unsigned device_num = 0)
                : fimage(fimage),
                  kernel_name(kernel_name),
                  input_limits(input_limits),
                  output_limit(output_limit),
                  device_num(device_num) {
            cl_int status;
            // create command queue for device
            queue = clCreateCommandQueue(fimage->getContext(), fimage->getDevice(device_num), CL_QUEUE_PROFILING_ENABLE,
                                         &status);
            checkError(status, "Failed to create command queue");

            // create kernel
            kernel = clCreateKernel(fimage->getProgram(), kernel_name.c_str(), &status);
            checkError(status, "Failed to create kernel");

            // create a readonly buffer(which of cl_mem type) for every input
            for (auto &limit : input_limits) {
                if (limit.getType() == KernelDataType::AlignedBuffer) {
                    this->input_mems.push_back(clCreateBuffer(fimage->getContext(), CL_MEM_READ_ONLY,
                                                              limit.getElemSize() * limit.getArrayLength(), NULL,
                                                              &status));
                    checkError(status, "Failed to create buffer for input");
                }
            }

            // create a write-only buffer for output
            if (this->hasOutput()) {
                output_mem = clCreateBuffer(fimage->getContext(), CL_MEM_WRITE_ONLY,
                                            output_limit.getElemSize() * output_limit.getArrayLength(), NULL, &status);
                checkError(status, "Failed to create buffer for input");
            }
        }

        virtual ~Kernel() {
            if (kernel) {
                clReleaseKernel(kernel);
            }
        };

        virtual void call(const std::vector<const KernelArg *> &inputs,
                          KernelArg *output)=0;

        bool hasOutput() {
            return output_limit.getType() == KernelDataType::AlignedBuffer;
        }

    protected:
        const FImage *fimage; /// An FPGA image instance.
        std::string kernel_name; /// Name of kernel.
        unsigned device_num; /// The device number where the kernel will running on.
        cl_kernel kernel;   /// OpenCL Kernel, need to be released in destructor.
        cl_command_queue queue; /// Command queue.

        const std::vector<KernelDataLimit> input_limits; /// Type and size limits of input array.
        std::vector<cl_mem> input_mems;  /// Input buffer(memory object).
        KernelDataLimit output_limit; /// Type and size limit of output.
        cl_mem output_mem = NULL; /// Output buffer(memory object).
    };

    class NDRangeKernel : public Kernel {
    public:
        NDRangeKernel(cl_uint work_dim,
                      const size_t *global_work_size_list,
                      const size_t *local_work_size_list,

                      const FImage *fimage,
                      const std::string &kernel_name,
                      const std::vector<KernelDataLimit> &input_limits,
                      const KernelDataLimit output_limit,
                      const unsigned device_num = 0)
                : Kernel(fimage, kernel_name, input_limits, output_limit, device_num),
                  work_dim(work_dim),
                  global_work_size_list(global_work_size_list),
                  local_work_size_list(local_work_size_list) {
        }

        void call(const std::vector<const KernelArg *> &inputs,
                  KernelArg *output) override {
            printf("Calling kernel '%s'\n", kernel_name.c_str());
            size_t input_size = inputs.size();
            // assure given input parameters meets the limits
            assert(input_size == input_limits.size());
            for (size_t i = 0; i < input_size; i++) {
                auto limit = input_limits[i];
                auto input = inputs[i];
                printf("arg %lu: ", i);
                limit.print();
                assert(input->checkValid(limit));
            }

            cl_int status;
            const double start_time = getCurrentTimestamp();

            // Transfer inputs to device. Each of the host buffers supplied to
            // clEnqueueWriteBuffer here is already aligned to ensure that DMA is used
            // for the host-to-device transfer.
            unsigned input_mems_index = 0;
            for (unsigned i = 0; i < input_size; i++) {
                const KernelArg *input = inputs[i];
                if (input->getType() == KernelDataType::AlignedBuffer) {
                    status = clEnqueueWriteBuffer(queue, input_mems[input_mems_index], CL_FALSE,
                                                  0, input->getSize(),
                                                  input->getReadonlyDataPtr(), 0, NULL, NULL);
                    checkError(status, "Failed to transfer input %d", i);
                    input_mems_index++;
                }
            }
            // wait for queue to finish
            clFinish(queue);

            // set kernel input arguments.
            input_mems_index = 0;
            unsigned argi = 0;
            for (; argi < input_size; argi++) {
                const KernelArg *input = inputs[argi];
                if (input->getType() == KernelDataType::AlignedBuffer) {
                    status = clSetKernelArg(kernel, argi, sizeof(cl_mem), &input_mems[input_mems_index]);
                    checkError(status, "Failed to set argument %d", argi);
                } else {
                    const void *p_data = input->getReadonlyDataPtr();
                    size_t size = input->getSize();
                    status = clSetKernelArg(kernel, argi, size, p_data);
                    checkError(status, "Failed to set argument %d", argi);
                }
            }
            // set kernel output argument
            if (hasOutput()) {
                status = clSetKernelArg(kernel, argi, sizeof(cl_mem), &output_mem);
                checkError(status, "Failed to set argument %d", argi);
            }

            // Enqueue kernel.
            cl_event kernel_event;
            status = clEnqueueNDRangeKernel(queue, kernel, work_dim, NULL,
                                            global_work_size_list, local_work_size_list, 0,
                                            NULL, &kernel_event);
            checkError(status, "Failed to launch kernel");

            // Read the result. This the final operation.
            if (hasOutput()) {
                status = clEnqueueReadBuffer(queue, output_mem, CL_FALSE,
                                             0, output->getSize(), output->getWriteableDataPtr(),
                                             0, NULL, &kernel_event);
            }
            // Wait for kernel to finish.
            clWaitForEvents(1, &kernel_event);

            const double end_time = getCurrentTimestamp();

            // Wall-clock time taken.
            printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

            // Get kernel times using the OpenCL event profiling API.
            cl_ulong time_ns = getStartEndTime(kernel_event);
            printf("Kernel time (device %d): %0.3f ms\n", device_num, double(time_ns) * 1e-6);

            // Release all events.
            clReleaseEvent(kernel_event);
        }

    protected:
        cl_uint work_dim;
        const size_t *global_work_size_list; /// work_dim elements
        const size_t *local_work_size_list; /// work_dim elements
    };


//class SingleItemKernel : NDRangeKernel {
//};
}
#endif //AOCL_CPP_KERNEL_HPP
