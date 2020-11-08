//
// Created by sergi on 01/11/2020.
//

#ifndef STREAMZ_AEDA_2020_21_USER_H
#define STREAMZ_AEDA_2020_21_USER_H

#include "stream.h"

class User {
private:
    static int counter;
    int id;
    Date birthday;
protected:
    string nickname;
public:
    Stream *s = NULL;
    string getName() const;
    int getID() const;
    Date getBirthday() const;
    User(string nickname, Date birthday);
    bool isActive() const;
    ~User();
};

class Viewer : public User {
public:
    bool alreadyLiked = false;
    bool alreadyDisliked = false;
    Viewer(string nickname, Date birthday);
    void printInfo();
    ~Viewer();
};

class Streamer : public User {
public:
    Streamer(string nickname, Date birthday);
    ~Streamer();
    vector<Viewer*> total_viewers;
    vector<Viewer*> active_viewers;
    unsigned getTotalViews() const;
    unsigned getActiveViewers() const;
    void printInfo();
};

class Admin : public User {  //maybe not appropriate because its a framework so the people controling it is the admin
    Admin(string nickname, Date birthday);
    ~Admin();
};


#endif //STREAMZ_AEDA_2020_21_USER_H
