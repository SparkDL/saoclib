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
                      unsigned num_args);

        NDRangeKernel(cl_uint work_dim,
                      const size_t *global_work_size_list,
                      const size_t *local_work_size_list,

                      const ClImage *f_image,
                      const cl_device_id device,
                      const std::string &kernel_name,
                      const KernelArgLimit *arg_limits_raw,
                      unsigned num_args);

        bool call(KernelArg **args, unsigned num_args) override;

    protected:
        cl_uint work_dim;
        scoped_array<size_t> global_work_size_list; /// work_dim elements
        scoped_array<size_t> local_work_size_list; /// work_dim elements
    };
}

#endif //SAOCLIB_CPP_NDRANGEKERNEL_H
