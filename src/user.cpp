#include "user.h"
#include <sstream>
#include "exceptions.h"

using namespace std;

///Static member to have unique id's to each user
unsigned User::counter = 0;

/**
 * Construct a user object
 *
 * @param nickname the user's nickname
 * @param birthday the user's date of birth
 * @param password the user's password
 */
User::User(const string &nickname, const Date &birthday, const string &password) {
    this->nickname = nickname;
    this->birthday = birthday;
    this->password = password;
    wallet = 0; //Part 2
    id = counter++;
}

/**
 * Construct a user object with a defined id (for when importing from a file)
 * NEEDS TO HAVE THE LAST ID
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
    counter = id++;
}

/**
 * Get the user's information
 *
 * @return a string with the user's name, ID, and date of birth
 */
string User::getInfo() {
    ostringstream info;
    info << "Name: " << nickname;
    info << "\tId: " << to_string(getID());
    info << "\tBirthday: " << getBirthday();
    return info.str();
}

/**
 * Get the user's name
 *
 * @return the user's name
 */
string User::getName() const {
    return nickname;
}

/**
 * Get the user's date of birth
 *
 * @return the user's date of birth
 */
Date User::getBirthday() const {
    return birthday;
}

/**
 * Get the user's password
 *
 * @return the user's password
 */
std::string User::getPassword() const {
    return password;
}

/**
 * Get the user's ID
 *
 * @return the user's ID
 */
unsigned User::getID() const {
    return id;
}

/**
 * Checks if a user is active.
 *
 * If the user is a viewer, checks if it is viewing any stream.
 * If the user is a streamer, checks if it is streaming.
 *
 * @return the user's active state
 */
bool User::isActive() const {
    return s != nullptr;
}

/**
 * Deposits the amount of money passed at parameter to user wallet
 *
 * @param amount of money to be deposited
 *
 */
void User::cashDeposit(double amount) {
    wallet += amount;
}

/**
 * Get the user wallet amount
 *
 * @return the value of user wallet
 */
double User::getWalletAmount() const {
    return wallet;
}

/**
 * Withdraws a determined amount from the user wallet
 *
 * @param amount to withdraw from the wallet
 * @return the value of the wallet after the withdraw
 */
double User::cashWithdraw(double amount) {
    wallet -= amount;
    return wallet;
}

/**
 * Construct a streamer object
 *
 * @param nickname the streamer's nickname
 * @param birthday the streamer's date of birth
 * @param password the streamer's password
 */
Streamer::Streamer(const string &nickname, const Date &birthday, const string &password) : User(nickname, birthday,
                                                                                                password) {
}

/**
* Construct a streamer object with a defined id (for when importing from a file)
*
* @param nickname the streamer's nickname
* @param birthday the streamer's date of birth
* @param password the streamer's password
* @param id the streamer's id
*/
Streamer::Streamer(const string &nickname, const Date &birthday, const string &password, unsigned int id) : User(
        nickname,
        birthday,
        password, id) {
}

/**
 * Construct a user object (just a mockup object for searching purposes)
 * @param nickname the user's nickname
 */
User::User(const string &nickname) {
    this->nickname = nickname;
    this->birthday = Date(1, 1, 1900);
    this->password = "";
    wallet = 0;
    id = 0;
}

/**
 * Construct a streamer object (just a mockup object for searching purposes)
 * @param nickname the streamer's nickname
 */
Streamer::Streamer(const std::string &nickname) : User(nickname){
}

/**
 * Delete all pointers
 */
Streamer::~Streamer() {
    delete s;
    vector<Stream *>::iterator stream;
    for (stream = streaming_history.begin(); stream != streaming_history.end(); stream++) {
        delete *stream;
    }
}

/**
 * Get the streamer's information
 *
 * @return a string with the streamer's information
 */
string Streamer::getInfo() {
    string active_status;

    if (isActive()) active_status = "Active";
    else active_status = "Inactive";

    ostringstream info;
    info << User::getInfo();
    info << "\t" << "Streaming: " << active_status << "\n";
    if (isActive()) {
        info << "Stream-> " << s->getInfo() << endl;
    }
    cout << endl;
    return info.str();
}

/**
 * Get the streamer's stream history
 *
 * @return the streamer's stream history
 */
std::vector<Stream *> Streamer::getHistory() const {
    return streaming_history;
}

/**
 * Get the account status
 *
 * @return true for an active account, false otherwise
 */
bool Streamer::getAccountStatus() const {
    return active_account;
}

/**
 * Activates an account and states if it is eligible for bonus or not
 */
void Streamer::toggleAccountStatus() {
    if (!active_account) eligible_for_bonus = true;
    active_account = !active_account;
}

/**
 * Gives a bonus of likes to a streamer's stream and changes the eligible for the same
 */
