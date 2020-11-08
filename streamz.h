#ifndef STREAMZ_H
#define STREAMZ_H

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "utils.h"
#include "user.h"
#include "stream.h"

/**
 * Main class for handling a StreamZ platform
 */
class StreamZ {
private:
    static Age calculateAge(const Date& birthday);
public:
    static int counter;
    unsigned id;
    unsigned capacity;   //how many streamers can be active at same time
    std::vector<Streamer*> streamers;
    std::vector<Viewer*> viewers;
    std::vector<Stream*> best_streams;
    //map<int, std::vector<Stream*>> history; ;  //history per id of streamer
    bool addStreamer(const std::string& nickname, const Date& birthday);
    bool addViewer(const std::string& nickname, const Date& birthday);
    //when stream is endend by streamer, stream is added to history after total viewers data is added to stream
    // for streamers
    bool startStream(Streamer *streamer, std::string title, Language lang, unsigned min_age);
    bool stopStream(Streamer* streamer);
    //for viewers
    bool enterStream(Streamer* streamer, Viewer *v);  //can only be in one stream at the time
    bool exitStream(Viewer *v);   //they can exit at any time (needs to check if the stream should be added to the top 10)
    static bool likeStream(Viewer * v);
    static bool dislikeStream(Viewer *v);
    static bool remlikeStream(Viewer * v);
    static bool remdislikeStream(Viewer *v);
    unsigned getStreamers() const;
    unsigned getViewers() const;
    Streamer* getStreamerByID(unsigned id);
    Viewer* getViewerByID(unsigned id);
    unsigned getActiveStreams() const;
    void printActiveStreams();
    StreamZ(unsigned capacity);
    ~StreamZ();
    std::vector<Stream*> getStreams(const Language& lang, Age min_age) const;
    bool saveStreams(const std::string& filename) const;
};

void streamz_framework();

#endif // STREAMZ_H
