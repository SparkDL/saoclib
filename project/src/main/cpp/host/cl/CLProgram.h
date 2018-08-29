//
// Created by pcz on 18-4-18.
//

#ifndef SAOCLIB_CPP_CLPROGRAM_H
#define SAOCLIB_CPP_CLPROGRAM_H

#include <cassert>
#include "CLContext.h"
#include "AOCLUtils/aocl_utils.h"

namespace saoclib {
    using namespace aocl_utils;

    // TODO CheckError with cleanup
    /**
     * This class must be *thread safe*
     * @note we assume all devices are of the same type
     */
    class CLProgram {
    public:
        /**
         *
         * @param context : The CLContext object which manages the OpenCL devices.
         * @param imagePath : The OpenCL binary path, *.aocx
         */
        CLProgram(CLContext *context, const std::string &imagePath);

        /**
         * Destructor
         */
        ~CLProgram();


        /**
         * Deploy the binary to specified devices
         * @param devices
         * @return
         */
        bool deploy(const std::vector<cl_device_id> &devices);

        /**
         * Getter of context
         * @return
         */
        const CLContext *getContext() const;

        /**
         * Getter of binaryPath
         */
        const std::string &getBinaryPath() const;

        /**
         * Getter of program
         * @return
         */
        const cl_program getProgram() const;

        /**
         * Get the number of the devices
         * @return
         */
        int getNumDeployedDevices() const;

        /**
         * Getter of deployedDevices
         * @return
         */
        const std::vector<cl_device_id> &getDeployedDevices() const;

        /**
         * Get the i'th element of deployedDevices
         * @param index
         * @return
         */
        cl_device_id getDeployedDevice(int index) const;

        /**
         * cleanup resources,
         * including kernels, command queues, buffers, program and context
         */
        void cleanup();

    protected:
        /// CLContext object that manages OpenCL devices on this machine
        const CLContext *context = NULL;

        /// Binary path: xxx.aocx
        const std::string binaryPath;

        /// OpenCL program object
        cl_program program = NULL;

        /// Devices that configured with this OpenCL binary
        std::vector<cl_device_id> deployedDevices;
    };
}

#endif //SAOCLIB_CPP_CLPROGRAM_H
