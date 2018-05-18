//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_HPP
#define SAOCLIB_CPP_KERNEL_HPP

#include <vector>
#include <cassert>
#include "AOCLUtils/aocl_utils.h"
#include "CLImage.h"
#include "KernelArg.h"


namespace saoclib {
    using namespace aocl_utils;

    // TODO CheckError with cleanup
    class Kernel {
    public:
        Kernel(const ClImage *f_image,
               const cl_device_id device,
               const std::string &kernel_name,
               const KernelArgLimit *arg_limits_raw,
               unsigned num_args);

        virtual ~Kernel();;

        void cleanup();

        virtual bool call(KernelArg **args, unsigned num_args)=0;

    protected:
        const ClImage *f_image; /// An FPGA image instance.
        std::string kernel_name; /// Name of kernel.
        cl_device_id device; /// The device id of the device where the kernel will running on.
        cl_kernel kernel;   /// OpenCL Kernel, need to be released in destructor.

        scoped_array<KernelArgLimit> arg_limits; /// Type and size limits of input array.
        unsigned num_args;
        std::vector<cl_mem> arg_mems;  /// Input and output buffers(memory object).
    };



//class SingleItemKernel : NDRangeKernel {
//};
}
#endif //SAOCLIB_CPP_KERNEL_HPP
