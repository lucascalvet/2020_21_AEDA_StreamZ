#ifndef STREAM_H
#define STREAM_H

#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include "utils.h"

using namespace std;

typedef unsigned Age;
typedef string Language;

class Stream {
private:
    string title;
    Date starting_date;
    static const vector<Language> langs;
    Language lang;
    Age min_age;
    unsigned total_viewers;
    unsigned likes;
    unsigned dislikes;
public:
    Stream(string title, Language lang, unsigned min_age);
    class InvalidLanguage{
    public:
        Language lang;
        InvalidLanguage(Language lang) {this->lang = lang;}
    };
    string getTitle();
    Date getDate();
    Language getLanguage();
    Age getMinAge();
};

class PublicStream : public Stream{
public:
    PublicStream(string title, Language lang, Age min_age);
    ~PublicStream();
};

class PrivateStream : public Stream{
public:
    PrivateStream(string title, Language lang, Age min_age);
    ~PrivateStream();
    vector<unsigned> autorized_viewers;  //autorized_viewers through id
    vector<string> comments;
    unsigned capacity;
};

#endif //STREAM_H
