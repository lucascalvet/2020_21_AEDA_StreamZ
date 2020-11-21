#include "streamz.h"
#include <fstream>
#include "exceptions.h"
#include <limits>

using namespace std;

int StreamZ::counter = 0;

/**
 * Constructor for the StreamZ class
 *
 * @param capacity the maximum capacity of streams the platform may have at any time
 */
StreamZ::StreamZ(unsigned capacity, const std::string &nickname, const Date &birthday, const std::string &password) {

    string hashed_password = sha256Encode(password);  //encrypts password

    admin = new Admin(nickname, birthday, hashed_password);
    id = counter++;
    this->capacity = capacity;
}

StreamZ::StreamZ(const string &filename) {
    id = counter++;
    capacity = 0;
    ifstream file;
    file.open(filename);
    if (file.fail()) throw InvalidFile(filename);
    string validate;
    getline(file, validate);
    if (validate != VALIDATION_STRING) throw InvalidFile(filename);
    file >> capacity;
    file.get();
    string nickname, password;
    unsigned user_id, day, month, year;
    char sep;
    file >> user_id;
    file.get();
    getline(file, nickname, '\t');
    getline(file, password, '\t');
    file >> day >> sep >> month >> sep >> year;
    Date birthday(day, month, year);
    admin = new Admin(nickname, birthday, password, user_id);
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    while (file.peek() != '\n') {
        file >> user_id;
        file.get();
        getline(file, nickname, '\t');
        getline(file, password, '\t');
        file >> day >> sep >> month >> sep >> year;
        birthday = Date(day, month, year);
        Viewer *viewer = new Viewer(nickname, birthday, password, user_id);
        viewers.push_back(viewer);
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    file.get();
    while (file.peek() != EOF && file.peek() != '\n') {
        file >> user_id;
        file.get();
        getline(file, nickname, '\t');
        getline(file, password, '\t');
        file >> day >> sep >> month >> sep >> year;
        birthday = Date(day, month, year);
        Streamer *streamer = new Streamer(nickname, birthday, password, user_id);
        streamers.push_back(streamer);
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        while (file.peek() != '\n') {
            string title, lang;
            unsigned min_age, num_views;
            getline(file, title, '\t');
            file >> day >> sep >> month >> sep >> year;
            Date starting_date(day, month, year);
            file.get();
            getline(file, lang, '\t');
            file >> min_age >> num_views;
            vector<unsigned> viewers_liked;
            file.get();
            while (file.get() == ',') {
                file >> user_id;
                viewers_liked.push_back(user_id);
            }
            vector<unsigned> viewers_disliked;
            while (file.get() == ',') {
                file >> user_id;
                viewers_disliked.push_back(user_id);
            }
            if (file.peek() == '\n') {
                PublicStream *pub_stream = new PublicStream(title, lang, min_age, starting_date, num_views,
                                                            viewers_liked, viewers_disliked);
                streamer->addToHistory(pub_stream);
                file.get();
                continue;
            }
            vector<unsigned> auth_viewers;
            while (file.get() == ',') {
                file >> user_id;
                auth_viewers.push_back(user_id);
            }
            string comment;
            vector<string> comments;
            while (file.peek() != '\n') {
                getline(file, comment, '\t');
                comments.push_back(comment);
            }
            PrivateStream *private_stream = new PrivateStream(title, lang, min_age, starting_date, num_views,
                                                           viewers_liked, viewers_disliked, auth_viewers, comments);
            streamer->addToHistory(private_stream);
            file.get();
        }
        file.get();
    }
}

/**
 * Deletes all pointers
 */
StreamZ::~StreamZ() {
    vector<Streamer *>::iterator streamer;
    vector<Viewer *>::iterator viewer;
    vector<Stream *>::iterator stream;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        //delete (*viewer)->s;
        delete *streamer;
    }
    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
        //delete (*viewer)->s;
        delete *viewer;
    }
    delete admin;
}

/**
 * Gets the capacity of the StreamZ platform
 *
 * @return the capacity of the StreamZ platform
 */
unsigned int StreamZ::getCapacity() const {
    return capacity;
}

/**
 * Gets the ID of the StreamZ platform
 *
 * @return the ID of the StreamZ platform
 */
unsigned StreamZ::getID() const {
    return this->id;
}

/**
 * Gets the number of registered viewers
 *
 * @return the number of registered viewers
 */
unsigned StreamZ::getNumViewers() const {
    return viewers.size();
}

