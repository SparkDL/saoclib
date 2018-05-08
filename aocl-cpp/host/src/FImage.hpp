//
// Created by pcz on 18-4-18.
//

#ifndef AOCL_CPP_FIMAGE_HPP
#define AOCL_CPP_FIMAGE_HPP

#include <cassert>
#include <utility>
#include <iostream>
#include "AOCLUtils/aocl_utils.h"
#include "CL/opencl.h"
#include "common.h"

using namespace aocl_utils;

class FImage {
public:
    FImage(const std::string &image_path)
            : image_name(image_path) {}

    ~FImage() {
        cleanup();
    }

    /*!
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

        // Query the available OpenCL device.
        devices.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));
        printf("Platform: %s\n", getPlatformName(platform).c_str());
        printf("Using %d device(s)\n", num_devices);
        for (unsigned i = 0; i < num_devices; ++i) {
            printf("  %s\n", getDeviceName(devices[i]).c_str());
        }

        // Create the context.
        context = clCreateContext(NULL, num_devices, devices, &oclContextCallback, NULL, &status);
        checkError(status, "Failed to create context");

        // Create the program for all device. Use the first device as the
        // representative device (assuming all device are of the same type).
        std::string binary_file = getBoardBinaryFile(image_name.c_str(), devices[0]);
        printf("Using AOCX: %s\n", binary_file.c_str());
        program = createProgramFromBinary(context, binary_file.c_str(), devices, num_devices);

        // Build the program that was just created.
        status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
        checkError(status, "Failed to build program");

        return true;

    }

    const std::string &getImage_name() const {
        return image_name;
    }

    const cl_platform_id getPlatform() const {
        return platform;
    }

    cl_device_id getDevice(unsigned num) const {
        if (num >= num_devices) {
            return NULL;
        }
        return devices[num];
    }

    unsigned getNumDevices() const {
        return num_devices;
    }

    cl_context getContext() const {
        return context;
    }

    const cl_program getProgram() const {
        return program;
    }

private:
    const std::string image_name;                  /// image name: xxx.aocx
    /* OpenCL runtime resources */
    cl_platform_id platform = NULL;                /// platform id
    scoped_array<cl_device_id> devices;            /// the array storing devices ids
    unsigned num_devices = 0;                      /// the number of devices connected in PCIE slots
    cl_context context = NULL;                     /// opencl context
    cl_program program = NULL;                     /// opencl program

    /*
     * cleanup resources,
     * including kernels, command queues, buffers, program and context
     */
    void cleanup() {
        // release program
        if (program) {
            clReleaseProgram(program);
        }
        // release context
        if (context) {
            clReleaseContext(context);
        }
    }
};


#endif //AOCL_CPP_FIMAGE_HPP
