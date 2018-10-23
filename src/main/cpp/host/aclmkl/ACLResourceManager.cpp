//
// Created by pcz on 8/27/18.
//

#include "ACLResourceManager.h"

namespace acl {

    ACLResourceManager::ACLResourceManager() {}

    const Context *ACLResourceManager::getContext() const {
        return context;
    }

    const Program *ACLResourceManager::getProgram() const {
        return program;
    }

    ACLResourceManager::~ACLResourceManager() {
        initedLock.unlock();
        delete program;
        program = NULL;
        delete context;
        context = NULL;
        for (auto &p:accelerators) {
            delete p;
            p = NULL;
        }
    }

    bool ACLResourceManager::allocateAccelerators(std::vector<ACLMKLAccelerator *> &accelerators,
                                                  std::string &msg) {

        if (!is_first_running()) {
            msg = "Another instance of aclmkl is running.";
            return false;
        }

        initedLock.lock();
        if (!inited) {
            initState = initResource(msg);
            inited = true;
        }
        initedLock.unlock();

        if (!initState) {
            log("Init OpenCL failed.%s", msg.c_str());
            return false;
        }

        ACLMKLAccelerator *acc;
        bool isOK = allocateAccelerator(&acc, msg);
        if (!isOK) {
            return false;
        }
        std::string info;
        while (isOK) {
            accelerators.push_back(acc);
            isOK = allocateAccelerator(&acc, info);
        }
        return true;
    }

    bool ACLResourceManager::initResource(std::string &msg) {
        auto chars = std::getenv("ACLMKL_BINARY");
        if (chars == NULL) {
            msg = "Cannot find environment variable 'ACLMKL_BINARY', "
                  "please set this variable with the path of 'aclmkl.aocx'.";
            return false;
        }

        binaryPath = chars;

        try {
            context = new Context;
            context->initOpenCL();
            program = new Program(context, binaryPath);
            auto allDevices = context->getDevices();
            program->deploy(allDevices);
            freeDevices = program->getDeployedDevices();
            return true;
        }
        catch (std::runtime_error &error) {
            msg = error.what();
            return false;
        }
    }

    bool ACLResourceManager::allocateAccelerator(ACLMKLAccelerator **accelerator, std::string &msg) {
        log("*** Allocating Accelerator ***\n");
        log("Free Devices:%d,Busy devices:%d\n\n", freeDevices.size(), busyDevices.size());

        devicesLock.lock();
        if (freeDevices.empty()) {
            devicesLock.unlock();
            msg = "No free devices.";
            return false;
        } else {
            auto device = freeDevices.back();
            busyDevices.push_back(device);
            freeDevices.pop_back();
            devicesLock.unlock();
            auto ret = new ACLMKLAccelerator(this, device);
            accelerators.push_back(ret);
            *accelerator = ret;
            return true;
        }
    }

    bool ACLResourceManager::refreshBenchmarkResultFile() {
        for (auto p:this->accelerators) {
            p->refreshBenchmarkResultFile();
        }
        return true;
    }

}