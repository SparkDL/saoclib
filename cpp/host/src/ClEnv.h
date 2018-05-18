//
// Created by pcz on 18-4-18.
//

#ifndef SAOCLIB_CPP_CLENV_H
#define SAOCLIB_CPP_CLENV_H

#include "AOCLUtils/aocl_utils.h"

namespace saoclib {
    using namespace aocl_utils;

    // TODO CheckError with cleanup
    class ClEnv {
    public:
        ClEnv();

        ~ClEnv();

        /**
         * Init OpenCL resources,
         * including platform, device, context and program
         * @return
         */
        bool initOpenCL();

        bool verifyDeviceID(cl_device_id id) const;

        const std::string &getPlatformName() const;

        unsigned getNumDevices() const;

        const scoped_array<cl_device_id> & getDevices();

        cl_device_id getDeviceID(unsigned num) const;

        const cl_platform_id getPlatformId() const;

        cl_context getContext() const;

        void cleanup();

    private:
        std::string platformName = "";
        cl_platform_id platform = NULL;                /// platform id
        scoped_array<cl_device_id> devices;            /// the array storing devices ids
        cl_context context = NULL;                     /// opencl context
        unsigned num_devices = 0;                      /// the number of devices connected in PCIE slots
    };



}
#endif //SAOCLIB_CPP_CLENV_H