void Streamer::giveBonus() {
    if (!isActive()) throw InactiveUser();
    if (eligible_for_bonus){
        s->addBonus();
        eligible_for_bonus = false;
    }
}

/**
 * Add a stream to the streamer's history
 *
 * @param stream the stream to be added to the history
 */
void Streamer::addToHistory(Stream *stream) {
    streaming_history.push_back(stream);
}

/**
 * Stop the streamer from streaming.
 *
 * Checks if the streamer is indeed streaming. If so, stops him from streaming
 */
void Streamer::stopStreaming() {
    if (!isActive()) throw InactiveUser();
    streaming_history.push_back(s);
    s = nullptr;
}

/**
 * Construct a viewer object
 *
 * @param nickname the viewer's nickname
 * @param birthday the viewer's date of birth
 * @param password the viewer's password
 */
Viewer::Viewer(const string &nickname, const Date &birthday, const string &password) : User(nickname, birthday,
                                                                                            password) {}

/**
* Construct a viewer object with a defined id (for when importing from a file)
*
* @param nickname the viewer's nickname
* @param birthday the viewer's date of birth
* @param password the viewer's password
* @param id the viewer's id
*/
Viewer::Viewer(const string &nickname, const Date &birthday, const string &password, unsigned int id) : User(nickname,
                                                                                                             birthday,
                                                                                                             password,
                                                                                                             id) {}

/**
 * Get the viewer's information
 *
 * @return a string with the viewer's information
 */
string Viewer::getInfo() {
    string active_status;

    if (isActive()) active_status = "Active";
    else active_status = "Inactive";

    ostringstream info;
    info << User::getInfo();
    info << "\t" << "Viewing: " << active_status << "\tWallet: " << getWalletAmount() << "\n";
    if (isActive()) {
        info << "Stream-> " << s->getInfo() << endl;
    }
    cout << endl;
    return info.str();
}

/**
 * Enter the viewer into a streamer's stream.
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
        throw InactiveUser();
    }
    if (calculateAge(getBirthday()) < streamer->s->getMinAge()){
        throw NoMinimumAge();
    }
    PrivateStream *stream = dynamic_cast<PrivateStream *>(streamer->s);
    if (stream != nullptr && !stream->isAuthorized(getID())) throw UnauthorizedViewer();
    streamer->s->addView();
    s = streamer->s;
}

/**
 * Remove the viewer from the stream he's watching
 *
 * Checks if the viewer is indeed watching a stream. If so, it removes him from the stream.
 */
void Viewer::exitStream() {
    if (!isActive()) {
        throw InactiveUser();
    }
    s = nullptr;  //exiting stream
}

/**
 * Like the stream that the viewer is watching
 */
void Viewer::likeStream() const {
    if (!isActive()) throw InactiveUser();
    try {
        s->addLike(getID());
    }
    catch (AlreadyInteracted &) {
        throw;
    }
}

/**
 * Remove a like from the stream that the viewer is watching
 *
 * Checks if the viewer already liked. If so, removes the like.
 */
void Viewer::remLikeStream() const {
    if (!isActive()) throw InactiveUser();
    try {
        s->remLike(getID());
    }
    catch (HasNotInteracted &) {
        throw;
    }
}

/**
 * Dislike the stream that the viewer is watching
 */
void Viewer::dislikeStream() const {
    if (!isActive()) throw InactiveUser();
    try {
        s->addDislike(getID());
    }
    catch (AlreadyInteracted &) {
        throw;
    }
}

/**
 * Remove a dislike from the stream that the viewer is watching
 *
 * Checks if the viewer already disliked. If so, removes the dislike.
 */
void Viewer::remDislikeStream() const {
    if (!isActive()) throw InactiveUser();
    try {
        s->remDislike(getID());
    }
    catch (AlreadyInteracted &) {
        throw;
    }
}

/**
 * Add a comment to the stream. Checks if the viewer is watching a private stream
 * @param comment
 */
void Viewer::comment(const string &comment) const {
    if (!isActive()) throw InactiveUser();
    PrivateStream *viewing_stream = dynamic_cast<PrivateStream *>(s);
    if (viewing_stream == nullptr) throw NotInPrivateStream();
    viewing_stream->addComment(comment);
}

/**
 * Construct an admin object
 *
 * @param nickname the admin's nickname
 * @param birthday the admin's date of birth
 * @param password the admin's password
 */
Admin::Admin(const string &nickname, const Date &birthday, const string &password) : User(nickname, birthday,
                                                                                          password) {}

/**
 * Construct an admin object
 *
 * @param nickname the admin's nickname
 * @param birthday the admin's date of birth
 * @param password the admin's password
 *
 */
Admin::Admin(const string &nickname, const Date &birthday, const string &password, unsigned id) : User(nickname,
                                                                                                       birthday,
                                                                                                       password, id) {
}
