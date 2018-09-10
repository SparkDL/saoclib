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
        std::vector<ACLMKLAccelerator *> accs;
        static ACLResourceManager manager;
        std::string reason;
        bool ok = manager.allocateAccelerators(accs, reason);
        if (!ok) {
            printf("\nSetup accelerator for test failed: %s\n", reason.c_str());
            exit(-1);
        }
        acc = accs[0];
    }

    void TearDown() override {
    }

private:

    ACLMKLAccelerator *acc;
};


#endif //SAOCLIB_CPP_BLASTESTENVIRONMENT_H
