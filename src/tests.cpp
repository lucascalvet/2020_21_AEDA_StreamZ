#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "streamz.h"
#include "exceptions.h"

using testing::Eq;

TEST(test, SaveToFiles) {
    Date bd0(1, 1, 1990);
    StreamZ sz1(20, "admin", bd0, "admin");
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1, "aa");
    sz1.addStreamer("sergio", bd2, "aa");
    sz1.addViewer("ze", bd3, "aa");
    sz1.save("streamz_save_test.txt");
    StreamZ sz2("streamz_save_test.txt");
    std::cout << std::endl << "TESTING1" << std::endl;
    //EXPECT_EQ(sz2.getCapacity(), 20);
    //EXPECT_EQ(sz2.getNumViewers(), 1);
    //EXPECT_EQ(sz2.getNumStreamers(), 2);
}

TEST(test, StartStopStreams) {
    Date bd0(1, 1, 1990);
    StreamZ sz1(20, "admin", bd0, "admin");
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1, "aa");
    sz1.addStreamer("sergio", bd2, "aa");
    sz1.addViewer("ze", bd3, "aa");
    Streamer *strmr_lucas = sz1.getStreamerByName("lucascs");
    if (strmr_lucas != nullptr) sz1.startPublicStream(strmr_lucas, "A stream do lucas", "PT", 18);
    Viewer *viewer_ze = sz1.getViewerByName("ze");
    if (viewer_ze != nullptr) viewer_ze->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    EXPECT_EQ(viewer_ze->s, nullptr);
}

TEST(test, EnterExitStream) {
    Date bd0(1, 1, 1990);
    StreamZ sz1(20, "admin", bd0, "admin");
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucascs", bd1, "aa");
    sz1.addStreamer("sergio", bd2, "aa");
    sz1.addViewer("ze", bd3, "aa");
    Streamer *strmr_lucas = sz1.getStreamerByName("lucascs");
    Viewer *viewer_ze = sz1.getViewerByName("ze");
    std::vector<unsigned> auth{viewer_ze->getID()};
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    viewer_ze->enterStream(strmr_lucas);
    EXPECT_THROW(viewer_ze->enterStream(strmr_lucas), AlreadyViewing);
    sz1.stopStream(strmr_lucas);
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    viewer_ze->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    auth.clear();
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    EXPECT_THROW(viewer_ze->enterStream(strmr_lucas), UnauthorizedViewer);
}

TEST(test, FileSavingFormat) {
    Date bd0(1, 1, 1990);
    StreamZ sz1(20, "admin", bd0, "admin");
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
    viewer_ze->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 12", "PT", 18);
    viewer_ze->enterStream(strmr_lucas);
    viewer_a->enterStream(strmr_lucas);
    viewer_a->likeStream();
    viewer_b->enterStream(strmr_lucas);
    viewer_b->likeStream();
    viewer_c->enterStream(strmr_lucas);
    viewer_c->likeStream();
    viewer_d->enterStream(strmr_lucas);
    viewer_d->dislikeStream();
    EXPECT_THROW(viewer_d->dislikeStream(), AlreadyInteracted);
    viewer_e->enterStream(strmr_lucas);
    viewer_e->dislikeStream();
    viewer_f->enterStream(strmr_lucas);
    viewer_g->enterStream(strmr_lucas);
    viewer_h->enterStream(strmr_lucas);
    viewer_i->enterStream(strmr_lucas);
    EXPECT_THROW(viewer_i->enterStream(strmr_lucas), AlreadyViewing);
    viewer_i->exitStream();
    viewer_i->enterStream(strmr_lucas);
    viewer_i->exitStream();
    sz1.stopStream(strmr_lucas);
    std::vector<unsigned> auth{viewer_ze->getID()};
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    viewer_ze->enterStream(strmr_lucas);
    viewer_ze->likeStream();
    EXPECT_THROW(viewer_a->likeStream(), InactiveUser);
    viewer_ze->comment("Comentário teste!! :)");
    viewer_ze->comment("Gostei da stream!! Quando é que fazes outra?");
    viewer_ze->comment("xau até logo");
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 3", "PT", 18);
    viewer_ze->enterStream(strmr_lucas);
    viewer_a->enterStream(strmr_lucas);
    viewer_b->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 4", "PT", 18);
    viewer_ze->enterStream(strmr_lucas);
    viewer_a->enterStream(strmr_lucas);
    viewer_b->enterStream(strmr_lucas);
    viewer_c->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    sz1.save("streamz_save_test.txt");
    StreamZ sz2("streamz_save_test.txt");
}

