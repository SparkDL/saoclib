//
// Created by pcz on 18-5-17.
//
#include "Kernel.h"

namespace acl {

    Kernel::Kernel(const Program *program,
                   const cl_device_id device,
                   const cl_command_queue queue,
                   const std::string &kernelName,
                   const KernelArgSignature *signatures,
                   unsigned numArgs)
            : binary(program),
              device(device),
              queue(queue),
              kernelName(kernelName),
              numArgs(numArgs),
              buffers(numArgs),
              bufferSizes(numArgs) {
        for (int i = 0; i < numArgs; i++) {
            buffers[i] = NULL;
            bufferSizes[i] = 0;
        }
        cl_int status;

        // Create kernel
        kernel = clCreateKernel(program->getProgram(), kernelName.c_str(), &status);
        checkError(status, "Failed to create kernel");

        // Create buffer for arguments
        this->signatures.reset(numArgs);
        for (unsigned i = 0; i < numArgs; i++) {
            auto sig = signatures[i];
            this->signatures[i] = sig;
        }
    }

    Kernel::~Kernel() {
        cleanup();
    }

    void Kernel::cleanup() {
        log("Cleaning up kernel resources: %s", this->kernelName.c_str());
        if (kernel) {
            clReleaseKernel(kernel);
            kernel = NULL;
        }
        for (auto &mem:buffers) {
            if (mem) {
                clReleaseMemObject(mem);
                mem = NULL;
            }
        }
    }

}