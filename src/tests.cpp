#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "streamz.h"
#include "exceptions.h"

using testing::Eq;


TEST(test, SaveToFiles) {
    StreamZ sz1(20);
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1);
    sz1.addStreamer("sergio", bd2);
    sz1.addViewer("ze", bd3);
    sz1.save("streamz_save_test.txt");
    StreamZ sz2("streamz_save_test.txt");
    EXPECT_EQ(sz2.getCapacity(), 20);
    EXPECT_EQ(sz2.getNumViewers(), 1);
    EXPECT_EQ(sz2.getNumStreamers(), 2);
}