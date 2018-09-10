//
// Created by pcz on 8/27/18.
//

#ifndef SAOCLIB_CPP_ACLBLASPROGRAM_H
#define SAOCLIB_CPP_ACLBLASPROGRAM_H

#include <mutex>
#include "Program.h"
#include "ACLMKLAccelerator.h"

namespace acl {

    // TODO this class must be thread safe
    class ACLResourceManager {
    public:
        ACLResourceManager();

        ~ACLResourceManager();

        const Context *getContext() const;

        const Program *getProgram() const;

        bool allocateAccelerators(std::vector<ACLMKLAccelerator *> &accelerators,
                                  std::string &msg);

    private:
        bool initResource(std::string &msg);

        bool allocateAccelerator(ACLMKLAccelerator **accelerator, std::string &msg);

        std::mutex devicesLock;
        std::mutex initedLock;
        bool inited = false;
        bool initState = false;

        std::vector<cl_device_id> freeDevices;
        std::vector<cl_device_id> busyDevices;

        Context *context = NULL;
        Program *program = NULL;
        std::string binaryPath;
        std::vector<ACLMKLAccelerator *> accelerators;
    };
}


#endif //SAOCLIB_CPP_ACLBLASPROGRAM_H
