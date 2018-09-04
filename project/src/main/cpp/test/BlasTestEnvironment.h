//
// Created by pcz on 8/25/18.
//

#ifndef SAOCLIB_CPP_BLASTESTENVIRONMENT_H
#define SAOCLIB_CPP_BLASTESTENVIRONMENT_H

#include "ACLResourceManager.h"
#include "gtest/gtest.h"

using namespace acl;

class BlasTestEnvironment :
        public testing::Environment {
public:

    acl::ACLMKLAccelerator *getAccelerator() {
        return acc;
    }

    void SetUp() override {
        static const char *binaryPath = "/home/pcz/develop/saoclib/project/target/aclmkl";
        static ACLResourceManager manager(binaryPath);
        std::string reason;
        bool ok = manager.allocateAccelerator(&acc, reason);
        if (!ok) {
            printf("\nSetup accelerator for test failed: %s\n", reason.c_str());
            exit(-1);
        }
    }

    void TearDown() override {
    }

private:

    ACLMKLAccelerator *acc;
};


#endif //SAOCLIB_CPP_BLASTESTENVIRONMENT_H
