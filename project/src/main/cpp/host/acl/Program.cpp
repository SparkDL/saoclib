//
// Created by pcz on 8/28/18.
//

#include <utils/utils.h>
#include "Program.h"

namespace acl {

    Program::Program(Context *context, const std::string &imagePath)
            : context(context),
              binaryPath(imagePath) {}

    Program::~Program() {
        cleanup();
    }

    bool Program::deploy(const std::vector<cl_device_id> &devices) {
        cl_int status;
        int numGivenDevices = devices.size();
        int numContextDevices = context->getNumDevices();

        if (numGivenDevices > numContextDevices) {
            printf("Given devices: %u, Actual devices: %u", numGivenDevices, numContextDevices);
            return false;
        }

        for (unsigned j = 0; j < numGivenDevices; j++) {
            auto given_id = devices[j];
            if (!context->isManagedDevice(given_id)) {
                printf("Invalid device id: %p", given_id);
                return false;
            }
            deployedDevices.push_back(context->getDeviceID(j));
        }

        // Create the program for all device. Use the first device as the
        // representative device (assuming all device are of the same type).
        std::string binary_file = getBoardBinaryFile(binaryPath.c_str(), deployedDevices[0]);
        printf("Using AOCX: %s\n\n", binary_file.c_str());
        program = createProgramFromBinary(context->getRawContext(),
                                          binary_file.c_str(),
                                          deployedDevices.data(),
                                          static_cast<unsigned int>(getNumDeployedDevices()));

        // Build the program that was just created.
        status = clBuildProgram(program,
                                static_cast<cl_uint>(getNumDeployedDevices()),
                                deployedDevices.data(),
                                "",
                                NULL,
                                NULL);
        checkError(status, "Failed to build program");

        return true;
    }

    const Context *Program::getContext() const {
        return context;
    }

    const std::string &Program::getBinaryPath() const {
        return binaryPath;
    }

    const cl_program Program::getProgram() const {
        return program;
    }

    int Program::getNumDeployedDevices() const {
        return static_cast<int>(deployedDevices.size());
    }

    const std::vector<cl_device_id> &Program::getDeployedDevices() const {
        return deployedDevices;
    }

    cl_device_id Program::getDeployedDevice(int index) const {
        assert(index < getNumDeployedDevices() && "index must less than deployed device number");
        return deployedDevices[index];
    }

    void Program::cleanup() {
        this->deployedDevices.resize(0);
        // release program
        if (program) {
            clReleaseProgram(program);
            program = NULL;
        }
    }
}