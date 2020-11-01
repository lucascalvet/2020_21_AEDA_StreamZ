//
// Created by sergi on 01/11/2020.
//

//Static member to have unique id's to each user
#include "user.h"

int User::counter = 0;

User::User(string nickname, Date birthday){
    this->nickname = nickname;
    this->birthday = birthday;
    id = counter++;
}

User::~User(){
}

string User::getName() const {
    return nickname;
}

int User::getID() const {
    return id;
}

Date User::getBirthday() const {
    return birthday;
}

Viewer::Viewer(string nickname, Date birthday) : User(nickname , birthday){
}

Viewer::~Viewer() {
}

Streamer::Streamer(string nickname, Date birthday) : User(nickname, birthday){
}

Streamer::~Streamer() {
}

unsigned Streamer::getTotalViews() const {
    return total_viewers.size();
}

unsigned Streamer::getActiveViewers() const {
    return active_viewers.size();
}

Admin::Admin(string nickname, Date birthday) : User(nickname , birthday){}

Admin::~Admin(){
}
