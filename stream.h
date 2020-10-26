#ifndef STREAMZ_AEDA2020_21_STREAM_H
#define STREAMZ_AEDA2020_21_STREAM_H
#include <vector>
#include <string>

using namespace std;

enum Language{
    PT, EN, IT
};

class Stream {
public:
    Stream();
    ~Stream();
    string title;
    Language lang;
    unsigned min_age;
    unsigned total_viewers;
};

Stream::Stream() {
}

Stream::~Stream() {
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
    vector<unsigned > autorized_viewers;  //autorized_viewers through id
    vector<string> comments;
    unsigned capacity;
};

#endif //STREAMZ_AEDA2020_21_STREAM_H
