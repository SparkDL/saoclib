#include <atomic>
#include "gtest/gtest.h"
#include "BlasTestEnvironment.h"
#include "level1_test.hpp"
#include "level2_test.hpp"

void cleanup() {}

BlasTestEnvironment *const testEnv = new BlasTestEnvironment();

#include "AOCLUtils/scoped_ptrs.h"
#include "utils.h"

void buffer_test() {
    std::vector<int, aligned_allocator<int>> a;
    a.push_back(1);
    printf("%d", (long) a.data() % 4096 == 0);
}

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




