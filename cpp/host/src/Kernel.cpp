//
// Created by pcz on 18-5-17.
//
#include "Kernel.h"

namespace saoclib {

    Kernel::Kernel(const ClImage *f_image, const cl_device_id device, const std::string &kernel_name,
                   const KernelArgLimit *arg_limits_raw, unsigned num_args) : f_image(f_image),
                                                                              device(device),
                                                                              kernel_name(kernel_name),
                                                                              num_args(num_args) {
        cl_int status;

        // create kernel
        kernel = clCreateKernel(f_image->getProgram(), kernel_name.c_str(), &status);
        checkError(status, "Failed to create kernel");

        // create buffer for arguments
        this->arg_limits.reset(num_args);
        for (unsigned i = 0; i < num_args; i++) {
            auto limit = arg_limits_raw[i];
            this->arg_limits[i] = limit;

            if (limit.isArray()) {
                switch (limit.getMode()) {
                    case KernelArgMode::mode_input:
                        this->arg_mems.push_back(clCreateBuffer(f_image->getEnv()->getContext(), CL_MEM_READ_ONLY,
                                                                limit.getElemSize() * limit.getArrayLength(), NULL,
                                                                &status));
                        break;
                    case KernelArgMode::mode_output:
                        this->arg_mems.push_back(clCreateBuffer(f_image->getEnv()->getContext(), CL_MEM_WRITE_ONLY,
                                                                limit.getElemSize() * limit.getArrayLength(), NULL,
                                                                &status));
                        break;
                    case KernelArgMode::mode_input_output:
                        this->arg_mems.push_back(clCreateBuffer(f_image->getEnv()->getContext(), CL_MEM_READ_WRITE,
                                                                limit.getElemSize() * limit.getArrayLength(), NULL,
                                                                &status));
                        break;
                }
                checkError(status, "Failed to create buffer for input");
            }
        }
    }

    Kernel::~Kernel() {
        cleanup();
    }

    void Kernel::cleanup() {
        if (kernel) {
            clReleaseKernel(kernel);
        }
        for (auto &mem:arg_mems) {
            if (mem) {
                clReleaseMemObject(mem);
            }
        }
    }
}