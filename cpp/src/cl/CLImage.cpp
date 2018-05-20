//
// Created by pcz on 18-5-17.
//

#include "CLImage.h"

namespace saoclib {

    CLImage::CLImage(const CLEnv *f_env, const std::string &image_path)
            : f_env(f_env), image_path(image_path) {}

    CLImage::~CLImage() {
        cleanup();
    }

    bool CLImage::deployImage(const cl_device_id *devices, unsigned num_given_devices) {
        cleanup();
        attached_devices.reset(num_given_devices);
        attached_queues.reset(num_given_devices);

        cl_int status;
        unsigned num_actual_devices = f_env->getNumDevices();
        if (num_given_devices > num_actual_devices) {
            printf("Given devices: %u, Actual devices: %u", num_given_devices, num_actual_devices);
            return false;
        }

        num_attached_devices = num_given_devices;

        for (unsigned j = 0; j < num_given_devices; j++) {
            auto given_id = devices[j];
            if (!f_env->verifyDeviceID(given_id)) {
                printf("Invalid device id: %p", given_id);
                return false;
            }
            attached_devices[j] = f_env->getDeviceID(j);
        }

        // Create the program for all device. Use the first device as the
        // representative device (assuming all device are of the same type).
        std::string binary_file = getBoardBinaryFile(image_path.c_str(), attached_devices[0]);
        printf("Using AOCX: %s\n", binary_file.c_str());
        program = createProgramFromBinary(f_env->getContext(), binary_file.c_str(),
                                          attached_devices.get(), num_attached_devices);

        // Build the program that was just created.
        status = clBuildProgram(program, num_given_devices, attached_devices.get(), "", NULL, NULL);
        checkError(status, "Failed to build program");

        // create command queue for device
        for (unsigned i = 0; i < num_given_devices; i++) {
            attached_queues[i] = clCreateCommandQueue(f_env->getContext(), attached_devices[i],
                                                      CL_QUEUE_PROFILING_ENABLE, &status);
            checkError(status, "Failed to create command queue");
        }
        return true;
    }

    const cl_command_queue CLImage::getQueueForDevice(cl_device_id id) const {
        for (unsigned i = 0; i < num_attached_devices; i++) {
            if (attached_devices[i] == id) {
                return attached_queues[i];
            }
        }
        printf("Invalid device id: %p", id);
        return NULL;
    }

    const CLEnv *CLImage::getEnv() const {
        return f_env;
    }

    const std::string &CLImage::getImagePath() const {
        return image_path;
    }

    const cl_program CLImage::getProgram() const {
        return program;
    }

    unsigned int CLImage::getNumAttachedDevices() const {
        return num_attached_devices;
    }

    const scoped_array<cl_device_id> &CLImage::getAttachedDevices() const {
        return attached_devices;
    }

    const scoped_array<cl_command_queue> &CLImage::getAttachedQueues() const {
        return attached_queues;
    }

    void CLImage::cleanup() {
        this->attached_devices.reset();
        this->num_attached_devices = 0;
        // release program
        if (program) {
            clReleaseProgram(program);
        }
    }
}