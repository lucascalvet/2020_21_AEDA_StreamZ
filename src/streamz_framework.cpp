#include "menu.h"
#include "streamz.h"
#include "streamz_framework.h"
#include "exceptions.h"
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

//to maintain readability of the code where they are used and avoiding having them global
void help_strings(string &help_main_menu, string &help_submenu) {
    help_main_menu =
            "\nFramework Help Instructions"
            "\n\n"
            "Inputs:\n"
            "All inputs have the respective instructions above them, but generally all the framework user has to do is to "
            "input a number to choose an option in the menus or to input some kind of data.\n"
            "In the case of wrong input given, the user is warned and is able to try again."
            "\n\n"
            "StreamZ:\n"
            "To create a streamz you only have to input the capacity number and create an admin (option 1).\n"
            "Then, to enter it and work with it just have to choose it in the main menu (option 2)."
            "\n\n"
            "Settings:\n"
            "Auto save-> if this is turned on, the framework will save all program data at the end, automatically. (turned on by default)\n"
            "Save-> used to save the program data manually.\n"
            "Import-> it imports a program data file."
            "\n\n";

    help_submenu =
            "\nStreamZ Help Instruction"
            "\n\n"
            "Create Streamer or Viewer:\n"
            "Only have to input the viewer's or streamer's data that is requested."
            "\n\n"
            "Choose Streamer or viewer:\n"
            "Just input the id of the streamer or viewer wanted. If any is created yet, it will warn the user."
            "\n\n"
            "Best Streams:\n"
            "Prints the bests streams and their information."
            "\n\n"
            "The next functionalities are all self explanatory, however here are their description."
            "\n\n"
            "Inside Streamer (after streamer chosen):\n"
            "Streamer info-> outputs the streamer's information.\n"
            "Start public stream-> starts a public stream and streamer can only have one stream at the time (private or public)\n"
            "Start private stream-> used to start a private stream\n"
            "Stop Stream-> stops a stream"
            "\n\n"
            "Inside Viewer (after viewer chosen):\n"
            "Viewer info-> outputs the user info\n"
            "Enter stream-> enters a stream (can only be at one at the time) and if stream is private, can only enter if it is allowed\n"
            "Exit stream-> used to exit the current stream it's in\n"
            "Like stream-> likes the stream it's in (can only like or dislike a stream and only once)\n"
            "Dislike stream-> dislikes the stream being watched\n"
            "Remove like-> removes the like given to a stream (can only be used if already liked)\n"
            "Remove dislike-> removes the dislike given to a stream (can only be used if already disliked)"
            "\n\n";
}

//the 3 following functions are used for inputs
void minAgeInput(unsigned &min_age) {
    bool in_min_age_input = true;

    while (in_min_age_input) {
        in_min_age_input = false;

        cout << "Input the minimum age: ";
        cin >> min_age;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cinFail()) {
            cout << endl << "Please input a number!!" << endl;
            in_min_age_input = true;
        }
    }
}

void passwordInput(string &password) {
    string password_strength;
    bool in_password_input = true;

    while (in_password_input) {
        cout
                << "The password needs need to be at least 6 characters long and have at least one upper case and one lower case letter."
                << endl;
        cout << "Enter the password: ";
        cin >> password;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!passwordStrength(password, password_strength)) in_password_input = true;
        else {
            in_password_input = false;
            cout << "Password accepted! ";
        }
        cout << "Your password is " << password_strength << endl << endl;
    }
}

void dateInput(Date &birthday, bool birth_date) {
    unsigned day, year, month;
    char sep;
    bool in_date_selection = true;  //loop of date input
    bool number_failed, separator_failed, ok; //used in checking date input (ok is used to prevent from next cin's to execute if previous fails)

    while (in_date_selection) {
        number_failed = false, separator_failed = false;
        ok = true;
        in_date_selection = false;


        if (birth_date) cout << "Enter birthday date in the format dd-mm-yyyy: ";
        else cout << "Enter the date in the format dd-mm-yyyy: ";

        if (ok) cin >> day;
        if (cinFail() && ok) {
            number_failed = true;
            ok = false;
        }
        if (ok) cin >> sep;
        if (sep != '-' && ok) {
            separator_failed = true;
            ok = false;
        }
        if (ok) cin >> month;
        if (cinFail() && ok) {
            number_failed = true;
            ok = false;
        }
        if (ok) cin >> sep;
        if (sep != '-' && ok) {
            separator_failed = true;
            ok = false;
        }
        if (ok) cin >> year;
        if (cinFail() && ok) {
            number_failed = true;
            ok = false;
        }

        if (separator_failed) {
            cout << "Expected '-' as separator!" << endl;
            in_date_selection = true;
            continue;
        }
        if (number_failed) {
            cout << "Input a number!!" << endl;
            in_date_selection = true;
            continue;
        }

        try {
            birthday = Date(day, month, year);
        } catch (InvalidDate &) {
            in_date_selection = true;
        }

        if (!in_date_selection && getCurrentDate() < birthday) in_date_selection = true;

        if (in_date_selection) cout << "Input date again, invalid date" << endl;
    }
}