/**
 * Get the number of viewers in a stream
 * @return the number of viewers in a stream
 */
unsigned StreamZ::getNumStreamViewers(Stream *stream) const {
    unsigned count = 0;
    vector<Viewer *>::const_iterator viewer;
    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
        if ((*viewer)->s == stream) count++;
    }
    return count;
}

/**
 * Gets all the registered viewers
 * @return a vector with pointers to all the registered viewers
 */
vector<Viewer *> StreamZ::getViewers() const {
    return this->viewers;
}

/**
 * Gets a viewer by its ID
 *
 * @param user_id the viewer's ID
 * @return a pointer to the viewer, a nullptr if the ID is invalid
 */
Viewer *StreamZ::getViewerByID(unsigned user_id) const {
    vector<Viewer *>::const_iterator viewer;
    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
        if ((*viewer)->getID() == user_id)
            return *viewer;
    }
    return nullptr;
}

/**
 * Gets a viewer by its nickname
 *
 * @param user_id the viewer's nickname
 * @return a pointer to the viewer, a nullptr if the nickname is invalid
 */
Viewer *StreamZ::getViewerByName(const string &nickname) const {
    vector<Viewer *>::const_iterator viewer;
    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
        if ((*viewer)->getName() == nickname)
            return *viewer;
    }
    return nullptr;
}

/**
 * Gets the number of registered streamers
 *
 * @return the number of registered streamers
 */
unsigned StreamZ::getNumStreamers() const {
    return streamers.size();
}

/**
 * Gets all the registered streamers
 * @return a vector with pointers to all the registered streamers
 */
vector<Streamer *> StreamZ::getStreamers() const {
    return this->streamers;
}

/**
 * Gets a streamer by its ID
 *
 * @param user_id the streamer's ID
 * @return a pointer to the streamer, a nullptr if the ID is invalid
 */
Streamer *StreamZ::getStreamerByID(unsigned user_id) const {
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        if ((*streamer)->getID() == user_id)
            return *streamer;
    }
    return nullptr;
}

/**
 * Gets a streamer by its nickname
 *
 * @param user_id the streamer's nickname
 * @return a pointer to the streamer, a nullptr if the nickname is invalid
 */
Streamer *StreamZ::getStreamerByName(const string &nickname) const {
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        if ((*streamer)->getName() == nickname)
            return *streamer;
    }
    return nullptr;
}

/**
 * Gets the number of active streams
 *
 * @return the number of active streams
 */
unsigned StreamZ::getNumActiveStreamers() const {
    unsigned count = 0;
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        if ((*streamer)->s != nullptr)
            count++;
    }
    return count;
}

/**
 * Gets all the active streamers
 * @return a vector with pointers to all the active streamers
 */
vector<Streamer *> StreamZ::getActiveStreamers() const {
    vector<Streamer *> active_streamers;
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        if ((*streamer)->isActive()) active_streamers.push_back(*streamer);
    }
    return active_streamers;
}

/**
 * Gets the best ten streams from the StreamZ platform
 * @return the best ten streams
 */
vector<Stream *> StreamZ::getBestStreams() const {
    return this->best_streams;
}

/**
 * Get the streamer with the most total views
 * @return the most viewed streamer
 */
Streamer *StreamZ::getMostViewedStreamer() const {
    if (streamers.empty()) return nullptr;
    Streamer *most_viewed = nullptr;
    unsigned most_views = 0;
    unsigned views;
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        views = 0;
        vector<Stream *> history = (*streamer)->getHistory();
        vector<Stream *>::const_iterator stream;
        for (stream = history.begin(); stream != history.end(); stream++) {
            views += (*stream)->getNumTotalViews();
        }
        if (most_viewed == nullptr) {
            most_viewed = *streamer;
            most_views = views;
        } else if (views > most_views) {
            most_viewed = *streamer;
            most_views = views;
        } else if (views == most_views &&
                   (*most_viewed->getHistory().end() - 1)->getDate() < (*history.end() - 1)->getDate()) {
            most_viewed = *streamer;
        }
    }
    return most_viewed;
}

/**
 * Get the number of all previously created streams
 *
 * @return the number of created streams
 */
unsigned StreamZ::getNumCreatedStreams() const {
    unsigned count = 0;
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        count += (*streamer)->getHistory().size();
        if ((*streamer)->isActive()) count++;
    }
    return count;
}

