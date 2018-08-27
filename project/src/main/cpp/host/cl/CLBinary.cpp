//
// Created by pcz on 18-5-17.
//

#include <cassert>
#include "CLBinary.h"

namespace saoclib {

    CLBinary::CLBinary(const CLEnv *env, const std::string &image_path)
            : env(env), binaryPath(image_path) {}

    CLBinary::~CLBinary() {
        cleanup();
    }

    bool CLBinary::deploy(const cl_device_id *devices, unsigned num_given_devices) {
        cleanup();
        deployedDevices.reset(num_given_devices);
        deviceQueues.reset(num_given_devices);

        cl_int status;
        unsigned num_actual_devices = env->getNumDevices();
        if (num_given_devices > num_actual_devices) {
            printf("Given devices: %u, Actual devices: %u", num_given_devices, num_actual_devices);
            return false;
        }

        numDeployedDevices = num_given_devices;

        for (unsigned j = 0; j < num_given_devices; j++) {
            auto given_id = devices[j];
            if (!env->verifyDeviceID(given_id)) {
                printf("Invalid device id: %p", given_id);
                return false;
            }
            deployedDevices[j] = env->getDeviceID(j);
        }

        // Create the program for all device. Use the first device as the
        // representative device (assuming all device are of the same type).
        std::string binary_file = getBoardBinaryFile(binaryPath.c_str(), deployedDevices[0]);
        printf("Using AOCX: %s\n", binary_file.c_str());
        program = createProgramFromBinary(env->getContext(), binary_file.c_str(),
                                          deployedDevices.get(), numDeployedDevices);

        // Build the program that was just created.
        status = clBuildProgram(program, num_given_devices, deployedDevices.get(), "", NULL, NULL);
        checkError(status, "Failed to build program");

        // create command queue for device
        for (unsigned i = 0; i < num_given_devices; i++) {
            deviceQueues[i] = clCreateCommandQueue(env->getContext(), deployedDevices[i],
                                                   CL_QUEUE_PROFILING_ENABLE, &status);
            checkError(status, "Failed to create command queue");
        }
        return true;
    }

    const CLEnv *CLBinary::getEnv() const {
        return env;
    }

    const std::string &CLBinary::getImagePath() const {
        return binaryPath;
    }

    const cl_program CLBinary::getProgram() const {
        return program;
    }

    unsigned int CLBinary::getNumDeployedDevices() const {
        return numDeployedDevices;
    }

    const scoped_array<cl_device_id> &CLBinary::getDeployedDevices() const {
        return deployedDevices;
    }

    cl_device_id CLBinary::getDeployedDevice(int index) const {
        assert(index < numDeployedDevices && "index must less than deployed device number");
        return deployedDevices[index];
    }

    const scoped_array<cl_command_queue> &CLBinary::getDeviceQueues() const {
        return deviceQueues;
    }

    const cl_command_queue CLBinary::getDeviceQueue(cl_device_id id) const {
        for (unsigned i = 0; i < numDeployedDevices; i++) {
            if (deployedDevices[i] == id) {
                return deviceQueues[i];
            }
        }
        printf("Invalid device id: %p", id);
        return NULL;
    }

    void CLBinary::cleanup() {
        for (int i = 0; i < numDeployedDevices; i++) {
            auto queue = deviceQueues[i];
            if (queue) {
                clReleaseCommandQueue(queue);
            }
        }
        this->deployedDevices.reset((size_t) 0);
        this->numDeployedDevices = 0;
        // release program
        if (program) {
            clReleaseProgram(program);
            program = NULL;
        }
    }

}