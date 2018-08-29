//
// Created by pcz on 18-5-17.
//

#include "CLContext.h"

namespace saoclib {
    CLContext::CLContext() {

    };

    CLContext::~CLContext() {
        cleanup();
    }

    bool CLContext::initOpenCL() {
        cl_int status;

        printf("Initializing OpenCL\n");

        // Get the OpenCL platform.
        platformID = findPlatform("Intel");
        if (platformID == NULL) {
            printf("ERROR: Unable to find Intel FPGA OpenCL platform.\n");
            return false;
        }
        platformName = aocl_utils::getPlatformName(platformID);

        // Query available OpenCL devices.
        unsigned numDevices;
        auto *all_devices = aocl_utils::getDevices(platformID, CL_DEVICE_TYPE_ALL, &numDevices);
        printf("Platform: %s\n", platformName.c_str());
        printf("Using %d device(s)\n", numDevices);
        for (unsigned i = 0; i < numDevices; ++i) {
            devices.push_back(all_devices[i]);
            printf("  %s\n", getDeviceName(devices[i]).c_str());
        }

        // Create an OpenCL context.
        context = clCreateContext(NULL, numDevices, devices.data(), &oclContextCallback, NULL, &status);
        checkError(status, "Failed to create context");
        return true;
    }
    bool CLContext::isManagedDevice(cl_device_id id) const {
        for (unsigned i = 0; i < getNumDevices(); i++) {
            if (devices[i] == id) {
                return true;
            }
        }
        return false;
    }

    const std::string &CLContext::getPlatformName() const {
        return platformName;
    }

    int CLContext::getNumDevices() const {
        return devices.size();
    }

    const std::vector<cl_device_id> &CLContext::getDevices() const {
        return devices;
    }

    cl_device_id CLContext::getDeviceID(unsigned i) const {
        if (i >= getNumDevices()) {
            return NULL;
        }
        return devices[i];
    }

    const cl_platform_id CLContext::getPlatformID() const {
        return platformID;
    }

    cl_context CLContext::getRawContext() const {
        return context;
    }

    void CLContext::cleanup() {
        if (context) {
            clReleaseContext(context);
            context = NULL;
        }
    }


}