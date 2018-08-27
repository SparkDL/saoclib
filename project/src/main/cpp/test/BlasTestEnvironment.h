//
// Created by pcz on 8/25/18.
//

#ifndef SAOCLIB_CPP_BLASTESTENVIRONMENT_H
#define SAOCLIB_CPP_BLASTESTENVIRONMENT_H

#include "aclblas.h"
#include "gtest/gtest.h"

class BlasTestEnvironment :
        public testing::Environment {
public:

    void SetUp() override {
        aclblas::requestAccelerator();
    }

    void TearDown() override {
    }
};


#endif //SAOCLIB_CPP_BLASTESTENVIRONMENT_H
