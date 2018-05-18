//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_NDRANGEKERNEL_H
#define SAOCLIB_CPP_NDRANGEKERNEL_H

#include "Kernel.h"

namespace saoclib {
    class NDRangeKernel : public Kernel {
    public:
        NDRangeKernel(cl_uint work_dim,
                      const long *global_work_size_list,
                      const long *local_work_size_list,

                      const ClImage *f_image,
                      const cl_device_id device,
                      const std::string &kernel_name,
                      const KernelArgLimit *arg_limits_raw,
                      unsigned num_args)
                : Kernel(f_image, device, kernel_name, arg_limits_raw, num_args),
                  work_dim(work_dim) {
            this->global_work_size_list.reset(work_dim);
            for (unsigned i = 0; i < work_dim; i++) {
                this->global_work_size_list[i] = global_work_size_list[i];
            }

            if (local_work_size_list != NULL) {
                this->local_work_size_list.reset(work_dim);
                for (unsigned i = 0; i < work_dim; i++) {
                    this->local_work_size_list[i] = local_work_size_list[i];
                }
            }
        }

        NDRangeKernel(cl_uint work_dim,
                      const size_t *global_work_size_list,
                      const size_t *local_work_size_list,

                      const ClImage *f_image,
                      const cl_device_id device,
                      const std::string &kernel_name,
                      const KernelArgLimit *arg_limits_raw,
                      unsigned num_args)
                : Kernel(f_image, device, kernel_name, arg_limits_raw, num_args),
                  work_dim(work_dim) {
            this->global_work_size_list.reset(work_dim);
            for (unsigned i = 0; i < work_dim; i++) {
                this->global_work_size_list[i] = global_work_size_list[i];
            }

            if (local_work_size_list != NULL) {
                this->local_work_size_list.reset(work_dim);
                for (unsigned i = 0; i < work_dim; i++) {
                    this->local_work_size_list[i] = local_work_size_list[i];
                }
            }
        }

        bool call(KernelArg **args, unsigned num_args) override {
            printf("Calling kernel '%s'\n", kernel_name.c_str());
            cl_command_queue queue = f_image->getQueueForDevice(device);
            // assure given parameters meets the limits
            assert(num_args == this->num_args && "Wrong arguments count");
            for (size_t i = 0; i < num_args; i++) {
                auto limit = arg_limits[i];
                auto arg = args[i];
                printf("arg %lu: ", i);
                printf("%s\n", limit.toString().c_str());
                assert(arg->verify(limit));
            }

            cl_int status;
            const double start_time = getCurrentTimestamp();

            // Transfer inputs to device. Each of the host buffers supplied to
            // clEnqueueWriteBuffer here is already aligned to ensure that DMA is used
            // for the host-to-device transfer.
            cl_event write_event;
            unsigned arg_mems_index = 0;
            for (unsigned i = 0; i < num_args; i++) {
                const KernelArg *arg = args[i];
                if (arg->isArray() && arg->getMode() == KernelArgMode::mode_input) {
                    status = clEnqueueWriteBuffer(f_image->getQueueForDevice(device), arg_mems[arg_mems_index],
                                                  CL_FALSE, 0, arg->getSize(),
                                                  arg->getReadonlyDataPtr(), 0, NULL, &write_event);
                    checkError(status, "Failed to transfer input %d", i);
                    arg_mems_index++;
                }
            }
            // wait for writing action to finish
            clWaitForEvents(1, &write_event);

            // set kernel arguments.
            arg_mems_index = 0;
            for (unsigned i = 0; i < num_args; i++) {
                KernelArg *arg = args[i];
                if (arg->isArray()) {
                    status = clSetKernelArg(kernel, i, sizeof(cl_mem), &arg_mems[arg_mems_index]);
                    checkError(status, "Failed to set argument %d", i);
                    arg_mems_index++;
                } else {
                    const void *p_data = arg->getReadonlyDataPtr();
                    size_t size = arg->getSize();
                    status = clSetKernelArg(kernel, i, size, p_data);
                    checkError(status, "Failed to set argument %d", i);
                }
            }

            // Enqueue kernel.
            cl_event kernel_event;
            status = clEnqueueNDRangeKernel(queue, kernel, work_dim, NULL,
                                            global_work_size_list.get(), local_work_size_list.get(), 0,
                                            NULL, &kernel_event);
            checkError(status, "Failed to launch kernel");
            // Wait for kernel to finish.
            clWaitForEvents(1, &kernel_event);

            // Read the result. This the final operation.
            scoped_array<cl_event> read_events(num_args);
            unsigned num_read_events = 0;
            arg_mems_index = 0;
            for (unsigned i = 0; i < num_args; i++) {
                KernelArg *arg = args[i];
                if (arg->isArray()) {
                    if (arg->getMode() == KernelArgMode::mode_output) {
                        status = clEnqueueReadBuffer(queue, arg_mems[arg_mems_index], CL_FALSE,
                                                     0, arg_limits[i].getSize(), arg->getWriteableDataPtr(),
                                                     0, NULL, &read_events[num_read_events]);
                        checkError(status, "Failed to read buffer for output");
                        num_read_events++;
                    }
                    arg_mems_index++;
                }
            }
            clWaitForEvents(num_read_events, read_events.get());

            const double end_time = getCurrentTimestamp();
            // Wall-clock time taken.
            printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

            // Get kernel times using the OpenCL event profiling API.
            cl_ulong time_ns = getStartEndTime(kernel_event);
            printf("Kernel time (device %p): %0.3f ms\n", device, double(time_ns) * 1e-6);

            // Release all events.
            for (unsigned i = 0; i < num_read_events; i++) {
                clReleaseEvent(read_events[i]);
            }
            clReleaseEvent(write_event);
            clReleaseEvent(kernel_event);
        }

    protected:
        cl_uint work_dim;
        scoped_array<size_t> global_work_size_list; /// work_dim elements
        scoped_array<size_t> local_work_size_list; /// work_dim elements
    };
}

#endif //SAOCLIB_CPP_NDRANGEKERNEL_H
