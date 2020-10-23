
#ifndef STREAMZ_H
#define STREAMZ_H
#include <vector>
#include "string"
#include "user.h"

using namespace std;

enum Language{
    PT, EN, IT
};

struct Date {
    unsigned day;
    unsigned month;
    unsigned year;
};

class Stream;

class User {
public:
    string name;
    Date birthday;
    Stream *stream;  //or the stream beeing viewed or beeing streammed
    unsigned min_age;
    User(string name, Date birthday);
    ~User();
};

class Streamer : public User {
    Streamer();
    ~Streamer();
    vector<Stream> history;
    bool endStream();
};

Streamer::Streamer() : User(name, birthday){
    min_age = 15;
}

bool Streamer::endStream() {  //removes stream from system and return true is success
}

class Viewer : public User {
    Viewer();
    ~Viewer();
    bool enterStream(Stream *stream);  //can only be in one stream at the time
    bool exitStream(Stream *stream);  //they can exit at any time
};

Viewer::Viewer() : User(name , birthday){
    min_age = 12;
}

class Stream {
public:
    Stream();
    ~Stream();
    string title;
    Language lang;
    unsigned min_age;
    vector<Viewer*> total_viewers;
    vector<Viewer*> active_viewers;
    unsigned getTotalViews() const;
    unsigned getActiveViewers() const;
};

Stream::Stream() {
}

Stream::~Stream() {
}

unsigned Stream::getTotalViews() const {
    return total_viewers.size();
}

unsigned Stream::getActiveViewers() const {
    return active_viewers.size();
}

class PublicStream : public Stream{
public:
    PublicStream();
    ~PublicStream();
};

class PrivateStream : public Stream{
public:
    PrivateStream();
    ~PrivateStream();
    vector<Viewer*> autorized_viewers;
    vector<string> comments;
    unsigned capacity;
};

class StreamZ {
public:
    vector<User*> users;
    vector<Stream*> active_streams;
    vector<Stream*> best_streams;
    Stream* createStream(const Streamer& streamer, string title, Language lang, unsigned min_age);
    bool removeStream(const Streamer& streamer);
    StreamZ();
    ~StreamZ();
};

StreamZ::StreamZ() {
}

#endif // STREAMZ_H
