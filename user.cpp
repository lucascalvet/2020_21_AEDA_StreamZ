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

void Viewer::printInfo() {
    cout << "Name: " << nickname << endl;
    cout << "Id: " << to_string(getID()) << endl;
    cout << "Birthday: " << getBirthday() << endl;
    cout << "Viewing: " << to_string(isActive()) << endl;
    if(isActive()) {
        cout << "Stream name: " << s->getTitle() << endl;
    }
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

bool User::isActive() const{
    if(s == NULL)
        return false;
    else
        return true;
}

void Streamer::printInfo(){
    cout << "Name: " << nickname << endl;
    cout << "Id: " << to_string(getID()) << endl;
    cout << "Birthday: " << getBirthday() << endl;
    cout << "Streaming: " << to_string(isActive()) << endl;
    if(isActive()) {
        cout << "Stream name: " << s->getTitle() << endl;
        cout << "Active viewers: " << to_string(getActiveViewers()) << endl;
    }
}

Admin::Admin(string nickname, Date birthday) : User(nickname , birthday){}

Admin::~Admin(){
}
