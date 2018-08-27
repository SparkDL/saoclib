#include "gtest/gtest.h"
#include "BlasTestEnvironment.h"
#include "level1_test.hpp"
#include "level2_test.hpp"

int main(int argc, char **argv) {
    testing::AddGlobalTestEnvironment(new BlasTestEnvironment());
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




