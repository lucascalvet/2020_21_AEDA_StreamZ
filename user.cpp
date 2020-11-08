#include "user.h"

using namespace std;

///Static member to have unique id's to each user
int User::counter = 0;

/**
 * Constructs a user object
 *
 * @param nickname the user's nickname
 * @param birthday the user's date of birth
 */
User::User(string nickname, Date birthday){
    this->nickname = nickname;
    this->birthday = birthday;
    id = counter++;
}

User::~User(){
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
int User::getID() const {
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
 * Constructs a viewer object
 *
 * @param nickname the viewer's nickname
 * @param birthday the viewer's date of birth
 */
Viewer::Viewer(string nickname, Date birthday) : User(nickname , birthday){
}

Viewer::~Viewer() {
}

/**
 * Prints the viewer's information
 */
void Viewer::printInfo() {
    cout << "Name: " << nickname << endl;
    cout << "Id: " << to_string(getID()) << endl;
    cout << "Birthday: " << getBirthday() << endl;
    cout << "Viewing: " << to_string(isActive()) << endl;
    if(isActive()) {
        cout << "Stream name: " << s->getTitle() << endl;
    }
}

/**
 * Constructs a streamer object
 *
 * @param nickname the streamer's nickname
 * @param birthday the streamer's date of birth
 */
Streamer::Streamer(string nickname, Date birthday) : User(nickname, birthday){
}

Streamer::~Streamer() {
}

/**
 * Gets the streamer's total views
 *
 * @return the streamer's number of total views
 */
unsigned Streamer::getTotalViews() const {
    return total_viewers.size();
}

/**
 * Gets the streamer's number of active viewers
 *
 * @return the streamer's number of active viewers
 */
unsigned Streamer::getActiveViewers() const {
    return active_viewers.size();
}

/**
 * Checks if a user is active.
 *
 * If the user is a viewer, checks if it is viewing any stream.
 * If the user is a streamer, checks if it is streaming
 *
 * @return the user's active state
 */
bool User::isActive() const{
    if(s == NULL) return false;
    return true;
}

/**
 * Prints the streamer's information
 */
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

/**
 * Constructs an admin object
 *
 * @param nickname the admin's nickname
 * @param birthday the admin's date of birth
 */
Admin::Admin(string nickname, Date birthday) : User(nickname , birthday){}

Admin::~Admin(){
}