//the following 4 functions are used in the users menus
void create_streamer(StreamZ *sz_selected) {
    string nickname, password;
    Date birthday;

    cout << "Enter the streamer's nickname: ";
    cin >> nickname;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    dateInput(birthday, true);

    passwordInput(password);

    bool exception_caught = true;

    try {
        sz_selected->addStreamer(nickname, birthday, password);
        exception_caught = false;

    } catch (NoMinimumAge&) {
        cout << "Unable to create streamer! No minimum age required:(" << endl;

    } catch (NameAlreadyInUse&) {
        cout << "Unable to create streamer! Username already used :(" << endl;
    }

    if(!exception_caught) cout << "Streamer created successfully!" << endl;
}

void create_viewer(StreamZ *sz_selected) {
    string nickname, password, inp;
    Date birthday;

    cout << "Enter the viewer's nickname: ";
    cin >> nickname;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    dateInput(birthday, true);

    passwordInput(password);

    bool exception_caught = true;

    try {
        sz_selected->addViewer(nickname, birthday, password);
        exception_caught = false;

    } catch (NoMinimumAge&) {
        cout << "Unable to create viewer! No minimum age required:(" << endl;

    } catch (NameAlreadyInUse&) {
        cout << "Unable to create viewer! Username already used :(" << endl;
    }

    if(!exception_caught) cout << "Viewer created successfully!" << endl;
}

