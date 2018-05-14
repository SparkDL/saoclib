//
// Created by pcz on 18-4-18.
//

#ifndef SAOCLIB_CPP_CLENV_HPP
#define SAOCLIB_CPP_CLENV_HPP

#include "AOCLUtils/aocl_utils.h"

namespace saoclib {
    using namespace aocl_utils;

    // TODO CheckError with cleanup
    class ClEnv {
    public:
        ClEnv() = default;

        ~ClEnv() {
            cleanup();
        }

        /**
         * Init OpenCL resources,
         * including platform, device, context and program
         * @return
         */
        bool init_opencl() {
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

        bool verifyDevice(cl_device_id id) const {
            for (unsigned i = 0; i < num_devices; i++) {
                if (devices[i] == id) {
                    return true;
                }
            }
            return false;
        }

        const std::string &getPlatformName() const {
            return platformName;
        }

        unsigned getNumDevices() const {
            return num_devices;
        }

        const scoped_array<cl_device_id> & getDevices(){
            return devices;
        }

        cl_device_id getDeviceId(unsigned num) const {
            if (num >= num_devices) {
                return NULL;
            }
            return devices[num];
        }

        const cl_platform_id getPlatformId() const {
            return platform;
        }

        cl_context getContext() const {
            return context;
        }

        void cleanup() {
            if (context) {
                clReleaseContext(context);
            }
        }

    private:
        std::string platformName = "";
        cl_platform_id platform = NULL;                /// platform id
        scoped_array<cl_device_id> devices;            /// the array storing devices ids
        cl_context context = NULL;                     /// opencl context
        unsigned num_devices = 0;                      /// the number of devices connected in PCIE slots
    };
}
#endif //SAOCLIB_CPP_CLENV_HPP
