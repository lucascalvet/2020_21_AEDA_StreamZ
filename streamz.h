#ifndef STREAMZ_H
#define STREAMZ_H

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "stream.h"
#include "utils.h"

using namespace std;

class StreamZ;

class User {
private:
    static int counter;
    int id;
    Date birthday;
protected:
    bool viewingStream = false;
    string nickname;
public:
    string getName() const;
    int getID() const;
    Date getBirthday() const;
    User(string nickname, Date birthday);
    ~User();
};

class Viewer : public User {
public:
    Viewer(string nickname, Date birthday);
    ~Viewer();
    bool enterStream(Stream* s, StreamZ platform);  //can only be in one stream at the time
    bool exitStream(Stream* s, StreamZ platform);    //they can exit at any time (needs to check if the stream should be added to the top 10)
};

class Streamer : public User {
public:
    Streamer(string nickname, Date birthday);
    Stream* s = NULL;
    ~Streamer();
    vector<Viewer*> total_viewers;
    vector<Viewer*> active_viewers;
    unsigned getTotalViews() const;
    unsigned getActiveViewers() const;
    bool newStream(StreamZ platform, string title, Language lang, unsigned min_age);
    bool endStream(StreamZ platform);  //when stream is endend by streamer, stream is added to history after total viewers data is added to stream
};

class Admin : public User {
    Admin(string nickname, Date birthday);
    ~Admin();
};

class StreamZ {
public:
    unsigned capacity;   //how many streams can be active at same time
    vector<User*> users;
    vector<Stream*> active_streams;
    vector<Stream*> best_streams;
    //map<int, vector<Stream*>> history; ;  //history per id of streamer
    bool addUser(bool streamer, string nickname, Date birthday); // or addStreamer and addViewer?...
    bool activeStream(Streamer &streamer, string title, Language lang, unsigned min_age);
    bool removeActiveStream(const Streamer& streamer);
    bool isStreamActive(const Stream* s, vector<Stream*>::iterator &it);
    unsigned getActiveStreams() const;
    StreamZ(unsigned capacity);
    ~StreamZ();
};

#endif // STREAMZ_H
