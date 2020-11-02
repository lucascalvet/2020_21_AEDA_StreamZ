#include "streamz.h"
#include <iostream>

using namespace std;

Age StreamZ::calculateAge(const Date& birthday) {
    time_t current_time;
    time(&current_time);
    tm* date = localtime(&current_time);
    if (date->tm_mon + 1 > birthday.month || date->tm_mon + 1 == birthday.month && date->tm_mday >= birthday.day)
        return date->tm_year - birthday.year;
    return date->tm_year - birthday.year - 1;
}

StreamZ::StreamZ(unsigned capacity) {
    this->capacity = capacity;
}

StreamZ::~StreamZ() {
}

bool StreamZ::addUser(bool streamer, string nickname, Date birthday) {
    Age age = calculateAge(birthday);
    if (streamer && age >= MIN_AGE_STREAMER) {
        users.push_back(new Streamer(nickname, birthday));
        return true;
    }
    if (!streamer && age >= MIN_AGE_VIEWER) {
        users.push_back(new Viewer(nickname, birthday));
        return true;
    }
    return false;
}

bool StreamZ::startStream(Streamer &streamer, string title, Language lang, unsigned min_age){
    if(streamer.s == NULL && getActiveStreams() <= capacity){
        Stream s(title, lang, min_age);
        streamer.s = &s;
        active_streams.push_back(&s);
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

bool StreamZ::stopStream(const Streamer& streamer){
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

bool StreamZ::enterStream(Stream *s, Viewer *v) {
    vector<Stream*>::iterator it;
    if(v->viewingStream) {
        cout << "Already viewing a stream!" << endl;
        return false;
    }
    else if(isStreamActive(s, it)){
        cout << "Stream has been deactivated!" << endl;
        return false;
    }
    else{
        v->viewingStream = true;
    }
    return true;
}

bool StreamZ::exitStream(Stream* s, Viewer *v){
    if(!v->viewingStream)
        cout << "User is not viewing any stream!" << endl;
    else
        v->viewingStream = false;
}
