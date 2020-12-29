#include "streamz.h"
#include <fstream>
#include "exceptions.h"
#include <limits>
#include <map>
#include <set>

using namespace std;

int StreamZ::counter = 0;

/**
 * Construct a StreamZ class object
 *
 * @param capacity the maximum capacity of streams the platform may have at any time
 * @param nickname the admin's nickname
 * @param birthday the admin's date of birth
 * @param password the admin's password
 */
StreamZ::StreamZ(unsigned capacity, const std::string &nickname, const Date &birthday, const std::string &password) {
    string hashed_password = sha256Encode(password);  //encrypts password
    admin = new Admin(nickname, birthday, hashed_password);

    id = counter++;
    this->capacity = capacity;
}

/**
 * Construct a StreamZ class object, from a formatted file
 *
 * @param filename the name of the file to import the StreamZ from
 */
StreamZ::StreamZ(const string &filename) {
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
        streamers_hash_table.insert(streamer); //Part 2
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
                for (unsigned i = 0; i < 9; i++) {
                    if (best_streams.at(i) == nullptr) {
                        best_streams.at(i) = pub_stream;
                        break;
                    }
                    if (pub_stream->getNumTotalViews() > best_streams.at(i)->getNumTotalViews()) {
                        for (unsigned j = 9; j > i; j--) {
                            best_streams.at(j) = best_streams.at(j - 1);
                        }
                        best_streams.at(i) = pub_stream;
                        break;
                    }
                }
                for (unsigned i = 10; i < 19; i++) {
                    if (best_streams.at(i) == nullptr) {
                        best_streams.at(i) = pub_stream;
                        break;
                    }
                    if (pub_stream->getNumLikes() > best_streams.at(i)->getNumLikes()) {
                        for (unsigned j = 19; j > i; j--) {
                            best_streams.at(j) = best_streams.at(j - 1);
                        }
                        best_streams.at(i) = pub_stream;
                        break;
                    }
                }
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
    id = counter++;
}

/**
 * Delete all pointers
 */
StreamZ::~StreamZ() {
    vector<Streamer *>::iterator streamer;
    vector<Viewer *>::iterator viewer;
    vector<Stream *>::iterator stream;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        delete *streamer;
    }
    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
        delete *viewer;
    }
    delete admin;
}

/**
 * Get the capacity of the StreamZ platform
 *
 * @return the capacity of the StreamZ platform
 */
unsigned int StreamZ::getCapacity() const {
    return capacity;
}

/**
 * Get the ID of the StreamZ platform
 *
 * @return the ID of the StreamZ platform
 */
unsigned StreamZ::getID() const {
    return this->id;
}

/**
 * Get all the registered streamers
 *
 * @return a vector with pointers to all the registered streamers
 */
vector<Streamer *> StreamZ::getStreamers() const {
    return this->streamers;
}

/**
 * Get the number of registered streamers
 *
 * @return the number of registered streamers
 */
unsigned StreamZ::getNumStreamers() const {
    return streamers.size();
}

/**
 * Get all the active streamers
 *
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
 * Get the number of current active streamers
 *
 * @return the number of active streamers
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
 * Get all the registered viewers
 *
 * @return a vector with pointers to all the registered viewers
 */
vector<Viewer *> StreamZ::getViewers() const {
    return this->viewers;
}

/**
 * Get the number of registered viewers
 *
 * @return the number of registered viewers
 */
unsigned StreamZ::getNumViewers() const {
    return viewers.size();
}

/**
 * Get the number of viewers in a stream
 *
 * @param stream the stream to get the number of viewers from
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
 * Get a user by its nickname
 *
 * @param nickname the user's nickname
 * @return user a pointer to the user, if it exists, nullptr otherwise
 */
User *StreamZ::getUserByName(const string &nickname) {
    if (admin->getName() == nickname) return admin;
    for (int i = 0; i < streamers.size(); i++) {
        if (streamers.at(i)->getName() == nickname) return streamers.at(i);
    }
    for (int i = 0; i < viewers.size(); i++) {
        if (viewers.at(i)->getName() == nickname) return viewers.at(i);
    }
    return nullptr;
}

/**
 * Get a streamer by its ID
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
 * Get a streamer by its nickname
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
 * Get a viewer by its ID
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
 * Get a viewer by its nickname
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

/**
 * Get the number of created streams of a specific language
 *
 * @param lang the language of the streams
 * @return the number of created streams with language lang
 */
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