unsigned StreamZ::getNumCreatedStreams(const Language &lang) const {
    unsigned count = 0;
    vector<Stream *> streams;
    vector<Streamer *>::const_iterator streamer;
    vector<Stream *>::const_iterator stream;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        streams = (*streamer)->getHistory();
        if ((*streamer)->isActive()) streams.push_back((*streamer)->s);
        for (stream = streams.begin(); stream != streams.end(); stream++)
            if ((*stream)->getLanguage() == lang) count++;
    }
    return count;
}

unsigned StreamZ::getNumCreatedStreams(bool public_streams, const Date &dt1, const Date &dt2) const {
    unsigned count = 0;
    vector<Stream *> streams;
    vector<Streamer *>::const_iterator streamer;
    vector<Stream *>::const_iterator stream;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        streams = (*streamer)->getHistory();
        if ((*streamer)->isActive()) streams.push_back((*streamer)->s);
        for (stream = streams.begin(); stream != streams.end(); stream++) {
            if (dt1 <= (*stream)->getDate() && (*stream)->getDate() <= dt2) {
                if (dynamic_cast<PrivateStream *>(*stream) == nullptr) {
                    if (public_streams) count++;
                } else if (!public_streams) count++;
            }
        }
    }
    return count;
}

/**
 * Get the average number of total views on previous streams
 *
 * @return the average number of total views on previous streams
 */
double StreamZ::getAverageViews() const {
    unsigned amount = 0;
    double total_average = 0;
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        vector<Stream *> history = (*streamer)->getHistory();
        vector<Stream *>::const_iterator stream;
        for (stream = history.begin(); stream != history.end(); stream++) {
            total_average += (*stream)->getNumTotalViews();
            amount++;
        }
    }
    total_average = total_average / amount;
    return total_average;
}


/**
 * Starts a public stream, with all of its initial properties
 *
 * Checks if the streamer already has an active stream and if the StreamZ platform hasn't reached
 * its maximum capacity of streams. If so, it starts the stream.
 *
 * @param streamer the streamer starting the stream
 * @param title the stream's title
 * @param lang the stream's language
 * @param min_age the stream's minimum age
 * @return
 */
void StreamZ::startPublicStream(Streamer *streamer, const string &title, const Language &lang, unsigned min_age) const {
    if (streamer->isActive()) throw AlreadyStreaming();
    if (getNumActiveStreamers() == this->capacity) throw FullCapacity();
    try {
        streamer->s = new PublicStream(title, lang, min_age);
    }
    catch (InvalidLanguage &) {
        throw;
    }

}

/**
 * Starts a private stream, with all of its initial properties
 *
 * Checks if the streamer already has an active stream and if the StreamZ platform hasn't reached
 * its maximum cap of streams. If so, it starts the stream.
 *
 * @param streamer the streamer starting the stream
 * @param title the stream's title
 * @param lang the stream's language
 * @param min_age the stream's minimum age
 * @param authorized_viewers a list of of the authorized viewers of the stream
 * @param cap the cap of the stream
 * @return
 */
void StreamZ::startPrivateStream(Streamer *streamer, const string &title, const Language &lang, unsigned min_age,
                                 const vector<unsigned int> &authorized_viewers) const {
    if (streamer->isActive()) throw AlreadyStreaming();
    if (getNumActiveStreamers() == this->capacity) throw FullCapacity();
    try {
        streamer->s = new PrivateStream(title, lang, min_age, authorized_viewers);
    }
    catch (InvalidLanguage &) {
        throw;
    }
}

/**
 * Stops a streamer's stream and adds or not to best streams. Checks if the streamer has indeed an active stream
 *
 * @param streamer the streamer that stopped its stream
 * @return
 */
