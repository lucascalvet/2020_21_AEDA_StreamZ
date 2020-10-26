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
    Stream(string title, Language lang);
    ~Stream();
    string title;
    Language lang;
    unsigned min_age;
    unsigned total_viewers;
    unsigned likes;
    unsigned dislikes;
};

class PublicStream : public Stream{
public:
    PublicStream(string title, Language lang);
    ~PublicStream();
};

class PrivateStream : public Stream{
public:
    PrivateStream(string title, Language lang);
    ~PrivateStream();
    vector<unsigned> autorized_viewers;  //autorized_viewers through id
    vector<string> comments;
    unsigned capacity;
};

#endif //STREAMZ_AEDA2020_21_STREAM_H
