#include "streamz.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "menu.h"

using namespace std;

int StreamZ::counter = 0;

/**
 * Calculate the age of someone born on "birthday", relative to the current day
 * @param birthday the person's date of birth
 * @return the age of the person
 */
Age StreamZ::calculateAge(const Date &birthday) {
    time_t current_time;
    time(&current_time);
    tm *date = localtime(&current_time);
    if (date->tm_mon + 1 > birthday.month || date->tm_mon + 1 == birthday.month && date->tm_mday >= birthday.day)
        return date->tm_year - birthday.year;
    return date->tm_year - birthday.year - 1;
}

/**
 * Constructor for the StreamZ class
 *
 * @param capacity the maximum capacity of streams the platform may have at any time
 */
StreamZ::StreamZ(unsigned capacity) {
    id = counter++;
    this->capacity = capacity;
}

StreamZ::~StreamZ() {
}

/**
 * Starts a stream, with all of its initial properties
 *
 * Checks if the streamer already has an active stream and if the StreamZ platform hasn't reached
 * its maximum capacity of streams. If so, it starts the stream.
 *
 * @param streamer the streamer starting the stream
 * @param title the stream's title
 * @param lang the stream's language
 * @param min_age the stream's minimum age
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::startStream(Streamer *streamer, string title, Language lang, unsigned min_age){
    if(!streamer->isActive() && getActiveStreams() <= capacity){
        Stream s(title, lang, min_age);
        streamer->s = &s;
        return true;
    } else {
        return false;  //already has a stream
    }
}

/**
 * Stops a streamer's stream. Checks if the streamer has indeed an active stream
 *
 * @param streamer the streamer that stopped its stream
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::stopStream(Streamer* streamer){
    if (streamer->isActive()){
        streamer->s = NULL;
        return true;
    } else
        return false;
}

/**
 * Gets the number of active streams
 *
 * @return the number of active streams
 */
unsigned StreamZ::getActiveStreams() const {
    unsigned counter = 0;
    for(int i = 0; i < streamers.size(); i++){
        if(streamers.at(i)->s != NULL)
            counter++;
    }
    return counter;
}

/**
 * Enters a certain viewer into a certain streamer's stream
 *
 * Checks if the viewer is not already in a stream and if the streamer has an active stream.
 * If so, enters the viewer into the streamer's stream.
 *
 * @param streamer the streamer streaming the stream in which the viewer wants to enter
 * @param v the viewer that wants to enter the stream
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::enterStream(Streamer *streamer, Viewer *v) {
    vector<Stream*>::iterator it;
    if(v->isActive()) {
        cout << "Already viewing a stream!" << endl;
        return false;
    }
    else if(!streamer->isActive()){
        cout << "Stream has been deactivated!" << endl;
        return false;
    }
    else{
        v->s = streamer->s;
    }
    return true;
}

/**
 * Removes a viewer from the stream he's watching
 *
 * Checks if the viewer is indeed watching a stream. If so, it removes him from the stream.
 *
 * @param v the viewer to remove from the stream
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::exitStream(Viewer *v){
    if(!v->isActive()) {
        cout << "User is not viewing any stream!" << endl;
        return false;
    }
    v->s = NULL;  //exiting stream
    v->alreadyLiked = false;
    v->alreadyDisliked = false;
    return true;
}

/**
 * Adds a user of type streamer to the StreamZ platform
 *
 * @param s the streamer to be added
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::addStreamer(const string& nickname, const Date& birthday) {
    Streamer *s1 = new Streamer(nickname, birthday);
    streamers.push_back(s1);
}

/**
 * Adds a user of type viewer to the StreamZ platform
 *
 * @param s the streamer to be added
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::addViewer(const string& nickname, const Date& birthday) {
    Viewer *v1 = new Viewer(nickname, birthday);
    viewers.push_back(v1);
}

/**
 * Likes the stream that the user is viewing
 *
 * Checks if the user already liked or disliked the stream. If not, likes the stream.
 *
 * @param v the viewer liking the stream
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::likeStream(Viewer *v) {
    if(!v->isActive()) return false;
    return v->s->addLike(v->getID());
}

/**
 * Dislikes the stream that the user is viewing
 *
 * Checks if the user already liked or disliked the stream. If not, dislikes the stream.
 *
 * @param v the viewer liking the stream
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::dislikeStream(Viewer *v) {
    if(!v->isActive()) return false;
    return v->s->addDislike(v->getID());
}

/**
 * Removes a like from the stream that the user is viewing
 *
 * Checks if the user already liked. If so, removes the like.
 *
 * @param v the viewer that liked the stream
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::remlikeStream(Viewer *v) {
    if(!v->isActive()) return false;
    return v->s->remLike(v->getID());
}

/**
 * Removes a dislike from the stream that the user is viewing
 *
 * Checks if the user already disliked. If so, removes the dislike.
 *
 * @param v the viewer that disliked the stream
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::remdislikeStream(Viewer *v) {
    if(!v->isActive()) return false;
    return v->s->remDislike(v->getID());
}

/**
 * Gets the number of registered streamers
 *
 * @return the number of registered streamers
 */
unsigned StreamZ::getStreamers() const {
    return streamers.size();
}

/**
 * Gets the number of registered viewers
 *
 * @return the number of registered viewers
 */
unsigned StreamZ::getViewers() const {
    return viewers.size();
}

/**
 * Gets a streamer by its ID
 *
 * @param id the streamer's ID
 * @return a pointer to the streamer, a nullptr if the ID is invalid
 */
Streamer *StreamZ::getStreamerByID(unsigned id) {
    for(int i = 0; i < getStreamers(); i++){
        if(streamers.at(i)->getID() == id)
            return streamers.at(i);
    }
    return nullptr;
}

/**
 * Gets a viewer by its ID
 *
 * @param id the viewer's ID
 * @return a pointer to the viewer, a nullptr if the ID is invalid
 */
Viewer *StreamZ::getViewerByID(unsigned id) {
    for(int i = 0; i < getViewers(); i++){
        if(viewers.at(i)->getID() == id)
            return viewers.at(i);
    }
    return nullptr;
}

/**
 * Prints the active streams
 */
void StreamZ::printActiveStreams() {
    for(int i = 0; i < streamers.size(); i++){
        if(streamers.at(i)->isActive())
            cout << "Streamer id: " << to_string(streamers.at(i)->getID()) << "   Stream: " << streamers.at(i)->s->getTitle() << endl;
    }
}

/*
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
 */