/**
 * Get the number of created public or private streams, optionally between a set time frame
 *
 * @param public_streams true to get the number of public streams, false to get the number of private streams
 * @param dt1 lower bound time frame date
 * @param dt2 upper bound time frame date
 * @return the number of created public or private streams
 */
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
 * Get a list of streamers streaming a stream with certain characteristics
 *
 * Selects the active streamers which the stream matches the language and has a minimum age smaller or
 * equal to the one specified. One can call the function without parameters in order to get all active
 * streams.
 *
 * @param lang the language of streams to search for. Empty string for any language.
 * @param min_age the maximum minimum age for the streams to search for. Leave blank for any minimum age.
 * @return the list of streamers streaming the specified streams
 */
vector<Streamer *> StreamZ::getStreams(Language lang, Age min_age) const {
    lang = strToUpper(lang);
    vector<Streamer *> ret_streams;
    vector<Streamer *>::const_iterator streamer;
    for (streamer = this->streamers.begin(); streamer != this->streamers.end(); streamer++) {
        if (!(*streamer)->isActive()) continue;
        if ((lang.empty() || lang == (*streamer)->s->getLanguage()) && min_age >= (*streamer)->s->getMinAge())
            ret_streams.push_back(*streamer);
    }
    return ret_streams;
}

/**
 * Get the best streams from the StreamZ platform (10 most viewed and 10 most liked)
 *
 * @return the best streams
 */
vector<Stream *> StreamZ::getBestStreams() const {
    return this->best_streams;
}

/**
 * Get the streamer with the most total views
 *
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
 * Get the language that is most used in the streams
 *
 * @return the language most used in the streams
 */
std::string StreamZ::getMostUsedLanguage() {
    if (getNumCreatedStreams() == 0) return "";
    string lang;
    map<string, unsigned> frequency;
    vector<Stream *> streams;
    vector<Streamer *>::const_iterator streamer;
    vector<Stream *>::const_iterator stream;
    for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
        streams = (*streamer)->getHistory();
        if ((*streamer)->isActive()) streams.push_back((*streamer)->s);
        for (stream = streams.begin(); stream != streams.end(); stream++) {
            lang = (*stream)->getLanguage();
            if (frequency.find(lang) == frequency.end()) frequency.insert(make_pair(lang, 1));
            else {
                frequency.at(lang)++;
            }
        }
    }
    pair<string, unsigned> max_lang = make_pair("", 0);
    map<string, unsigned>::const_iterator current_entry;
    for (current_entry = frequency.begin(); current_entry != frequency.end(); current_entry++) {
        if (current_entry->second > max_lang.second) {
            max_lang = make_pair(current_entry->first, current_entry->second);
        }
    }
    return max_lang.first;
}

/**
 * Print a list of streams
 *
 * @param streams the streams to be printed
 */
void StreamZ::printStreams(const vector<Streamer *> &streams) {
    vector<Streamer *>::const_iterator streamer;
    for (streamer = streams.begin(); streamer != streams.end(); streamer++) {
        cout << "ID:" << (*streamer)->getID() << " Streamer: " << (*streamer)->getName() << "\tStream-> " << (*streamer)->s->getInfo()
             << endl;
    }
}

/**
 * Add a user of type streamer to the StreamZ platform
 *
 * @param nickname the streamer's nickname
 * @param birthday the streamer's date of birth
 * @param password the streamer's password
 */
void StreamZ::addStreamer(const string &nickname, const Date &birthday, const std::string &password) {
    if (calculateAge(birthday) < MIN_AGE_STREAMER) throw NoMinimumAge();
    vector<Streamer *>::const_iterator streamer_it;
    vector<Viewer *>::const_iterator viewer_it;
    if (admin->getName() == nickname) throw NameAlreadyInUse();
    for (streamer_it = streamers.begin(); streamer_it != streamers.end(); streamer_it++)
        if ((*streamer_it)->getName() == nickname) throw NameAlreadyInUse();
    for (viewer_it = viewers.begin(); viewer_it != viewers.end(); viewer_it++)
        if ((*viewer_it)->getName() == nickname) throw NameAlreadyInUse();

    string hashed_password = sha256Encode(password);  //encrypts password

    Streamer *s1 = new Streamer(nickname, birthday, hashed_password);
    streamers.push_back(s1);
    streamers_hash_table.insert(s1); //Part 2
}

/**
 * Add a user of type viewer to the StreamZ platform
 *
 * @param nickname the viewer's nickname
 * @param birthday the viewer's date of birth
 * @param password the viewer's password
 */