void streamer_menu_loop(Menu streamer_menu, Streamer *s_selected, StreamZ *sz_selected) {
    bool streamer_loop = true;

    streamer_menu.changeTitle("Streamer " + s_selected->getName());

    while (streamer_loop) {
        streamer_menu.startMenu();

        switch (streamer_menu.getSelected()) {
            //streamer info
            case 0: {
                cout << s_selected->getInfo();
                break;
            }
                //start public stream
            case 1: {
                if (s_selected->isActive()) { //also in exceptions, but here too to prevent user from inputing if is already active
                    cout << "This streamer is already streaming!" << endl;
                    cout << "If you want to start a new one you have to stop this first!" << endl;
                    stopConsole();
                } else {
                    string title, lang;
                    unsigned min_age;

                    cout << "Input the stream's title: ";
                    cin.get();
                    getline(cin, title);
                    cout << "Input the stream's language: ";
                    cin >> lang;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    minAgeInput(min_age);

                    bool exception_caught = true;

                    try {
                        sz_selected->startPublicStream(s_selected, title, lang, min_age);
                        exception_caught = false;
                    } catch (AlreadyStreaming &) {
                        cout << "The streamer is already streaming!" << endl;
                    } catch (FullCapacity &) {
                        cout << "StreamZ has reached its full capacity of active streams: "
                             << sz_selected->getCapacity()
                             << endl;
                    } catch (InvalidLanguage &invalidL) {
                        cout << "User has inputted an invalid language: " << invalidL.getLanguage() << endl;
                    }

                    if (!exception_caught) cout << "Started Stream..." << endl;
                }
                break;
            }
                //start private stream
            case 2: {
                string title, lang, inp;
                unsigned min_age, id_selected;
                vector<unsigned> authorized_viewers;

                if (s_selected->isActive()) { //also in exceptions, but here too to prevent user from inputing if is already active
                    cout << "This streamer is already streaming!" << endl;
                    cout << "If you want to start a new one you have to stop this first!" << endl;
                    stopConsole();
                } else {
                    cout << "Input the stream's title: ";
                    cin.get();
                    getline(cin, title);
                    cout << "Input the stream's language: ";
                    cin >> lang;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    minAgeInput(min_age);

                    cout << "Input the authorized viewers by id: (to stop enter 'stop')" << endl << endl;

                    cout << "Viewers" << endl;

                    vector<Viewer *> viewers = sz_selected->getViewers();
                    vector<Viewer *>::const_iterator viewer;

                    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
                        cout << "Id: " << to_string((*viewer)->getID()) + "  ";
                        cout << "Name: " << (*viewer)->getName() << endl;
                    }

                    while (true) {
                        cout << "Input the id: ";
                        cin >> inp;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (inp == "stop") break;

                        try {
                            id_selected = stoi(inp);
                        } catch (invalid_argument &) {
                            cout << "Invalid input!" << endl;
                            continue;
                        }

                        if (find(authorized_viewers.begin(), authorized_viewers.end(), id_selected) !=
                            authorized_viewers.end())
                            cout << "Already inputted id" << endl;
                        else if (sz_selected->getViewerByID(id_selected) == nullptr)
                            cout << "Invalid viewer id inputted!" << endl;
                        else {
                            authorized_viewers.push_back(id_selected);
                            cout << "Successfully inputted viewer" << endl;
                        }
                    }

                    bool exception_caught = true;

                    try {
                        sz_selected->startPrivateStream(s_selected, title, lang, min_age, authorized_viewers);
                        exception_caught = false;

                    } catch (AlreadyStreaming &) {
                        cout << "The streamer is already streaming!" << endl;
                    } catch (FullCapacity &) {
                        cout << "StreamZ has reached is full capacity of active streams: " << sz_selected->getCapacity()
                             << endl;
                    } catch (InvalidLanguage &invalidL) {
                        cout << "User have inputted an invalid language: " << invalidL.getLanguage() << endl;
                    }

                    if (!exception_caught) cout << "Started Stream..." << endl;
                }
                break;
            }
                //stop stream
            case 3: {
                bool exception_caught = true;

                try {
                    sz_selected->stopStream(s_selected);
                    exception_caught = false;
                } catch (NotStreaming &) {
                    cout << "There is no stream to stop! Streamer is not streaming!" << endl;
                }

                if (!exception_caught) cout << "Stream stopped..." << endl;

                stopConsole();

                break;
            }
                //back
            case 4: {
                streamer_loop = false;
                break;
            }
        }
    }
}


