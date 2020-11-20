#ifndef STREAMZ_USER_H
#define STREAMZ_USER_H

#include "utils.h"
#include "stream.h"

/**
 * Class with all of the attributes and methods of a user
 */
class User {
private:
    std::string password;
    static unsigned counter;
    unsigned id;
    Date birthday;
protected:
    std::string nickname;
public:
    Stream *s = nullptr;
    User(const std::string &nickname, const Date &birthday, const std::string &password);
    User(const std::string &nickname, const Date &birthday, const std::string &password, unsigned id);
    std::string getName() const;
    unsigned getID() const;
    std::string getPassword() const;
    Date getBirthday() const;
    bool isActive() const;
    virtual std::string getInfo();
    ~User();
};

/**
 * Class derived from User, to represent a user of type viewer
 */
class Viewer : public User {
public:
    bool alreadyLiked = false;
    bool alreadyDisliked = false;
    Viewer(const std::string &nickname, const Date &birthday, const std::string &password);
    Viewer(const std::string &nickname, const Date &birthday, const std::string &password, unsigned id);
    std::string getInfo();
    void comment(const std::string &comment);
};

/**
 * Class derived from User, to represent a user of type streamer
 */
class Streamer : public User {
private:
    std::vector<Stream *> streaming_history;
public:
    Streamer(const std::string &nickname, const Date &birthday, const std::string &password);
    Streamer(const std::string &nickname, const Date &birthday, const std::string &password, unsigned id);
    ~Streamer();
    std::string getInfo();
    unsigned getTotalViews() const;
    std::vector<Stream *> getHistory() const;
    void addToHistory(Stream *stream);
    void stopStreaming();
};

/**
 * Class derived from User, to represent a user of type admin
 */
class Admin : public User {  //maybe not appropriate because its a framework so the people controling it is the admin
public:
    Admin(const std::string &nickname, const Date &birthday, const std::string &password);
};


#endif //STREAMZ_USER_H
