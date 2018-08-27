//
// Created by pcz on 18-5-17.
//

#include "CLEnv.h"

namespace saoclib {
    CLEnv::CLEnv() = default;

    CLEnv::~CLEnv() {
        cleanup();
    }

    bool CLEnv::initOpenCL() {
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

    bool CLEnv::verifyDeviceID(cl_device_id id) const {
        for (unsigned i = 0; i < num_devices; i++) {
            if (devices[i] == id) {
                return true;
            }
        }
        return false;
    }

    const std::string &CLEnv::getPlatformName() const {
        return platformName;
    }

    unsigned CLEnv::getNumDevices() const {
        return num_devices;
    }

    const scoped_array<cl_device_id> &CLEnv::getDevices() {
        return devices;
    }

    cl_device_id CLEnv::getDeviceID(unsigned num) const {
        if (num >= num_devices) {
            return NULL;
        }
        return devices[num];
    }

    const cl_platform_id CLEnv::getPlatformId() const {
        return platform;
    }

    cl_context CLEnv::getContext() const {
        return context;
    }

    void CLEnv::cleanup() {
        if (context) {
            clReleaseContext(context);
            context = NULL;
        }
    }


}