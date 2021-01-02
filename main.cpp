#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "src/streamz.h"
#include "src/streamz_framework.h"

int main(int argc, char* argv[]){
    streamzFramework();

    //NOTE: In order to run the test comment streamzFramework(); and uncomment google test

    //testing::InitGoogleTest(&argc, argv);
    //std::cout << "\n\n----------STREAMZ TESTS----------" << std::endl;

    //return RUN_ALL_TESTS();
    return 0;
}