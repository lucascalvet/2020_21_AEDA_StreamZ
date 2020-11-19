#ifndef STREAM_H
#define STREAM_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "utils.h"

/**
 * Class with all of attributes and methods of a stream
 */
class Stream {
private:
    std::string title;
    Date starting_date;
    static const std::vector<Language> LANGS;
    Language lang;
    Age min_age;
    unsigned num_viewers = 0;
    std::vector<unsigned> viewers_liked;
    std::vector<unsigned> viewers_disliked;
public:
    Stream(const std::string& title, const Language& lang, unsigned min_age);
    Stream(const std::string& title, const Language& lang, unsigned min_age, const Date& starting_date, unsigned num_viewers);
    std::string getTitle() const;
    Date getDate() const;
    Language getLanguage() const;
    Age getMinAge() const;
    unsigned getNumViewers() const;
    bool addLike(unsigned id);
    bool addDislike(unsigned id);
    bool remLike(unsigned id);
    bool remDislike(unsigned id);
    bool alreadyLikedOrDisliked(unsigned id);
    std::string getInfo() const;
};

/**
 * Class derived from Stream, to represent a public stream
 */
class PublicStream : public Stream{
public:
    PublicStream(std::string title, Language lang, Age min_age);
    ~PublicStream();
};

/**
 * Class derived from Stream, to represent a private stream, with its additional attributes
 */
class PrivateStream : public Stream{
public:
    PrivateStream(std::string title, Language lang, Age min_age,
                  std::vector<unsigned> authorized_viewers);
    ~PrivateStream();
    std::vector<unsigned> authorized_viewers;  //authorized_viewers through id (or std::vector<Users*>?)
    std::vector<std::string> comments;
};

#endif //STREAM_H
