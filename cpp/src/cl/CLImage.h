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
    class ClImage {
    public:
        ClImage(const CLEnv *f_env, const std::string &image_path);

        ~ClImage();

        bool deployImage(const cl_device_id *devices, unsigned num_given_devices);

        const cl_command_queue getQueueForDevice(cl_device_id id) const;

        const CLEnv *getEnv() const;

        const std::string &getImagePath() const;

        const cl_program getProgram() const;

        unsigned int getNumAttachedDevices() const;

        const scoped_array<cl_device_id> &getAttachedDevices() const;

        const scoped_array<cl_command_queue> &getAttachedQueues() const;

        /**
         * cleanup resources,
         * including kernels, command queues, buffers, program and context
         */
        void cleanup();

    private:
        const CLEnv *f_env = NULL;
        const std::string image_path;                  /// image path: xxx.aocx
        cl_program program = NULL;                     /// opencl program

        unsigned num_attached_devices = 0;
        scoped_array<cl_device_id> attached_devices;
        scoped_array<cl_command_queue> attached_queues;
    };
}

#endif //SAOCLIB_CPP_CLIMAGE_H