TEST(test, BestStreams) {
    Date bd0(1, 1, 1990);
    StreamZ sz1(20, "admin", bd0, "Administrator<3");
    Date bd1(14, 10, 2000);
    Date bd2(1, 1, 2001);
    Date bd3(28, 11, 2001);
    sz1.addStreamer("lucas", bd1, "Pass123");
    sz1.addStreamer("sergio", bd2, "BestStreamer1");
    sz1.addViewer("ze", bd3, "ZeViewer100%");
    sz1.addViewer("a", bd1, "AaBbCc123");
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
    Streamer *strmr_lucas = sz1.getStreamerByName("lucas");
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
    viewer_ze->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 12", "PT", 18);
    viewer_ze->enterStream(strmr_lucas);
    viewer_a->enterStream(strmr_lucas);
    viewer_a->likeStream();
    viewer_b->enterStream(strmr_lucas);
    viewer_b->likeStream();
    viewer_c->enterStream(strmr_lucas);
    viewer_c->likeStream();
    viewer_d->enterStream(strmr_lucas);
    viewer_d->dislikeStream();
    EXPECT_THROW(viewer_d->dislikeStream(), AlreadyInteracted);
    viewer_e->enterStream(strmr_lucas);
    viewer_e->dislikeStream();
    viewer_f->enterStream(strmr_lucas);
    viewer_g->enterStream(strmr_lucas);
    viewer_h->enterStream(strmr_lucas);
    viewer_i->enterStream(strmr_lucas);
    EXPECT_THROW(viewer_i->enterStream(strmr_lucas), AlreadyViewing);
    viewer_i->exitStream();
    viewer_i->enterStream(strmr_lucas);
    viewer_i->exitStream();
    viewer_i->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    std::vector<unsigned> auth{viewer_ze->getID()};
    sz1.startPrivateStream(strmr_lucas, "A minha stream privada", "PT", 18, auth);
    viewer_ze->enterStream(strmr_lucas);
    viewer_ze->likeStream();
    EXPECT_THROW(viewer_a->likeStream(), InactiveUser);
    viewer_ze->comment("Comentário teste!! :)");
    viewer_ze->comment("Gostei da stream!! Quando é que fazes outra?");
    viewer_ze->comment("xau até logo");
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 3", "PT", 18);
    viewer_ze->enterStream(strmr_lucas);
    viewer_a->enterStream(strmr_lucas);
    viewer_b->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    sz1.startPublicStream(strmr_lucas, "A minha stream 4", "PT", 18);
    viewer_ze->enterStream(strmr_lucas);
    viewer_a->enterStream(strmr_lucas);
    viewer_b->enterStream(strmr_lucas);
    viewer_c->enterStream(strmr_lucas);
    sz1.stopStream(strmr_lucas);
    sz1.save("streamz_save_test.txt");
    StreamZ sz2("streamz_save_test.txt");
    std::cout << "testing";
    EXPECT_EQ(sz2.getBestStreams().at(0)->getTitle(), "A minha stream 12");
    EXPECT_EQ(sz2.getBestStreams().at(1)->getTitle(), "A minha stream 4");
    EXPECT_EQ(sz2.getBestStreams().at(2)->getTitle(), "A minha stream 3");
    EXPECT_EQ(sz2.getBestStreams().at(3)->getTitle(), "A minha stream 1");
    EXPECT_EQ(sz2.getBestStreams().at(4), nullptr);
    EXPECT_EQ(sz2.getBestStreams().at(5), nullptr);
    EXPECT_EQ(sz2.getBestStreams().at(6), nullptr);
    EXPECT_EQ(sz2.getBestStreams().at(7), nullptr);
    EXPECT_EQ(sz2.getBestStreams().at(8), nullptr);
    EXPECT_EQ(sz2.getBestStreams().at(9), nullptr);
    EXPECT_DOUBLE_EQ(sz2.getAverageViews(), 21/5.0);
    EXPECT_EQ(sz2.getNumCreatedStreams(), 5);
}

TEST(test, orders) {
    Date bd0(1, 1, 1990);
    StreamZ sz1(20, "admin", bd0, "Administrator<3");
    Date bd1(14, 10, 2000);
    Date bd3(28, 11, 2001);
    sz1.addViewer("ze", bd3, "ZeViewer100%");
    sz1.addViewer("a", bd1, "AaBbCc123");
    sz1.addViewer("b", bd1, "aa");
    Viewer *viewer_ze = sz1.getViewerByName("ze");
    Viewer *viewer_a = sz1.getViewerByName("a");
    Viewer *viewer_b = sz1.getViewerByName("b");

    sz1.makeOrder(viewer_ze, 2, 2);
    sz1.makeOrder(viewer_ze, 3, 2);
    sz1.makeOrder(viewer_ze, 4, 2);
    sz1.makeOrder(viewer_ze, 2, 1);

    Order empty_order;
    Order ze_order(2, 1, "ze");

    EXPECT_EQ(sz1.getOrders().size(), 4);
    EXPECT_EQ(sz1.searchOrder("ze", 2, 3), empty_order);
    EXPECT_EQ(sz1.searchOrder("ze", 2, 1), ze_order);

    sz1.deleteOrder(viewer_ze, 2, 1);
    EXPECT_EQ(sz1.getOrders().size(), 3);
    EXPECT_THROW(sz1.deleteOrder(viewer_ze, 2, 1);, OrderDoesNotExist);

    sz1.changeMaxOrdersPerViewer(2);
    EXPECT_EQ(sz1.getOrders().size(), 2);

    EXPECT_THROW(Order(2, 6, viewer_a->getName()), InvalidPriority);
    EXPECT_THROW(sz1.makeOrder(viewer_a, 10, 6), ExceededMaxQuantityPerPurchase);
    EXPECT_THROW(sz1.makeOrder(viewer_ze, 2, 2), OrderAlreadyExists);
}