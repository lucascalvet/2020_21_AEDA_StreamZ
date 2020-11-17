#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "src/streamz.h"
#include "src/streamz_framework.h"

int main(){
    streamz_framework();
    testing::InitGoogleTest(&argc, argv);
    std::cout << "AEDA 2020/2021 - Practical 6" << std::endl;
    return RUN_ALL_TESTS();
}
