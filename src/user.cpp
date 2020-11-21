#include "user.h"
#include <sstream>
#include "exceptions.h"

using namespace std;

///Static member to have unique id's to each user
unsigned User::counter = 0;

/**
 * Constructs a user object
 *
 * @param nickname the user's nickname
 * @param birthday the user's date of birth
 * @param password the user's password
 */
User::User(const string &nickname, const Date &birthday, const string &password) {
    this->nickname = nickname;
    this->birthday = birthday;
    this->password = password;
    id = counter++;
}

/**
 * Constructs a user object with a defined id (for importing from a file)
 *
 * @param nickname the user's nickname
 * @param birthday the user's date of birth
 * @param password the user's password
 * @param id the user's id
 */
User::User(const string &nickname, const Date &birthday, const string &password, unsigned int id) : User(nickname,
                                                                                                         birthday,
                                                                                                         password) {
    this->id = id;
}

User::~User() {
}

/**
 * Gets the user's name
 *
 * @return the user's name
 */
string User::getName() const {
    return nickname;
}

/**
 * Gets the user's ID
 *
 * @return the user's ID
 */
unsigned User::getID() const {
    return id;
}

/**
 * Gets the user's date of birth
 *
 * @return the user's date of birth
 */
Date User::getBirthday() const {
    return birthday;
}

/**
 * Prints the user's information
 *
 * @return a string with the user's name, id, and date of birth
 */
string User::getInfo() {
    ostringstream info;
    info << "Name: " << nickname;
    info << "\tId: " << to_string(getID());
    info << "\tBirthday: " << getBirthday();
    return info.str();
}

/**
 * Checks if a user is active.
 *
 * If the user is a viewer, checks if it is viewing any stream.
 * If the user is a streamer, checks if it is streaming
 *
 * @return the user's active state
 */
bool User::isActive() const {
    return s != nullptr;
}

/**
 * Constructs a viewer object
 *
 * @param nickname the viewer's nickname
 * @param birthday the viewer's date of birth
 */
Viewer::Viewer(const string &nickname, const Date &birthday, const string &password) : User(nickname, birthday,
                                                                                            password) {
}

Viewer::Viewer(const string &nickname, const Date &birthday, const string &password, unsigned int id) : User(nickname,
                                                                                                             birthday,
                                                                                                             password,
                                                                                                             id) {
}

/**
 * Prints the viewer's information
 *
 * @return a string with the viewer's information
 */
string Viewer::getInfo() {
    string active_status;

    if (isActive()) active_status = "Active";
    else active_status = "Inactive";

    ostringstream info;
    info << User::getInfo();
    info << "\t" << "Viewing: " << active_status << "\n";
    if (isActive()) {
        info << "Stream-> " << s->getInfo() << endl;
    }
    cout << endl;
    return info.str();
}

/**
 * Enters the viewer into a streamer's stream
 *
 * Checks if the viewer is not already in a stream and if the streamer has an active stream.
 * If so, enters the viewer into the streamer's stream.
 *
 * @param streamer the streamer streaming the stream in which the viewer wants to enter
 */
void Viewer::enterStream(Streamer *streamer) {
    if (isActive()) {
        throw AlreadyViewing();
    }
    if (!streamer->isActive()) {
        throw NotStreaming();
    }
    PrivateStream *stream = dynamic_cast<PrivateStream *>(streamer->s);
    if (stream != nullptr && !stream->isAuthorized(getID())) throw UnauthorizedViewer();
    streamer->s->addView();
    s = streamer->s;
}

/**
 * Removes a viewer from the stream he's watching
 *
 * Checks if the viewer is indeed watching a stream. If so, it removes him from the stream.
 *
 * @param v the viewer to remove from the stream
 * @return true if the operation was successful, false otherwise
 */
 void Viewer::exitStream() {
    if (!isActive()) {
        throw InactiveUser();
    }
    s = nullptr;  //exiting stream
 }

/**
 * Likes the stream that the viewer is watching
 *
 * @param v the viewer liking the stream
 */
void Viewer::likeStream() const{
    if (!isActive()) throw InactiveUser();
    try {
        s->addLike(getID());
    }
    catch (AlreadyInteracted&) {
        throw;
    }
}

/**
 * Removes a like from the stream that the user is viewing
 *
 * Checks if the user already liked. If so, removes the like.
 *
 * @param v the viewer that liked the stream
 * @return true if the operation was successful, false otherwise
 */
void Viewer::remlikeStream() const {
    if (!isActive()) throw InactiveUser();
    try {
        s->remLike(getID());
    }
    catch (HasNotInteracted&) {
        throw;
    }
}

/**
 * Dislikes the stream that the viewer is watching
 */
void Viewer::dislikeStream() const{
    if (!isActive()) throw InactiveUser();
    try {
        s->addDislike(getID());
    }
    catch (AlreadyInteracted&) {
        throw;
    }
}

/**
 * Removes a dislike from the stream that the viewer is watching
 *
 * Checks if the viewer already disliked. If so, removes the dislike.
 */
void Viewer::remdislikeStream() const {
    if (!isActive()) throw InactiveUser();
    try {
        s->remDislike(getID());
    }
    catch (AlreadyInteracted&) {
        throw;
    }
}

/**
 * Add a comment to the stream. Checks if a user is viewing a private stream
 * @param comment
 */
void Viewer::comment(const string &comment) const{
    if (!isActive()) throw InactiveUser();
    PrivateStream *viewing_stream = dynamic_cast<PrivateStream *>(s);
    if (viewing_stream == nullptr) throw NotInPrivateStream();
    viewing_stream->addComment(comment);
}

/**
 * Constructs a streamer object
 *
 * @param nickname the streamer's nickname
 * @param birthday the streamer's date of birth
 */
Streamer::Streamer(const string &nickname, const Date &birthday, const string &password) : User(nickname, birthday,
                                                                                                password) {
}

Streamer::Streamer(const string &nickname, const Date &birthday, const string &password, unsigned int id) : User(
        nickname,
        birthday,
        password, id) {
}

Streamer::~Streamer() {
    delete s;
    vector<Stream *>::iterator stream;
    for (stream = streaming_history.begin(); stream != streaming_history.end(); stream++) {
        delete *stream;
    }
}

/**
 * Gets the streamer's total views
 *
 * @return the streamer's number of total views
 */
unsigned Streamer::getTotalViews() const {
    return 0; //TODO: get from history
}

std::vector<Stream *> Streamer::getHistory() const {
    return streaming_history;
}

void Streamer::addToHistory(Stream *stream) {
    streaming_history.push_back(stream);
}

void Streamer::stopStreaming() {
    streaming_history.push_back(s);
    s = nullptr;
}

std::string User::getPassword() const {
    return password;
}

/**
 * Prints the streamer's information
 *
 * @return a string with the viewer's information
 */
string Streamer::getInfo() {
    string active_status;

    if (isActive()) active_status = "Active";
    else active_status = "Inactive";

    ostringstream info;
    info << User::getInfo();
    info << "\t" << "Streaming: " << active_status << "\t";
    if (isActive()) {
        info << "Stream: " << s->getInfo() << endl;
    }
    cout << endl;
    return info.str();
}

/**
 * Constructs an admin object
 *
 * @param nickname the admin's nickname
 * @param birthday the admin's date of birth
 */
Admin::Admin(const string &nickname, const Date &birthday, const string &password) : User(nickname, birthday,
                                                                                          password) {}

Admin::Admin(const string &nickname, const Date &birthday, const string &password, unsigned id) : User(nickname,
                                                                                                       birthday,
                                                                                                       password, id) {
}
