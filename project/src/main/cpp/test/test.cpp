#include "gtest/gtest.h"
#include "BlasTestEnvironment.h"
#include "level1_test.hpp"
#include "level2_test.hpp"

void cleanup() {}

BlasTestEnvironment *const testEnv = new BlasTestEnvironment();

#include "AOCLUtils/scoped_ptrs.h"
#include "utils.h"

void test() {
    const int n = 10000;
    const int loop = 1000;
    float src[n] = {0,};
    executeTime(
            [src]() -> void {
                for (int i = 0; i < loop; i++) {
                    auto des = (float *) alignedMalloc(n * sizeof(float));
                    memcpy(des, src, n * sizeof(float));
                }
            },
            "test"
    );
}

int main(int argc, char **argv) {
    testing::AddGlobalTestEnvironment(testEnv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




