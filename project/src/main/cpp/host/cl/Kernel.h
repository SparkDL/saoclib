//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_HPP
#define SAOCLIB_CPP_KERNEL_HPP

#include <vector>
#include <cassert>
#include "AOCLUtils/aocl_utils.h"
#include "CLBinary.h"
#include "KernelArg.h"


namespace saoclib {
    using namespace aocl_utils;

    // TODO CheckError with cleanup
    class Kernel {
    public:
        Kernel(const CLBinary *binary,
               const cl_device_id device,
               const std::string &kernelName,
               const KernelArgSignature *signatures,
               unsigned numArgs);

        virtual ~Kernel();;

        void cleanup();

        virtual bool call(cl_uint work_dim,
                          const size_t *global_work_size_list,
                          const size_t *local_work_size_list,
                          KernelArg **args,
                          unsigned num_args)=0;

    protected:
        const CLBinary *binary; /// An FPGA image instance.
        std::string kernelName; /// Name of kernel.
        cl_device_id device; /// The device id of the device where the kernel will running on.
        cl_kernel kernel;   /// OpenCL Kernel, need to be released in destructor.

        scoped_array<KernelArgSignature> signatures; /// Type and size limits of input array.
        unsigned numArgs;
        std::vector<cl_mem> buffers;  /// Input and output buffers(memory object).
        std::vector<size_t> bufferSizes;
    };



//class SingleItemKernel : NDRangeKernel {
//};
}
#endif //SAOCLIB_CPP_KERNEL_HPP