void StreamZ::stopStream(Streamer *streamer) {
    if (!streamer->isActive()) throw NotStreaming();

    vector<Viewer *>::const_iterator viewer;
    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
        if ((*viewer)->s == streamer->s) (*viewer)->s = nullptr;
    }

    if (dynamic_cast<PrivateStream *>(streamer->s) != nullptr) {
        streamer->stopStreaming();  //private streams doesn't enter in best streams
        return;
    }

    for (unsigned i = 0; i < 9; i++) {
        if (best_streams.at(i) == nullptr) {
            best_streams.at(i) = streamer->s;
            break;
        }
        if (streamer->s->getNumTotalViews() > best_streams.at(i)->getNumTotalViews()) {
            for (unsigned j = 9; j > i; j--) {
                best_streams.at(j) = best_streams.at(j - 1);
            }
            best_streams.at(i) = streamer->s;
            break;
        }
    }
    for (unsigned i = 10; i < 19; i++) {
        if (best_streams.at(i) == nullptr) {
            best_streams.at(i) = streamer->s;
            break;
        }
        if (streamer->s->getNumLikes() > best_streams.at(i)->getNumLikes()) {
            for (unsigned j = 19; j > i; j--) {
                best_streams.at(j) = best_streams.at(j - 1);
            }
            break;
        }
    }
    streamer->stopStreaming();
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
    if (v->isActive()) {
        return false;
    }
    if (!streamer->isActive()) {
        return false;
    }
    PrivateStream *stream = dynamic_cast<PrivateStream *>(streamer->s);
    if (stream != nullptr && !stream->isAuthorized(v->getID())) return false;
    streamer->s->addView();
    v->s = streamer->s;
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
bool StreamZ::exitStream(Viewer *v) {
    if (!v->isActive()) {
        cout << "User is not viewing any stream!" << endl;
        return false;
    }
    //TODO: change number of viewers in stream?????
    v->s = nullptr;  //exiting stream
    return true;
}

/**
 * Adds a user of type streamer to the StreamZ platform
 *
 * @param nickname the user's nickname
 * @param birthday the user's date of birth
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::addStreamer(const string &nickname, const Date &birthday, const std::string &password) {
    if (calculateAge(birthday) < MIN_AGE_STREAMER) return false;
    vector<Streamer *>::const_iterator streamer_it;
    vector<Viewer *>::const_iterator viewer_it;
    if (admin->getName() == nickname) return false;
    for (streamer_it = streamers.begin(); streamer_it != streamers.end(); streamer_it++)
        if ((*streamer_it)->getName() == nickname) return false;
    for (viewer_it = viewers.begin(); viewer_it != viewers.end(); viewer_it++)
        if ((*viewer_it)->getName() == nickname) return false;

    string hashed_password = sha256Encode(password);  //encrypts password

    Streamer *s1 = new Streamer(nickname, birthday, hashed_password);
    streamers.push_back(s1);
    return true;
}

/**
 * Adds a user of type viewer to the StreamZ platform
 *
 * @param nickname the user's nickname
 * @param birthday the user's date of birth
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::addViewer(const string &nickname, const Date &birthday, const std::string &password) {
    if (calculateAge(birthday) < MIN_AGE_VIEWER) return false;
    vector<Streamer *>::const_iterator streamer_it;
    vector<Viewer *>::const_iterator viewer_it;
    if (admin->getName() == nickname) return false;
    for (streamer_it = streamers.begin(); streamer_it != streamers.end(); streamer_it++)
        if ((*streamer_it)->getName() == nickname) return false;
    for (viewer_it = viewers.begin(); viewer_it != viewers.end(); viewer_it++)
        if ((*viewer_it)->getName() == nickname) return false;

    string hashed_password = sha256Encode(password);  //encrypts password

    Viewer *v1 = new Viewer(nickname, birthday, hashed_password);
    viewers.push_back(v1);
    return true;
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
    if (!v->isActive()) return false;
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
    if (!v->isActive()) return false;
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
    if (!v->isActive()) return false;
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
    if (!v->isActive()) return false;
    return v->s->remDislike(v->getID());
}

/**
 * Prints the active streams
 */
void StreamZ::printActiveStreams() const {
    vector<Streamer *> active_streamers = getActiveStreamers();
    vector<Streamer *>::const_iterator streamer;
    for (streamer = active_streamers.begin(); streamer != active_streamers.end(); streamer++) {
        cout << "Streamer id: " << to_string((*streamer)->getID()) << "   Stream: " << (*streamer)->s->getInfo()
             << endl;
    }
}

/**
 * Gets a list of streamers streaming a stream with certain characteristics
 *
 * Selects the active streamers which the stream matches the language and has a minimum age smaller or
 * equal to the one specified. One can call the function without parameters in order to get all active
 * streams.
 *
 * @param lang the language of streams to search for. Empty string for any language.
 * @param min_age the maximum minimum age for the streams to search for. Leave blank for any minimum age.
 * @return the list of streamers streaming the specified streams
 */
vector<Streamer *> StreamZ::getStreams(const Language &lang = "", Age min_age = UINT_MAX) const {
    vector<Streamer *> ret_streams;
    vector<Streamer *>::const_iterator streamer;
    for (streamer = this->streamers.begin(); streamer != this->streamers.end(); streamer++) {
        if (!(*streamer)->isActive()) continue;
        if (lang.empty() || lang == (*streamer)->s->getLanguage() && min_age >= (*streamer)->s->getMinAge())
            ret_streams.push_back(*streamer);
    }
    return ret_streams;
}

