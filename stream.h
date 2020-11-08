#ifndef STREAM_H
#define STREAM_H

#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <sstream>
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
    unsigned num_viewers = 0;
    unsigned likes = 0;
    unsigned dislikes = 0;
    vector<unsigned> viewers_liked;
    vector<unsigned> viewers_disliked;
    static string strToUpper(const string& str) ;
public:
    Stream(const string& title, const Language& lang, unsigned min_age);
    class InvalidLanguage{
    public:
        Language lang;
        explicit InvalidLanguage(Language lang) {this->lang = lang;}
    };
    string getTitle() const;
    Date getDate() const;
    Language getLanguage() const;
    Age getMinAge() const;
    void addLike();
    void addDislike();
    void remLike();
    void remDislike();
    string getInfo() const;
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
    vector<unsigned> authorized_viewers;  //authorized_viewers through id (or vector<Users*>?)
    vector<string> comments;
    unsigned capacity;
};

#endif //STREAM_H
