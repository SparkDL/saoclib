//
// Created by pcz on 18-4-18.
//

#ifndef SAOCLIB_CPP_CLIMAGE_H
#define SAOCLIB_CPP_CLIMAGE_H

#include "CLEnv.h"
#include "AOCLUtils/aocl_utils.h"

namespace saoclib {
    using namespace aocl_utils;

    /**
     * @note we assume all devices are of the same type
     */
    // TODO CheckError with cleanup
    class CLBinary {
    public:
        CLBinary(const CLEnv *env, const std::string &image_path);

        ~CLBinary();

        bool deploy(const cl_device_id *devices, unsigned num_given_devices);


        const CLEnv *getEnv() const;

        const std::string &getImagePath() const;

        const cl_program getProgram() const;

        unsigned int getNumDeployedDevices() const;

        const scoped_array<cl_device_id> &getDeployedDevices() const;

        cl_device_id getDeployedDevice(int index) const;

        const scoped_array<cl_command_queue> &getDeviceQueues() const;

        const cl_command_queue getDeviceQueue(cl_device_id id) const;

        /**
         * cleanup resources,
         * including kernels, command queues, buffers, program and context
         */
        void cleanup();

    private:
        const CLEnv *env = NULL;
        const std::string binaryPath;                  /// image path: xxx.aocx
        cl_program program = NULL;                     /// opencl program

        unsigned numDeployedDevices = 0;
        scoped_array<cl_device_id> deployedDevices;
        scoped_array<cl_command_queue> deviceQueues;
    };
}

#endif //SAOCLIB_CPP_CLIMAGE_H
