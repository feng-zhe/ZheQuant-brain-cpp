#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "UT_MovingAverage.h"
#include "UT_Storage.h"

using namespace std;

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
