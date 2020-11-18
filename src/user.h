#ifndef STREAMZ_USER_H
#define STREAMZ_USER_H

#include "utils.h"
#include "stream.h"

/**
 * Class with all of the attributes and methods of a user
 */
class User {
private:
    static int counter;
    int id;
    Date birthday;
protected:
    std::string nickname;
public:
    Stream *s = nullptr;
    std::string getName() const;
    int getID() const;
    Date getBirthday() const;
    User(std::string nickname, Date birthday);
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
    Viewer(std::string nickname, Date birthday);
    std::string getInfo();
    ~Viewer();
};

/**
 * Class derived from User, to represent a user of type streamer
 */
class Streamer : public User {
public:
    Streamer(std::string nickname, Date birthday);
    ~Streamer();
    std::vector<Viewer*> total_viewers;
    std::vector<Viewer*> active_viewers;
    unsigned getTotalViews() const;
    unsigned getActiveViewers() const;
    std::string getInfo();
};

/**
 * Class derived from User, to represent a user of type admin
 */
class Admin : public User {  //maybe not appropriate because its a framework so the people controling it is the admin
    Admin(std::string nickname, Date birthday);
    ~Admin();
};


#endif //STREAMZ_USER_H