void viewer_menu_loop(Menu viewer_menu, Viewer *v_selected, StreamZ *sz_selected, Menu viewer_interaction_menu) {
    viewer_menu.changeTitle("Viewer " + v_selected->getName());

    bool viewer_loop = true, inInput = true;
    bool interactions_loop = true;

    while (viewer_loop) {
        viewer_menu.startMenu();
        switch (viewer_menu.getSelected()) {
            //viewer info
            case 0: {
                cout << v_selected->getInfo();
                break;
            }
                //enter stream
            case 1: {
                if (v_selected->isActive()) {
                    cout << "This viewer is already in a stream!" << endl;
                    cout << "If you want to enter a new one you have to exit this one first!" << endl;
                    stopConsole();
                } else {
                    unsigned choice;

                    if (sz_selected->getNumActiveStreamers() == 0) {
                        cout << "There are no streams active right now" << endl;
                        stopConsole();
                    } else {
                        cout << "Active streams:" << endl << endl;
                        //sz_selected->printActiveStreams(); //TODO: change method
                        cout << endl << "Chose the stream you want to enter" << endl;
                        cout << "Enter the respective streamer id" << endl;

                        cin >> choice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (!cinFail()) {
                            Streamer *selection = sz_selected->getStreamerByID(choice);
                            if (selection == nullptr) {
                                cout << "Invalid streamer id inputted!" << endl;
                                break;
                            }
                            try {
                                v_selected->enterStream(selection);
                            }
                            catch (AlreadyViewing&) {
                                //TODO: Redundant?
                                break;
                            }
                            catch (NotStreaming&){
                                //TODO: Redundant?
                                break;
                            }
                            catch (UnauthorizedViewer&) {
                                cout << "This is a private stream. You are not authorized to enter it.";
                                break;
                            }
                            cout << "Entered stream successfully!" << endl;
                        } else {
                            cout << "No valid number inputted!" << endl;
                            break;
                        }
                    }
                }
                break;
            }
                //exit stream
            case 2: {
                if (!v_selected->isActive()) { //TODO: Remove and use exception?
                    cout << "This viewer is not in a stream!" << endl;
                } else {
                    try {
                        v_selected->exitStream();
                    }
                    catch(InactiveUser&){
                        cout << "This viewer is not in a stream!" << endl;
                        break;
                    }
                    cout << "Exiting stream successfully!" << endl;
                }
                stopConsole();
                break;
            }
            case 3: {

                viewer_interaction_menu.startMenu();

                while (interactions_loop) {

                    switch (viewer_interaction_menu.getSelected()) {
                        //like stream
                        case 0: {
                            if (!v_selected->isActive()) {
                                cout << "Not viewing any stream!" << endl;
                            } else {
                                try {
                                    v_selected->likeStream();
                                }
                                catch (AlreadyInteracted &) {
                                    cout << "You have already liked or disliked the stream!" << endl;
                                    break;
                                }
                                cout << "Liked the stream that viewer is watching!" << endl;
                            }
                            break;
                        }
                            //dislike stream
                        case 1: {
                            if (!v_selected->isActive()) {
                                cout << "Not viewing any stream!" << endl;
                            } else {
                                try {
                                    v_selected->dislikeStream();
                                }
                                catch (AlreadyInteracted &) {
                                    cout << "You have already liked or disliked the stream!" << endl;
                                    break;
                                }
                                cout << "Disliked the stream that viewer is watching!" << endl;
                            }
                            break;
                        }
                            //remove like
                        case 2: {
                            if (!v_selected->isActive()) {
                                cout << "Not viewing any stream!" << endl;
                            } else {
                                try {
                                    v_selected->remlikeStream();
                                }
                                catch (HasNotInteracted &) {
                                    cout << "You haven't liked the stream!" << endl;
                                    break;
                                }
                                cout << "Removed like to the stream that viewer is watching!" << endl;
                            }
                            break;
                        }
                            //remove dislike
                        case 3: {
                            if (!v_selected->isActive()) {
                                cout << "Not viewing any stream!" << endl;
                            } else {
                                try {
                                    v_selected->remdislikeStream();
                                }
                                catch (HasNotInteracted &) {
                                    cout << "You haven't disliked the stream!" << endl;
                                    break;
                                }
                                cout << "Removed dislike to the stream that viewer is watching!" << endl;
                            }
                            break;
                        }
                            //comments
                        case 4: {
                            string comment;

                            cout << "Input the comment you want to make: " << endl;
                            cout << " --> ";

                            cin.get();
                            getline(cin, comment);

                            try {
                                v_selected->comment(comment);
                            }
                            catch (InactiveUser &) {
                                cout << "Not viweing any stream!" << endl;
                                break;
                            }
                            catch (NotInPrivateStream &) {
                                cout << "You can't comment on a public stream! Only on private ones." << endl;
                                break;
                            }

                            cout << "Your comment has been successfully made!" << endl;

                            break;
                        }
                        case 5: {
                            interactions_loop = false;
                            break;
                        }
                    }
                }
                break;
            }
                //back
            case 4: {
                viewer_loop = false;
                break;
            }
        }
    }
}

/**
 * Main loop for interaction with the StreamZ framework
 */
