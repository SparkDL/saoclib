#include "gtest/gtest.h"
#include "BlasTestEnvironment.h"
#include "level1_test.hpp"
#include "level2_test.hpp"
#include "level3_test.hpp"

void cleanup() {}

BlasTestEnvironment *const testEnv = new BlasTestEnvironment();

int main(int argc, char **argv) {
    testing::AddGlobalTestEnvironment(testEnv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




