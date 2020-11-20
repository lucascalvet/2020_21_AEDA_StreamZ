#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "streamz.h"
#include "exceptions.h"

using testing::Eq;

TEST(test, SaveToFiles) {
    Date bd0(1,1,1990);
    Admin* adm = new Admin("admin", bd0, "admin");
    StreamZ sz1(20, adm);
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1, "aa");
    sz1.addStreamer("sergio", bd2, "aa");
    sz1.addViewer("ze", bd3, "aa");
    sz1.save("streamz_save_test.txt");
    StreamZ sz2("streamz_save_test.txt");
    EXPECT_EQ(sz2.getCapacity(), 20);
    EXPECT_EQ(sz2.getNumViewers(), 1);
    EXPECT_EQ(sz2.getNumStreamers(), 2);
}

TEST(test, StartStopStreams) {
    Date bd0(1,1,1990);
    Admin* adm = new Admin("admin", bd0, "admin");
    StreamZ sz1(20, adm);
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1, "aa");
    sz1.addStreamer("sergio", bd2, "aa");
    sz1.addViewer("ze", bd3, "aa");
    Streamer* strmr_lucas = sz1.getStreamerByName("lucascs");
    if(strmr_lucas != nullptr) sz1.startPublicStream(strmr_lucas, "A stream do lucas", "PT", 18);
    Viewer* viewer_ze = sz1.getViewerByName("ze");
    if(viewer_ze != nullptr) sz1.enterStream(strmr_lucas, viewer_ze);
    sz1.stopStream(strmr_lucas);
    EXPECT_EQ(viewer_ze->s, nullptr);
}

TEST(test, EnterExitStream) {
    Date bd0(1,1,1990);
    Admin* adm = new Admin("admin", bd0, "admin");
    StreamZ sz1(20, adm);
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1, "aa");
    sz1.addStreamer("sergio", bd2, "aa");
    sz1.addViewer("ze", bd3, "aa");
    Streamer* strmr_lucas = sz1.getStreamerByName("lucascs");
    Viewer* viewer_ze = sz1.getViewerByName("ze");
    std::vector<unsigned> auth {viewer_ze->getID()};
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    EXPECT_EQ(true, sz1.enterStream(strmr_lucas, viewer_ze));
    EXPECT_EQ(false, sz1.enterStream(strmr_lucas, viewer_ze));
    sz1.stopStream(strmr_lucas);
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    EXPECT_EQ(true, sz1.enterStream(strmr_lucas, viewer_ze));
    sz1.stopStream(strmr_lucas);
    auth.clear();
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    EXPECT_EQ(false, sz1.enterStream(strmr_lucas, viewer_ze));
}

TEST(test, BestStreams) {
    EXPECT_EQ(true, true);

    Date bd0(1, 1, 1990);
    Admin *adm = new Admin("admin", bd0, "admin");
    StreamZ sz1(20, adm);
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1, "aa");
    sz1.addStreamer("sergio", bd2, "aa");
    sz1.addViewer("ze", bd3, "aa");
    sz1.addViewer("a", bd1, "aa");
    sz1.addViewer("b", bd1, "aa");
    sz1.addViewer("c", bd1, "aa");
    sz1.addViewer("d", bd1, "aa");
    sz1.addViewer("e", bd1, "aa");
    sz1.addViewer("f", bd1, "aa");
    sz1.addViewer("g", bd1, "aa");
    sz1.addViewer("h", bd1, "aa");
    sz1.addViewer("i", bd1, "aa");
    sz1.addViewer("j", bd1, "aa");
    sz1.addViewer("k", bd1, "aa");
    Streamer *strmr_lucas = sz1.getStreamerByName("lucascs");
    Viewer *viewer_ze = sz1.getViewerByName("ze");
    Viewer *viewer_a = sz1.getViewerByName("a");
    Viewer *viewer_b = sz1.getViewerByName("b");
    Viewer *viewer_c = sz1.getViewerByName("c");
    Viewer *viewer_d = sz1.getViewerByName("d");
    Viewer *viewer_e = sz1.getViewerByName("e");
    Viewer *viewer_f = sz1.getViewerByName("f");
    Viewer *viewer_g = sz1.getViewerByName("g");
    Viewer *viewer_h = sz1.getViewerByName("h");
    Viewer *viewer_i = sz1.getViewerByName("i");
    Viewer *viewer_j = sz1.getViewerByName("j");
    sz1.startPublicStream(strmr_lucas, "A minha stream 1", "PT", 18);
    sz1.enterStream(strmr_lucas, viewer_ze);
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 12", "PT", 18);
    sz1.enterStream(strmr_lucas, viewer_ze);
    sz1.enterStream(strmr_lucas, viewer_a);
    sz1.enterStream(strmr_lucas, viewer_b);
    sz1.enterStream(strmr_lucas, viewer_c);
    sz1.enterStream(strmr_lucas, viewer_d);
    sz1.enterStream(strmr_lucas, viewer_e);
    sz1.enterStream(strmr_lucas, viewer_f);
    sz1.enterStream(strmr_lucas, viewer_g);
    sz1.enterStream(strmr_lucas, viewer_h);
    sz1.enterStream(strmr_lucas, viewer_i);
    sz1.exitStream(viewer_i);
    sz1.enterStream(strmr_lucas, viewer_i);
    sz1.exitStream(viewer_i);
    sz1.enterStream(strmr_lucas, viewer_i);
    sz1.exitStream(viewer_i);
    EXPECT_EQ(strmr_lucas->s->getNumTotalViews(), 12);
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 3", "PT", 18);
    sz1.enterStream(strmr_lucas, viewer_ze);
    sz1.enterStream(strmr_lucas, viewer_a);
    sz1.enterStream(strmr_lucas, viewer_b);
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 4", "PT", 18);
    sz1.enterStream(strmr_lucas, viewer_ze);
    sz1.enterStream(strmr_lucas, viewer_a);
    sz1.enterStream(strmr_lucas, viewer_b);
    sz1.enterStream(strmr_lucas, viewer_c);
    sz1.stopStream(strmr_lucas);
    EXPECT_EQ(sz1.getBestStreams().at(0)->getTitle(), "A minha stream 12");
    EXPECT_EQ(sz1.getBestStreams().at(1)->getTitle(), "A minha stream 4");
    EXPECT_EQ(sz1.getBestStreams().at(2)->getTitle(), "A minha stream 3");
    EXPECT_EQ(sz1.getBestStreams().at(3)->getTitle(), "A minha stream 1");
    EXPECT_EQ(sz1.getBestStreams().at(4), nullptr);
    EXPECT_EQ(sz1.getBestStreams().at(5), nullptr);
    EXPECT_EQ(sz1.getBestStreams().at(6), nullptr);
    EXPECT_EQ(sz1.getBestStreams().at(7), nullptr);
    EXPECT_EQ(sz1.getBestStreams().at(8), nullptr);
    EXPECT_EQ(sz1.getBestStreams().at(9), nullptr);
    EXPECT_EQ(sz1.getAverageViews(), 5.0);
    EXPECT_EQ(sz1.getNumCreatedStreams(), 4);
}