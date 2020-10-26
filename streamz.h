
#ifndef STREAMZ_H
#define STREAMZ_H
#include <vector>
#include <string>
#include <map>
#include "user.h"
#include "stream.h"

using namespace std;

class StreamZ {
public:
    vector<User*> users;
    vector<Stream*> active_streams;
    vector<Stream*> best_streams;
   // map<int, vector<Stream*>> history; ;  //history per id of streamer
    Stream* createStream(const Streamer& streamer, string title, Language lang, unsigned min_age);
    bool removeStream(const Streamer& streamer);
    StreamZ();
    ~StreamZ();
};

StreamZ::StreamZ() {
}

#endif // STREAMZ_H
