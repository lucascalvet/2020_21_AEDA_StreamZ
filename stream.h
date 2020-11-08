#ifndef STREAM_H
#define STREAM_H

#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <sstream>
#include "utils.h"

/// Unsigned type for representing an age
typedef unsigned Age;
/// String type for representing a language, in the predefined format
typedef std::string Language;

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
    unsigned likes = 0;
    unsigned dislikes = 0;
    std::vector<unsigned> viewers_liked;
    std::vector<unsigned> viewers_disliked;
public:
    Stream(const std::string& title, const Language& lang, unsigned min_age);
    class InvalidLanguage{
    public:
        Language lang;
        explicit InvalidLanguage(Language lang) {this->lang = lang;}
    };
    std::string getTitle() const;
    Date getDate() const;
    Language getLanguage() const;
    Age getMinAge() const;
    void addLike();
    void addDislike();
    void remLike();
    void remDislike();
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
    PrivateStream(std::string title, Language lang, Age min_age);
    ~PrivateStream();
    std::vector<unsigned> authorized_viewers;  //authorized_viewers through id (or std::vector<Users*>?)
    std::vector<std::string> comments;
    unsigned capacity;
};

#endif //STREAM_H
