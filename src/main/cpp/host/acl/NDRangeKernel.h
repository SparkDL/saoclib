//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_NDRANGEKERNEL_H
#define SAOCLIB_CPP_NDRANGEKERNEL_H

#include <mutex>
#include "Kernel.h"
#include "common.h"

namespace acl {
    class NDRangeKernel : public Kernel {
    public:
        NDRangeKernel(const Program *binary,
                      const cl_device_id device,
                      const cl_command_queue queue,
                      const std::string &kernel_name,
                      const KernelArgSignature *signatures,
                      unsigned num_args);

        ~NDRangeKernel() override;


        bool refreshBenchmarkResultFile() override;

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
         * @param argi : argument index
         * @param mode : input/output/input_output
         * @param size : buffer size
         * @return
         */
        void createBuffer(int argi, int bufferi, KernelArgMode mode, size_t size);

        bool checkArgs(KernelArg **args);

        std::vector<float> kernelTime;
        std::vector<float> totalTime;
#ifdef BENCHMARK
        std::mutex fileLock;
#endif
    };
}

#endif //SAOCLIB_CPP_NDRANGEKERNEL_H