void StreamZ::stopAllStreams() {
    vector<Streamer *> active_streamers = getActiveStreamers();
    vector<Streamer *>::iterator streamer;
    for (streamer = active_streamers.begin(); streamer != active_streamers.end(); streamer++) {
        stopStream(*streamer);
    }
}

/**
 * Saves the StreamZ object to a text file, in a formatted way.
 *
 * @param filename the name of the file to save to
 * @return true if the operation was successful, false otherwise
 */
bool StreamZ::save(const string &filename) const {
    ofstream file;
    file.open(filename, ofstream::trunc);
    if (file.fail()) return false;
    file << VALIDATION_STRING << '\n';
    file << this->capacity << '\n';
    file << admin->getID() << '\t' << admin->getName() << '\t' << admin->getPassword() << '\t'
         << admin->getBirthday() << '\n';
    vector<Viewer *>::const_iterator viewer;
    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
        file << (*viewer)->getID() << '\t' << (*viewer)->getName() << '\t' << (*viewer)->getPassword() << '\t'
             << (*viewer)->getBirthday() << '\n';
    }
    file << '\n';
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        file << (*streamer)->getID() << '\t' << (*streamer)->getName() << '\t' << (*streamer)->getPassword() << '\t'
             << (*streamer)->getBirthday() << '\n';
        vector<Stream *> history = (*streamer)->getHistory();
        vector<Stream *>::const_iterator stream;
        for (stream = history.begin(); stream != history.end(); stream++) {
            file << (*stream)->getTitle() << '\t' << (*stream)->getDate() << '\t' << (*stream)->getLanguage()
                 << '\t' << (*stream)->getMinAge() << '\t' << (*stream)->getNumTotalViews() << '\t';
            vector<unsigned>::const_iterator id;
            vector<unsigned> viewers_liked = (*stream)->getViewersLiked();
            for (id = viewers_liked.begin(); id != viewers_liked.end(); id++) {
                file << ',' << (*id);
            }
            file << '\t';
            vector<unsigned> viewers_disliked = (*stream)->getViewersDisliked();
            for (id = viewers_disliked.begin(); id != viewers_disliked.end(); id++) {
                file << ',' << (*id);
            }
            file << '\t';
            PrivateStream *private_stream = dynamic_cast<PrivateStream *>(*stream);
            if (private_stream != nullptr) {
                vector<unsigned> auth = (*private_stream).getAuthorizedViewers();
                vector<unsigned>::const_iterator id;
                for (id = auth.begin(); id != auth.end(); id++) {
                    file << ',' << (*id);
                }
                file << '\t';
                vector<string> comments = (*private_stream).getComments();
                vector<string>::const_iterator comment;
                for (comment = comments.begin(); comment != comments.end(); comment++) {
                    file << (*comment) << '\t';
                }
            }
            file << '\n';
        }
        file << '\n';
    }
    return true;
}

/**
 * Verifies is a user is already registered using sha256Verifier()
 * @param nickname the user nickname
 * @param password the user password
 * @return true if it is already registered, false otherwise
 */
bool StreamZ::loginVerifier(string nickname, string password_inputted) const {
    if (admin->getName() == nickname && sha256Verifier(admin->getPassword(), password_inputted)) return true;
    for (int i = 0; i < streamers.size(); i++) {
        if (streamers.at(i)->getName() == nickname && sha256Verifier(streamers.at(i)->getPassword(), password_inputted))
            return true;
    }
    for (int i = 0; i < viewers.size(); i++) {
        if (viewers.at(i)->getName() == nickname && sha256Verifier(viewers.at(i)->getPassword(), password_inputted))
            return true;
    }
    return false;
}

/**
 * Gets user by it's nickname
 * @param nickname the user nickname
 * @return user pointer to user if it exists, nullptr otherwise
 */
User *StreamZ::getUserByName(const string &nickname) {
    if(admin->getName() == nickname) return admin;
    for (int i = 0; i < streamers.size(); i++) {
        if (streamers.at(i)->getName() == nickname) return streamers.at(i);
    }
    for (int i = 0; i < viewers.size(); i++) {
        if (viewers.at(i)->getName() == nickname) return viewers.at(i);
    }
    return nullptr;
}