void StreamZ::addViewer(const string &nickname, const Date &birthday, const std::string &password) {
    if (calculateAge(birthday) < MIN_AGE_VIEWER) throw NoMinimumAge();
    vector<Streamer *>::const_iterator streamer_it;
    vector<Viewer *>::const_iterator viewer_it;
    if (admin->getName() == nickname) throw NameAlreadyInUse();
    for (streamer_it = streamers.begin(); streamer_it != streamers.end(); streamer_it++)
        if ((*streamer_it)->getName() == nickname) throw NameAlreadyInUse();
    for (viewer_it = viewers.begin(); viewer_it != viewers.end(); viewer_it++)
        if ((*viewer_it)->getName() == nickname) throw NameAlreadyInUse();

    string hashed_password = sha256Encode(password);  //encrypts password

    Viewer *v1 = new Viewer(nickname, birthday, hashed_password);
    viewers.push_back(v1);
}

/**
 * Start a public stream, with all of its initial properties
 *
 * Checks if the streamer already has an active stream and if the StreamZ platform hasn't reached
 * its maximum capacity of streams. If so, it starts the stream.
 *
 * @param streamer the streamer starting the stream
 * @param title the stream's title
 * @param lang the stream's language
 * @param min_age the stream's minimum age
 */
void StreamZ::startPublicStream(Streamer *streamer, const string &title, const Language &lang, unsigned min_age) const {
    if (!streamer->getAccountStatus()) throw InactiveAccount(); //Part 2
    if (streamer->isActive()) throw AlreadyStreaming();
    if (getNumActiveStreamers() == this->capacity) throw FullCapacity();
    try {
        streamer->s = new PublicStream(title, lang, min_age);
    }
    catch (InvalidLanguage &) {
        throw;
    }
    streamer->giveBonus(); //Part 2
}

/**
 * Start a private stream, with all of its initial properties
 *
 * Checks if the streamer already has an active stream and if the StreamZ platform hasn't reached
 * its maximum capacity of streams. If so, it starts the stream.
 *
 * @param streamer the streamer starting the stream
 * @param title the stream's title
 * @param lang the stream's language
 * @param min_age the stream's minimum age
 * @param authorized_viewers a list of of the authorized viewers of the stream
 */
void StreamZ::startPrivateStream(Streamer *streamer, const string &title, const Language &lang, unsigned min_age,
                                 const vector<unsigned int> &authorized_viewers) const {
    if (!streamer->getAccountStatus()) throw InactiveAccount(); //Part 2
    if (streamer->isActive()) throw AlreadyStreaming();
    if (getNumActiveStreamers() == this->capacity) throw FullCapacity();
    try {
        streamer->s = new PrivateStream(title, lang, min_age, authorized_viewers);
    }
    catch (InvalidLanguage &) {
        throw;
    }
    streamer->giveBonus(); //Part 2
}

/**
 * Stops a streamer's stream and adds it or not to the best streams. Checks if the streamer has indeed an active stream
 *
 * @param streamer the streamer that stopped its stream
 */
void StreamZ::stopStream(Streamer *streamer) {
    if (!streamer->isActive()) throw InactiveUser();

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
            best_streams.at(i) = streamer->s;
            break;
        }
    }
    streamer->stopStreaming();
}

/**
 * Stop all active streams
 */
void StreamZ::stopAllStreams() {
    vector<Streamer *> active_streamers = getActiveStreamers();
    vector<Streamer *>::iterator streamer;
    for (streamer = active_streamers.begin(); streamer != active_streamers.end(); streamer++) {
        stopStream(*streamer);
    }
}

/**
 * Make a donation to a certain streamer with nickname nick
 *
 * @param nick the streamer's nickname
 * @param amnt the donation's amount
 * @param eval the evaluation given to the streamer
 */
void StreamZ::makeDonation(const Streamer* strmr, unsigned amnt, unsigned eval) {
    Donation donation(strmr->getName(), amnt, eval);
    donations.insert(donation);
}

BST<Donation> StreamZ::getDonations() const {
    return donations;
}

/**
 * Get a list with the n biggest donations (if n is specified, otherwise get all donations) with an evaluation between lower and upper
 * @param lower the lower bound of the evaluation
 * @param upper the upper bound of the evaluation
 * @param n the number of donations to list
 * @return a vector with the desired donations
 */
