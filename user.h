#ifndef STREAMZ_AEDA2020_21_USER_H
#define STREAMZ_AEDA2020_21_USER_H

#include <vector>
#include <string>
#include "stream.h"

using namespace std;

struct Date {
    unsigned day;
    unsigned month;
    unsigned year;
};

class User {
public:
    static int counter;
    int id;
    string name;
    Date birthday;
    unsigned min_age;
    User(string name, Date birthday){
        cout << "HERE";
        id = counter++;
        cout << id << endl;
    }
    //~User();
};

// Initialize static member of class User
int User::counter = 0;

class Viewer : public User {
public:
    Viewer(string name, Date birthday);
    //~Viewer();
    bool enterStream(Stream* s);  //can only be in one stream at the time
    bool exitStream(Stream* s);    //they can exit at any time
};

Viewer::Viewer(string name, Date birthday) : User(name , birthday){
    min_age = 12;
}

class Streamer : public User {
public:
    Streamer(string name, Date birthday);
    Stream* s;
    //~Streamer();
    vector<Viewer*> total_viewers;
    vector<Viewer*> active_viewers;
    unsigned getTotalViews() const;
    unsigned getActiveViewers() const;
    bool endStream();  //when stream is endend by streamer, stream is added to history after total viewers data is added to stream
};

Streamer::Streamer(string name, Date birthday) : User(name, birthday){
    min_age = 15;
}

unsigned Streamer::getTotalViews() const {
    return total_viewers.size();
}

unsigned Streamer::getActiveViewers() const {
    return active_viewers.size();
}


bool Streamer::endStream() {  //removes stream from system and return true is success
}

#endif //STREAMZ_AEDA2020_21_USER_H
