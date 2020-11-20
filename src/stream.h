#ifndef STREAMZ_STREAM_H
#define STREAMZ_STREAM_H

#include <vector>
#include <string>
#include "utils.h"

/**
 * Class with all of attributes and methods of a stream
 */
class Stream {
private:
    std::string title;
    Date starting_date;
    static const std::vector<Language> LANGS;
    ///Total number of times that any viewer enters the stream
    unsigned num_total_views = 0;
    Language lang;
    Age min_age;
    std::vector<unsigned> viewers_liked;
    std::vector<unsigned> viewers_disliked;
public:
    Stream(const std::string& title, const Language& lang, unsigned min_age);
    Stream(const std::string& title, const Language& lang, unsigned min_age, const Date& starting_date, unsigned num_viewers);
    virtual std::string getInfo() const;
    std::string getTitle() const;
    Date getDate() const;
    Language getLanguage() const;
    Age getMinAge() const;
    unsigned int getNumTotalViews() const;
    unsigned getNumLikes() const;
    unsigned getNumDislikes() const;
    void addView();
    bool addLike(unsigned id);
    bool addDislike(unsigned id);
    bool remLike(unsigned id);
    bool remDislike(unsigned id);
    bool alreadyLikedOrDisliked(unsigned id);
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
private:
    std::vector<unsigned> authorized_viewers;  //authorized_viewers through id (or std::vector<Users*>?)
    std::vector<std::string> comments; //TODO: save the viewer that commented? implement comments
public:
    PrivateStream(std::string title, Language lang, Age min_age,
                  std::vector<unsigned> authorized_viewers);
    ~PrivateStream();
    std::string getInfo() const;
    const std::vector<unsigned int> &getAuthorizedViewers() const;
    const std::vector<std::string> &getComments() const;
    bool isAuthorized(unsigned user_id) const;
    void addComment(const std::string &comment);
};

#endif //STREAMZ_STREAM_H
