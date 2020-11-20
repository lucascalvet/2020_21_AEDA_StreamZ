#ifndef STREAMZ_H
#define STREAMZ_H

#include <vector>
#include <string>
#include <map>
#include "utils.h"
#include "user.h"
#include "stream.h"
#include <typeinfo>

/**
 * Main class for handling a StreamZ platform
 */
class StreamZ {
private:
    Admin* admin;
    static int counter;
    unsigned id;
    unsigned capacity;   //how many streamers can be active at same time
    std::vector<Streamer*> streamers;
    std::vector<Viewer*> viewers;
    ///A container with the 10 most viewed streams, followed by the 10 most liked streams, sorted in descending order
    std::vector<Stream*> best_streams = std::vector<Stream*>(20, nullptr);
public:
    StreamZ(unsigned capacity, const std::string &nickname, const Date &birthday, const std::string &password);
    StreamZ(const std::string& filename);
    ~StreamZ();
    unsigned getCapacity() const;
    unsigned getID() const;
    unsigned getNumViewers() const;
    unsigned getNumStreamViewers(Stream* stream) const;
    std::vector<Viewer*> getViewers() const;
    Viewer* getViewerByID(unsigned user_id) const;
    Viewer *getViewerByName(const std::string &nickname) const;
    unsigned getNumStreamers() const;
    unsigned getNumActiveStreamers() const;
    std::vector<Streamer*> getStreamers() const;
    Streamer *getStreamerByID(unsigned user_id) const;
    Streamer *getStreamerByName(const std::string &nickname) const;
    std::vector<Streamer*> getActiveStreamers() const;
    std::vector<Stream*> getBestStreams() const;
    Streamer * getMostViewedStreamer() const;
    unsigned getNumCreatedStreams() const;
    unsigned getNumCreatedStreams(const Language &lang) const;
    unsigned getNumCreatedStreams(bool public_streams, const Date &dt1 = Date(0,0,0), const Date &dt2 = Date(31,12,UINT_MAX)) const;
    double getAverageViews() const;
    //map<int, std::vector<Stream*>> history; ;  //history per id of streamer
    bool addStreamer(const std::string& nickname, const Date& birthday, const std::string& password);
    bool addViewer(const std::string& nickname, const Date& birthday, const std::string& password);
    //when stream is ended by streamer, stream is added to history after total viewers data is added to stream
    // for streamers
    void startPublicStream(Streamer *streamer, const std::string &title, const Language &lang, unsigned min_age) const;
    bool startPrivateStream(Streamer *streamer, const std::string &title, const Language &lang, unsigned min_age, const std::vector<unsigned>& authorized_viewers) const;
    bool stopStream(Streamer* streamer) ;
    //for viewers
    //TODO: Doesn't it make more sense for these static to be part of the viewer class;
    static bool enterStream(Streamer* streamer, Viewer *v) ;  //can only be in one stream at the time
    static bool exitStream(Viewer *v) ;   //they can exit at any time (needs to check if the stream should be added to the top 10)
    static bool likeStream(Viewer * v);
    static bool dislikeStream(Viewer *v);
    static bool remlikeStream(Viewer * v);
    static bool remdislikeStream(Viewer *v);
    void printActiveStreams() const;
    std::vector<Streamer*> getStreams(const Language& lang, Age min_age) const;
    void stopAllStreams();
    bool save(const std::string &filename) const;
    bool loginVerifier(std::string nickname, std::string password) const;
    User* getUserByName(std::string nickname);
};

#endif // STREAMZ_H
