//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_HPP
#define SAOCLIB_CPP_KERNEL_HPP

#include <vector>
#include <cassert>
#include "AOCLUtils/aocl_utils.h"
#include "Program.h"
#include "KernelArg.h"


namespace acl {
    using namespace aocl_utils;

    // TODO CheckError with cleanup
    class Kernel {
    public:
        Kernel(const Program *program,
               const cl_device_id device,
               const cl_command_queue queue,
               const std::string &kernelName,
               const KernelArgSignature *signatures,
               unsigned numArgs);


        virtual ~Kernel();

        virtual bool refreshBenchmarkResultFile()=0;

        void cleanup();

        virtual bool call(cl_uint work_dim,
                          const size_t *global_work_size_list,
                          const size_t *local_work_size_list,
                          KernelArg **args,
                          unsigned num_args)=0;

    protected:
        /// An FPGA image instance.
        const Program *binary;

        /// Name of kernel.
        std::string kernelName;

        /// The device where the kernel will running on.
        cl_device_id device;

        /// Queue used to invoke kernel
        cl_command_queue queue;

        /// OpenCL Kernel, need to be released in destructor.
        cl_kernel kernel;

        /// Signature of kernel parameters
        scoped_array<KernelArgSignature> signatures;

        /// Number of kernel parameters
        unsigned numArgs;

        /// Input and output buffers(memory object) used to communicate with device
        std::vector<cl_mem> buffers;

        /// Buffer size
        std::vector<size_t> bufferSizes;
    };
}
#endif //SAOCLIB_CPP_KERNEL_HPP