void
streamzFramework() {
    bool auto_save = true;  //used to enable disable the auto_save (true by default)
    bool loop = true, sub_loop; //(loop) used in main loop // (sub_loop) used in other sub loops
    bool stats_loop, admin_bool = false; //access control

    vector<StreamZ *> streamz_vector;

    Menu main_menu("StreamZ Framework", 5);
    main_menu.changeOption(0, "Help");
    main_menu.changeOption(1, "Create StreamZ");
    main_menu.changeOption(2, "Choose StreamZ");
    main_menu.changeOption(3, "Settings");
    main_menu.changeOption(4, "Exit");

    Menu login_menu("Login", 3);
    login_menu.changeOption(0, "SignIn");
    login_menu.changeOption(1, "SignUp");
    login_menu.changeOption(2, "Back");

    Menu settings("Settings", 4);
    settings.changeOption(0, "Auto Save");
    settings.changeOption(1, "Save");
    settings.changeOption(2, "Import");
    settings.changeOption(3, "Back");

    Menu sub_menu("StreamZ admin menu default title", 7);
    sub_menu.changeOption(0, "Help");
    sub_menu.changeOption(1, "Create Streamer");
    sub_menu.changeOption(2, "Choose Streamer");
    sub_menu.changeOption(3, "Create Viewer");
    sub_menu.changeOption(4, "Choose Viewer");
    sub_menu.changeOption(5, "Statistics");
    sub_menu.changeOption(6, "Back");

    Menu streamer_menu("Streamer default title", 5);
    streamer_menu.changeOption(0, "Streamer Info");
    streamer_menu.changeOption(1, "Start public stream");
    streamer_menu.changeOption(2, "Start private stream");
    streamer_menu.changeOption(3, "Stop stream");
    streamer_menu.changeOption(4, "Back");

    Menu viewer_menu("Viewer default title", 5);
    viewer_menu.changeOption(0, "Viewer info");
    viewer_menu.changeOption(1, "Enter stream");
    viewer_menu.changeOption(2, "Exit stream");
    viewer_menu.changeOption(3, "Stream interactions");
    viewer_menu.changeOption(4, "Back");

    Menu stats_menu("Admin Statistics", 5);
    stats_menu.changeOption(0, "StreamZ statistics");
    stats_menu.changeOption(1, "Best streams");
    stats_menu.changeOption(2, "Best Streamer");
    stats_menu.changeOption(3, "Streams at a given time");
    stats_menu.changeOption(4, "Back");

    Menu viewer_interaction_menu("Viewer Streaming Interactions", 6);
    viewer_interaction_menu.changeOption(0, "Like stream");
    viewer_interaction_menu.changeOption(1, "Dislike stream");
    viewer_interaction_menu.changeOption(2, "Remove Like");
    viewer_interaction_menu.changeOption(3, "Remove Dislike");
    viewer_interaction_menu.changeOption(4, "Comment");
    viewer_interaction_menu.changeOption(5, "Back");

    string help_main_menu, help_submenu;

    help_strings(help_main_menu, help_submenu);

    while (loop) {
        admin_bool = false;
        main_menu.startMenu();

        switch (main_menu.getSelected()) {
            //help
            case 0: {
                cout << help_main_menu << endl;
                stopConsole();
                break;
            }
                //create streamz
            case 1: {
                string inp, nickname, password;
                Date birthday;
                bool inCreation = true;
                unsigned cap;  //streamz capacity

                while (inCreation) {
                    cout << "Enter StreamZ active streamers capacity: ";
                    cin >> cap;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (cinFail()) {
                        cout << endl << "Please input a number!!" << endl;
                        continue;
                    }
                    cout << "Enter admin name: ";
                    cin >> nickname;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    dateInput(birthday, true);

                    passwordInput(password);

                    auto *sz1 = new StreamZ(cap, nickname, birthday, password);

                    streamz_vector.push_back(sz1);

                    cout << "StreamZ created successfully!" << endl;

                    inCreation = false;
                }
                break;
            }
                //choose streamz
            case 2: {
                unsigned input;
                string nickname, password;

                if (streamz_vector.empty()) {
                    cout << "No StreamZ's created yet!" << endl;
                    stopConsole();
                } else {
                    vector<StreamZ *>::const_iterator streamz;
                    for (streamz = streamz_vector.begin(); streamz != streamz_vector.end(); streamz++) {
                        cout << "StreamZ " + to_string((*streamz)->getID()) + "  ";
                        cout << "Streamers: " + to_string((*streamz)->getNumStreamers()) + "  ";
                        cout << "Viewers: " + to_string((*streamz)->getNumViewers()) + "  ";
                        cout << "Capacity: " + to_string((*streamz)->getCapacity()) << endl;
                    }
                    cout << endl << "Select the StreamZ by its id" << endl;

                    inputChecker(input, streamz_vector.size());

                    StreamZ *sz_selected = streamz_vector.at(input);  //the vector must be ordered by id

                    sub_loop = true;

                    while (sub_loop) {

                        login_menu.startMenu();

                        switch (login_menu.getSelected()) {
                            //sign In
                            case 0: {
                                cout << "Enter the user's nick name: ";
                                cin >> nickname;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Enter the user's password: ";
                                cin >> password;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                if (!sz_selected->loginVerifier(nickname, password))
                                    cout << "User not found or incorrect password, incorrect input given!!";
                                else {
                                    User *user = sz_selected->getUserByName(nickname);

                                    Streamer *s_selected = dynamic_cast<Streamer *>(user);
                                    Viewer *v_selected = dynamic_cast<Viewer *>(user);

                                    if (s_selected != nullptr) {
                                        streamer_menu_loop(streamer_menu, s_selected, sz_selected);
                                    } else if (v_selected != nullptr) {
                                        viewer_menu_loop(viewer_menu, v_selected, sz_selected, viewer_interaction_menu);
                                    } else {
                                        admin_bool = true;
                                    }
                                    sub_loop = false;
                                }
                                break;
                            }
                                //sign Up
                            case 1: {
                                bool notCorrect = true;
                                string signUpChoice;

                                while (notCorrect) {
                                    cout << "Input s to register as streamer and v as viewer: ";
                                    cin >> signUpChoice;
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    if (signUpChoice == "s") {
                                        create_streamer(sz_selected);
                                        notCorrect = false;
                                    } else if (signUpChoice == "v") {
                                        create_viewer(sz_selected);
                                        notCorrect = false;
                                    } else {
                                        cout << "Please input one of the two:";
                                    }
                                }
                                break;
                            }
                                //exit
                            case 2: {
                                sub_loop = false;
                                break;
                            }
                        }
                    }

                    sub_menu.changeTitle("StreamZ " + to_string(sz_selected->getID()) + " Admin");

                    sub_loop = true;

                    while (sub_loop && admin_bool) {
                        sub_menu.startMenu();

                        switch (sub_menu.getSelected()) {
                            //help
                            case 0: {
                                cout << help_submenu << endl;
                                stopConsole();
                                break;
                            }
                                //create streamer
                            case 1: {
                                create_streamer(sz_selected);
                                break;
                            }
                                //choose streamer
                            case 2: {
                                unsigned input;

                                cout << "Streamers" << endl;
                                vector<Streamer *> streamers = sz_selected->getStreamers();
                                vector<Streamer *>::const_iterator streamer;
                                for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
                                    cout << "Id: " << to_string((*streamer)->getID()) + "  ";
                                    cout << "Name: " << (*streamer)->getName() << endl;
                                }
                                cout << endl << "Select the Streamer by its id" << endl;
                                cout << "--> ";
                                cin >> input;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                if (!cinFail()) {
                                    if (sz_selected->getStreamerByID(input) == nullptr) {
                                        cout << "Invalid streamer id inputted!" << endl;
                                        break;
                                    }
                                } else {
                                    cout << "No valid number inputted!" << endl;
                                    break;
                                }

                                Streamer *s_selected = sz_selected->getStreamerByID(
                                        input);

                                streamer_menu_loop(streamer_menu, s_selected, sz_selected);

                                break;
                            }
                                //create viewer
                            case 3: {

                                create_viewer(sz_selected);
                                break;
                            }
                                //choose viewer
                            case 4: {

                                unsigned input;

                                cout << "Viewers" << endl;
                                vector<Viewer *> viewers = sz_selected->getViewers();
                                vector<Viewer *>::const_iterator viewer;
                                for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
                                    cout << "Id: " << to_string((*viewer)->getID()) + "  ";
                                    cout << "Name: " << (*viewer)->getName() << endl;
                                }
                                cout << endl << "Select the Viewer by its id" << endl;
                                cout << "--> ";
                                cin >> input;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                if (!cinFail()) {
                                    if (sz_selected->getViewerByID(input) == nullptr) {
                                        cout << "Invalid viewer id inputted!" << endl;
                                        break;
                                    }
                                } else {
                                    cout << "No valid number inputted!" << endl;
                                    break;
                                }

                                Viewer *v_selected = sz_selected->getViewerByID(input);  //not treating exceptions yet

                                viewer_menu_loop(viewer_menu, v_selected, sz_selected, viewer_interaction_menu);

                                break;
                            }
                                //statistics
                            case 5: {
                                stats_loop = true;

                                while (stats_loop) {

                                    stats_menu.startMenu();

                                    switch (stats_menu.getSelected()) {
                                        //streamz statistics
                                        case 0: {
                                            Language lang;
                                            sz_selected->getNumCreatedStreams(lang);

                                            cout << "Most used languages: " << lang << endl;
                                            cout << "Total streams created: "
                                                 << to_string(sz_selected->getNumCreatedStreams()) << endl;
                                            cout << "Average views per stream: "
                                                 << to_string(sz_selected->getAverageViews()) << endl;
                                            break;
                                        }
                                            //best streams
                                        case 1: {
                                            cout << "Best streams: (upwards order)" << endl;

                                            vector<Stream *> best = sz_selected->getBestStreams();

                                            cout << "Most viewed streams: " << endl;

                                            for (int i = 0; i < 10; i++) {
                                                if (best.at(i) != nullptr) cout << best.at(i)->getInfo();
                                            }

                                            cout << "Most liked streams: " << endl;

                                            for (int i = 10; i < 20; i++) {
                                                if (best.at(i) != nullptr) cout << best.at(i)->getInfo();
                                            }

                                            stopConsole();

                                            break;
                                        }
                                            //best streamer
                                        case 2: {
                                            cout << "Best Streamer (with most views): " << endl;
                                            if (sz_selected->getMostViewedStreamer() != nullptr) {
                                                cout << sz_selected->getMostViewedStreamer()->getInfo() << endl;
                                            } else cout << "none" << endl;

                                            stopConsole();
                                            break;
                                        }
                                            //streams at a given time
                                        case 3: {
                                            Date date1, date2;

                                            cout << "Enter the period you want by entering two dates" << endl;

                                            cout << "Begin date-> ";
                                            dateInput(date1, false);
                                            cout << "End date-> ";
                                            dateInput(date2, false);

                                            cout << endl << "Streams created between " << date1 << " and " << date2
                                                 << endl << endl;

                                            cout << "Public streams: "
                                                 << to_string(sz_selected->getNumCreatedStreams(true, date1, date2))
                                                 << endl;

                                            cout << "Private streams: "
                                                 << to_string(sz_selected->getNumCreatedStreams(false, date1, date2))
                                                 << endl << endl;

                                            stopConsole();
                                            break;
                                        }
                                            //back
                                        case 4:
                                            stats_loop = false;
                                            break;
                                    }
                                }
                                break;
                            }
                                //back
                            case 6: {
                                sub_loop = false;
                                break;
                            }
                        }
                    }
                }
                break;
            }
                //settings
            case 3: {
                sub_loop = true;

                while (sub_loop) {
                    settings.startMenu();

                    switch (settings.getSelected()) {
                        //auto save
                        case 0: {
                            if (!auto_save)
                                cout << "Auto save is off" << endl;
                            else
                                cout << "Auto save is on" << endl;

                            cout << endl << "Enter 'c' to change its state and anything else to exit" << endl;
                            cout << "Input: ";

                            string off_or_on;
                            cin >> off_or_on;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            if (off_or_on == "c") auto_save = !auto_save;

                            else break;
                        }
                            //TODO: save (save all StreamZ instances? or only one?)
                        case 1: {
                            vector<StreamZ *>::iterator it;
                            for (it = streamz_vector.begin(); it != streamz_vector.end();) {
                                string filename = "StreamZ_" + to_string((*it)->getID()) + ".txt";
                                (*it)->save(filename);
                            }
                            break;
                        }
                        case 2: {
                            string filename;
                            cout << "The name of the file with the StreamZ information: ";
                            cin >> filename;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            try {
                                streamz_vector.emplace_back(new StreamZ(filename));
                            }
                            catch (InvalidFile &invf) {
                                cout << "\"" << invf.getFileName() << "\" failed to load. StreamZ not imported!\n\n";
                                break;
                            }
                            cout << "StreamZ instance imported successfully!\n\n";
                            break;
                        }
                            //back
                        case 3: {
                            sub_loop = false;
                            break;
                        }
                    }
                }
                break;
            }
                //exit
            case 4: {
                string choice;

                if (!auto_save) {
                    cout << "Don't forget to save if you want!" << endl;
                } else {
                    vector<StreamZ *>::iterator it;
                    for (it = streamz_vector.begin(); it != streamz_vector.end(); it++) {
                        string filename = "StreamZ_" + to_string((*it)->getID()) + ".txt";
                        (*it)->save(filename);
                    }
                    cout << "StreamZ's have been saved automatically." << endl;
                }
                cout << "Are you sure you want exit? (if yes enter 'y')" << endl;
                cout << "--> ";

                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (choice == "y") {
                    cout << "Thank you for using our framework!" << endl;
                    stopConsole();
                    loop = false;
                }
                break;
            }
        }
    }
    //deleting pointers allocated in streamz_vector
    vector<StreamZ *>::iterator it;
    for (it = streamz_vector.begin(); it != streamz_vector.end();) {
        delete *it;
    }
}
