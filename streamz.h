#ifndef STREAMZ_H
#define STREAMZ_H

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "user.h"

using namespace std;

class StreamZ {
public:
    static int counter;
    unsigned id;
    unsigned capacity;   //how many streamers can be active at same time
    vector<Streamer*> streamers;
    vector<Viewer*> viewers;
    vector<Stream*> best_streams;
    //map<int, vector<Stream*>> history; ;  //history per id of streamer
    bool addStreamer(Streamer* s);
    bool addViewer(Viewer* v);
    //when stream is endend by streamer, stream is added to history after total viewers data is added to stream
    // for streamers
    bool startStream(Streamer *streamer, string title, Language lang, unsigned min_age);
    bool stopStream(Streamer* streamer);
    //for viewers
    bool enterStream(Streamer* streamer, Viewer *v);  //can only be in one stream at the time
    bool exitStream(Viewer *v);   //they can exit at any time (needs to check if the stream should be added to the top 10)
    bool likeStream(Viewer * v);
    bool dislikeStream(Viewer *v);
    bool remlikeStream(Viewer * v);
    bool remdislikeStream(Viewer *v);
    unsigned getStreamers() const;
    unsigned getViewers() const;
    Streamer* getStreamerByID(unsigned id);
    Viewer* getViewerByID(unsigned id);
    unsigned getActiveStreams() const;
    void printActiveStreams();
    StreamZ(unsigned capacity);
    ~StreamZ();
};

void streamz_framework();

#endif // STREAMZ_H
