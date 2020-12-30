#ifndef STREAMZ_H
#define STREAMZ_H

#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <queue>
#include "bst.h"
#include "utils.h"
#include "user.h"
#include "stream.h"
#include "donation.h"
#include "order.h"
#include "product.h"

struct streamerHash {
    int operator()(const Streamer *s) const {
        int hash = 0;
        for (auto c : s->getName()) hash += c;
        return hash;
    }

    bool operator()(const Streamer * s1, const Streamer * s2) const {
        return s1->getID() == s2->getID() && s1->getName() == s2->getName();
    }
};

/**
 * Main class for handling a StreamZ platform
 */
class StreamZ {
private:
    Admin *admin;
    double streamz_capital = 1000000; //Part 2
    static int counter;
    unsigned id;
    ///how many streamers can be active at the same time
    unsigned capacity;
    std::unordered_set<Streamer *, streamerHash, streamerHash> streamers_hash_table;
    std::vector<Streamer *> streamers;
    std::vector<Viewer *> viewers;
    ///A container with the 10 most viewed streams, followed by the 10 most liked streams, sorted in descending order
    std::vector<Stream *> best_streams = std::vector<Stream *>(20, nullptr);

    BST<Donation> donations = BST<Donation>(Donation("", 0, 1));
    unsigned max_orders_per_viewer = 5;  //arbitrated the initial value that can be change in the streamz
    std::priority_queue<Order> orders;
    vector<Product> products;
public:
    StreamZ(unsigned capacity, const std::string &nickname, const Date &birthday, const std::string &password);
    explicit StreamZ(const std::string &filename);
    ~StreamZ();
    unsigned getCapacity() const;
    unsigned getID() const;
    std::vector<Streamer *> getStreamers() const;
    unsigned getNumStreamers() const;
    std::vector<Streamer *> getActiveStreamers() const;
    unsigned getNumActiveStreamers() const;
    std::vector<Viewer *> getViewers() const;
    unsigned getNumViewers() const;
    unsigned getNumStreamViewers(Stream *stream) const;
    User *getUserByName(const std::string &nickname);
    Streamer *getStreamerByID(unsigned user_id) const;
    Streamer *getStreamerByName(const std::string &nickname) const;
    Viewer *getViewerByID(unsigned user_id) const;
    Viewer *getViewerByName(const std::string &nickname) const;
    unsigned getNumCreatedStreams() const;
    unsigned getNumCreatedStreams(const Language &lang) const;
    unsigned getNumCreatedStreams(bool public_streams, const Date &dt1 = Date(0, 0, 0),
                                  const Date &dt2 = Date(31, 12, UINT_MAX)) const;
    std::vector<Streamer *> getStreams(Language lang = "", Age min_age = UINT_MAX) const;
    std::vector<Stream *> getBestStreams() const;
    Streamer *getMostViewedStreamer() const;
    double getAverageViews() const;
    std::string getMostUsedLanguage();
    static void printStreams(const std::vector<Streamer *> &streams) ;
    void addStreamer(const std::string &nickname, const Date &birthday, const std::string &password);
    void addViewer(const std::string &nickname, const Date &birthday, const std::string &password);
    void startPublicStream(Streamer *streamer, const std::string &title, const Language &lang, unsigned min_age) const;
    void startPrivateStream(Streamer *streamer, const std::string &title, const Language &lang, unsigned min_age,
                            const std::vector<unsigned> &authorized_viewers) const;
    void stopStream(Streamer *streamer);
    void stopAllStreams();
    bool loginVerifier(const std::string &nickname,const std::string &password) const;
    void save(const std::string &filename) const;

    void makeDonation(const Streamer* strmr, unsigned amnt, unsigned eval);
    BST<Donation> getDonations() const;
    vector<Donation> getDonations(unsigned int lower, unsigned int upper, unsigned int n = UINT_MAX);
    Order searchOrder(std::string viewer_nickname, unsigned quantity, unsigned priority);
    void makeOrder(Viewer *viewer, unsigned quantity, unsigned priority, unsigned product_id);
    void deleteOrder(Viewer *viewer, unsigned quantity, unsigned priority); //can only make one order with same parameters to know which one to be deleted if wanted
    void changeMaxOrdersPerViewer(unsigned new_limit);
    std::priority_queue<Order> getOrders() const;

    Product getProductById(unsigned id);
    void sellProduct(Streamer* streamer, unsigned price, unsigned stock); //the product is automatic sold to streamz end then the streamz resells at higher price
    void deleteProduct(unsigned id);
    vector<Product> getProducts() const;
    void sendMoney(User *sender, User *receiver, double amount);
    void printAvailableProducts();
    void printStreamers();
    void depositCapitalInStreamz(unsigned value);
    double getStreamzCapital() const;
};

#endif // STREAMZ_H
