#ifndef STREAMZ_H
#define STREAMZ_H

#include <vector>
#include <string>
#include <map>
#include "utils.h"
#include "user.h"
#include "stream.h"

/**
 * Main class for handling a StreamZ platform
 */
class StreamZ {
private:
    Admin *admin;
    static int counter;
    unsigned id;
    unsigned capacity;   //how many streamers can be active at same time
    std::vector<Streamer *> streamers;
    std::vector<Viewer *> viewers;
    ///A container with the 10 most viewed streams, followed by the 10 most liked streams, sorted in descending order
    std::vector<Stream *> best_streams = std::vector<Stream *>(20, nullptr);
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
    bool loginVerifier(std::string nickname, std::string password) const;
    void save(const std::string &filename) const;
};

#endif // STREAMZ_H
