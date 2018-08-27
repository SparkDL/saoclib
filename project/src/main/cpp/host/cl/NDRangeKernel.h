//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_NDRANGEKERNEL_H
#define SAOCLIB_CPP_NDRANGEKERNEL_H

#include "Kernel.h"

namespace saoclib {
    class NDRangeKernel : public Kernel {
    public:
        NDRangeKernel(const CLBinary *binary,
                      const cl_device_id device,
                      const std::string &kernel_name,
                      const KernelArgSignature *signatures,
                      unsigned num_args);

        ~NDRangeKernel() override;

        /**
         * Call a kernel with arguments
         * @param work_dim
         * @param global_work_size_list
         * @param local_work_size_list
         * @param args
         * @param numArgs
         * @return
         */
        bool call(cl_uint work_dim,
                  const size_t *global_work_size_list,
                  const size_t *local_work_size_list,
                  KernelArg **args,
                  unsigned numArgs);

    private:
        void setInputs(KernelArg **args, int numArgs);

        void callKernel(cl_uint work_dim,
                        const size_t *global_work_size_list,
                        const size_t *local_work_size_list);

        void getOutputs(KernelArg **args, int numArgs);

        /**
         * Create buffer for the i'th argument
         * @param i : argument index
         * @param mode : input/output/input_output
         * @param size : buffer size
         * @return
         */
        void createBuffer(int i, KernelArgMode mode, size_t size);

        void writeBuffer(int i, const KernelArg *arg, cl_event *event);

        bool checkArgs(KernelArg **args);
    };
}

#endif //SAOCLIB_CPP_NDRANGEKERNEL_H
