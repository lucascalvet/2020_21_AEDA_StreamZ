#include "streamz.h"
#include <iostream>

using namespace std;

//Static member to have unique id's to each user
int User::counter = 0;

User::User(string name, Date birthday){
    this->name = name;
    this->birthday = birthday;
    id = counter++;
}

User::~User(){
}

Viewer::Viewer(string name, Date birthday) : User(name , birthday){
    min_age = 12;
}

Viewer::~Viewer() {
}

Streamer::Streamer(string name, Date birthday) : User(name, birthday){
    min_age = 15;
}

Streamer::~Streamer() {
}

unsigned Streamer::getTotalViews() const {
    return total_viewers.size();
}

unsigned Streamer::getActiveViewers() const {
    return active_viewers.size();
}

StreamZ::StreamZ(unsigned capacity) {
    this->capacity = capacity;
}

StreamZ::~StreamZ() {
}

bool StreamZ::activeStream(Streamer &streamer, string title, Language lang, unsigned min_age){
    if(streamer.s == NULL && getActiveStreams() <= capacity){
        Stream s(title, lang, min_age);
        streamer.s = &s;
        return true;
    }
    else{
        return false;  //already has a stream
    }
}

bool StreamZ::isStreamActive(const Stream* s, vector<Stream*>::iterator &it) {
    it = find(active_streams.begin(), active_streams.end(), s);
    if (it != active_streams.end())
    {
        return true;
    }
    else
        return false;
}

bool StreamZ::removeActiveStream(const Streamer& streamer){
    vector<Stream*>::iterator it;
    if (isStreamActive(streamer.s, it))
    {
        active_streams.erase(it);
        return true;
    }
    else
        return false;
}

unsigned StreamZ::getActiveStreams() const {
    return active_streams.size();
}

bool Streamer::endStream(StreamZ platform) {  //removes stream from system and return true is success
    if (platform.removeActiveStream(*this)) {
        cout << this->name << " ended the stream " << this->s->getTitle() << endl;
        return true;
    } else {
        cout << "Stream already endend!" << endl;
        return false;
    }
}

bool Streamer::newStream(StreamZ platform, string title, Language lang, unsigned min_age){
    if(s != NULL)
        return false;
    else{
        if(platform.activeStream(*this, title, lang, min_age))
            return true;
        else
            return false;
    }
}

bool Viewer::enterStream(Stream *s, StreamZ platform) {
    vector<Stream*>::iterator it;
    if(viweingStream) {
        cout << "Already viewing a stream!" << endl;
        return false;
    }
    else if(!platform.isStreamActive(s, it)){
        cout << "Stream has been deactivated!" << endl;
        return false;
    }
    else{

    }
    return true;
}
