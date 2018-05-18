//
// Created by pcz on 18-5-17.
//

#include "ClEnv.h"

namespace saoclib {
    ClEnv::ClEnv() = default;

    ClEnv::~ClEnv() {
        cleanup();
    }

    bool ClEnv::initOpenCL() {
        cl_int status;

        printf("Initializing OpenCL\n");

        // Get the OpenCL platform.
        platform = findPlatform("Intel");
        if (platform == NULL) {
            printf("ERROR: Unable to find Intel FPGA OpenCL platform.\n");
            return false;
        }

        platformName = aocl_utils::getPlatformName(platform);

        // Query the available OpenCL device.
        devices.reset(aocl_utils::getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));
        printf("Platform: %s\n", platformName.c_str());
        printf("Using %d device(s)\n", num_devices);
        for (unsigned i = 0; i < num_devices; ++i) {
            printf("  %s\n", getDeviceName(devices[i]).c_str());
        }

        // Create the context.
        context = clCreateContext(NULL, num_devices, devices, &oclContextCallback, NULL, &status);
        checkError(status, "Failed to create context");
        return true;
    }

    bool ClEnv::verifyDeviceID(cl_device_id id) const {
        for (unsigned i = 0; i < num_devices; i++) {
            if (devices[i] == id) {
                return true;
            }
        }
        return false;
    }

    const std::string &ClEnv::getPlatformName() const {
        return platformName;
    }

    unsigned ClEnv::getNumDevices() const {
        return num_devices;
    }

    const scoped_array<cl_device_id> &ClEnv::getDevices() {
        return devices;
    }

    cl_device_id ClEnv::getDeviceID(unsigned num) const {
        if (num >= num_devices) {
            return NULL;
        }
        return devices[num];
    }

    const cl_platform_id ClEnv::getPlatformId() const {
        return platform;
    }

    cl_context ClEnv::getContext() const {
        return context;
    }

    void ClEnv::cleanup() {
        if (context) {
            clReleaseContext(context);
        }
    }


}