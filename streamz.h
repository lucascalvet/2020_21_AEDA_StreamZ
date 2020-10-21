
#ifndef STREAMZ_H
#define STREAMZ_H
#include <vector>
#include "string"
#include "user.h"

using namespace std;

enum Language{
    PT, EN
};

struct Date {
    unsigned day;
    unsigned month;
    unsigned year;
};

class Stream {
public:
    Stream();
    ~Stream();
    string title;
    Language lang;
    unsigned min_age;
    vector<Viewer*> viewers;
};

class PrivateStream : public Stream{
public:
    PrivateStream();
    ~PrivateStream();
    vector<Viewer*> allowed_viewers;
    unsigned capacity;
};


class StreamZ {
public:
    vector<User*> users;
    vector<Stream*> active_streams;
    vector<Stream*> best_streams;
    Stream* startStream(const Streamer& streamer, string title, Language lang, unsigned min_age);
    bool stopStream(const Streamer& streamer);
};

#endif // STREAMZ_H
