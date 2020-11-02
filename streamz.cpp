#include "streamz.h"
#include <fstream>

using namespace std;

Age StreamZ::calculateAge(const Date &birthday) {
    time_t current_time;
    time(&current_time);
    tm *date = localtime(&current_time);
    if (date->tm_mon + 1 > birthday.month || date->tm_mon + 1 == birthday.month && date->tm_mday >= birthday.day)
        return date->tm_year - birthday.year;
    return date->tm_year - birthday.year - 1;
}

StreamZ::StreamZ(unsigned capacity) {
    this->capacity = capacity;
}

StreamZ::~StreamZ() {
}

User *StreamZ::addUser(bool streamer, const string &nickname, const Date &birthday) {
    Age age = calculateAge(birthday);
    User* new_user = nullptr;
    if (streamer && age >= MIN_AGE_STREAMER)
        new_user = new Streamer(nickname, birthday);
    if (!streamer && age >= MIN_AGE_VIEWER)
        new_user = new Viewer(nickname, birthday);
    if (new_user != nullptr) users.push_back(new_user);
    return new_user;
}

bool StreamZ::startStream(Streamer &streamer, const string& title, const Language& lang, unsigned min_age) {
    if (streamer.s == nullptr && getActiveStreams() <= capacity) {
        Stream* s = new Stream(title, lang, min_age);
        streamer.s = s;
        active_streams.push_back(s);
        return true;
    } else {
        return false;  //already has a stream
    }
}

bool StreamZ::isStreamActive(const Stream *s, vector<Stream*>::iterator &it) {
    it = find(active_streams.begin(), active_streams.end(), s);
    if (it == active_streams.end())
        return false;
    return true;
}

bool StreamZ::stopStream(const Streamer &streamer) {
    vector<Stream *>::iterator it;
    if (isStreamActive(streamer.s, it)) {
        active_streams.erase(it);
        return true;
    } else
        return false;
}

unsigned StreamZ::getActiveStreams() const {
    return active_streams.size();
}

bool StreamZ::enterStream(Stream *s, Viewer *v) {
    vector<Stream*>::iterator it;
    if (v->viewingStream) {
        cout << "Already viewing a stream!" << endl;
        return false;
    } else if (!isStreamActive(s, it)) {
        cout << "Stream has been deactivated!" << endl;
        return false;
    } else {
        v->s = s;
        v->viewingStream = true;
    }
    return true;
}

bool StreamZ::exitStream(Viewer *v) {
    if (!v->viewingStream) {
        cout << "User is not viewing any stream!" << endl;
        return false;
    }
    v->s = nullptr;
    v->viewingStream = false;
    return true;
}

vector<Stream *> StreamZ::getStreams(const Language &lang = "", Age min_age = UINT_MAX) const {
    vector<Stream *> ret_streams;
    vector<Stream *>::const_iterator stream;
    for (stream = this->active_streams.begin(); stream != this->active_streams.end(); stream++) {
        if (lang.empty() || lang == (*stream)->getLanguage() && min_age >= (*stream)->getMinAge())
            ret_streams.push_back(*stream);
    }
    return ret_streams;
}

bool StreamZ::saveStreams(const string &filename) const{
    ofstream streams_file;
    streams_file.open(filename, ofstream::trunc);
    if (streams_file.fail())
        return false;
    vector<Stream*>::const_iterator stream;
    for (stream = active_streams.begin(); stream != active_streams.end(); stream++){
        streams_file << (*stream)->getTitle() << '\t' << (*stream)->getDate() << '\t' <<
        (*stream)->getLanguage() << '\t' << (*stream)->getMinAge() << '\n';
    }
    return true;
}