vector<Donation> StreamZ::getDonations(unsigned int lower, unsigned int upper, unsigned int n) {
    iteratorBST<Donation> it;
    vector<Donation> res;
    for (it = donations.begin(); it != donations.end(); it++) {
        if (res.size() == n) break;
        if ((*it).getEvaluation() >= lower && (*it).getEvaluation() <= upper)
            res.push_back(*it);
    }
    return res;
}

/**
 * Verify if a user is already registered using sha256Verifier()
 *
 * @param nickname the user's nickname
 * @param password the user's password
 * @return true if it is already registered, false otherwise
 */
bool StreamZ::loginVerifier(const string &nickname,const string& password_inputted) const {
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
 * Save the StreamZ object to a text file, in the same path as the executable, in a formatted way.
 *
 * @param filename the name of the file to save to
 */
void StreamZ::save(const string &filename) const {
    ofstream file;
    file.open(filename, ofstream::trunc);
    if (file.fail()) throw InvalidFile(filename);
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
}

/**
 * Searchs for an order with equal parameters of the ones given
 *
 * @param viewer_nickname the viewer nickname associated to the order
 * @param quantity quantity of the order to be searched
 * @param priority priority of the order to be searched
 * @return the order if found, or an empty order if not found
 */
Order StreamZ::searchOrder(std::string viewer_nickname, unsigned int quantity, unsigned int priority) {
    Order empty_order;
    Order ord_to_found(quantity, priority, viewer_nickname);

    priority_queue<Order> buffer = orders;

    while(!buffer.empty()){
        Order current_ord = buffer.top();
        buffer.pop();

        if(current_ord == ord_to_found) return current_ord;
    }

    return empty_order;
}

/**
 * Creates an order and adds it to the orders priority_queue
 *
 * @param viewer the viewer associated with that order
 * @param quantity the quantity of products to buy
 * @param priority the priority of the purchase
 */
void StreamZ::makeOrder(Viewer *viewer, unsigned int quantity, unsigned int priority) {
    Order empty_order;
    if(quantity > MAX_QUANTITY_PER_PURCHASE) throw ExceededMaxQuantityPerPurchase();

    if(searchOrder(viewer->getName(), quantity, priority) == empty_order){
        Order ord(quantity, priority, viewer->getName());

        orders.push(ord);
    }
    else{
        throw OrderAlreadyExists();
    }

}

/**
 * Deletes an order from the orders priority_queue
 *
 * @param viewer the viewer associated to the order
 * @param quantity the quantity of product of the order
 * @param priority the priority of the order
 */
void StreamZ::deleteOrder(Viewer *viewer, unsigned int quantity, unsigned int priority) {
    Order empty_order;

    if(searchOrder(viewer->getName(), quantity, priority) == empty_order){
        throw OrderDoesNotExist();
    }
    else {
        Order ord(quantity, priority, viewer->getName());
        vector<Order> auxiliar_vector;

        while(!orders.empty()){
            if(orders.top() == ord) {
                orders.pop();
                continue;
            }
            else{
                auxiliar_vector.push_back(orders.top());
            }
            orders.pop();
        }

        auto it = auxiliar_vector.begin();

        while(it != auxiliar_vector.end()){
            orders.push((*it));
            it++;
        }
    }
}

/**
 * Changes the value of the maximum orders that a viewer can make and it adjusts the queue accordingly
 *
 * @param new_limit the value to replace the current value of max_orders_per_viewer
 */
void StreamZ::changeMaxOrdersPerViewer(unsigned new_limit) {
    unsigned old_limit = max_orders_per_viewer;
    max_orders_per_viewer = new_limit;

    if(old_limit > new_limit) {
        unsigned counter = 0;

        set<string, greater<string>> unique_nicknames;

        priority_queue<Order> buffer = orders;
        priority_queue<Order> buffer2 = orders;

        while (!orders.empty()) {
            unique_nicknames.insert(buffer.top().getViewerNickname());
            orders.pop();
        }

        auto it = unique_nicknames.begin();

        while (it != unique_nicknames.end()) {
            while (!buffer.empty() && counter != new_limit) {
                if (buffer.top().getViewerNickname() == *it) {
                    orders.push(buffer.top());
                    counter++;
                }
                buffer.pop();
            }
            it++;
            counter = 0;
            buffer = buffer2;
        }
    }
}

/**
 * Get priority_queue of orders in streamz
 *
 * @return the orders priority_queue
 */
std::priority_queue<Order> StreamZ::getOrders() const {
    return orders;
}
