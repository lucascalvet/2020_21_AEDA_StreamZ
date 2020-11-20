#include "menu.h"
#include "streamz.h"
#include "streamz_framework.h"
#include "exceptions.h"
#include <limits>
#include <algorithm>

using namespace std;

string help_main_menu =
        "\nFramework Help Instructions"
        "\n\n"
        "Inputs:\n"
        "All inputs have the respective instructions above them, but generally all the framework user has to do is to "
        "input a number to choose an option in the menus or to input some kind of data.\n"
        "In the case of wrong input given, the user is warned and is able to try again."
        "\n\n"
        "StreamZ:\n"
        "To create a streamz you only have to input the capacity number and create an admin (option 1).\n"
        "Then, to enter it and work with it just go back and choose it (option 2)."
        "\n\n"
        "Settings:\n"
        "Auto save-> if this is turned on, the framework will save all program data at the end, automatically.\n"
        "Save-> used to save the program data manually.\n"
        "Import-> it imports a program data file."
        "\n\n";

string help_submenu =
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
        "The next functionalities are all self explanatories, however here are their description."
        "\n\n"
        "Inside Streamer (after streamer chosen):\n"
        "Streamer info-> outputs the streamer's information.\n"
        "Start public stream-> starts a public stream and streamer can only have one stream at the time (private or public)\n"
        "Start private stream-> used to start a private stream\n"
        "Stop Stream-> stops a stream"
        "\n\n"
        "Inside Viewer (after viewer chosen):\n"
        "Viewer info-> outputs the user info"
        "Enter stream-> enters a stream (can only be at one at the time) and if stream is private, can only enter if it is allowed\n"
        "Exit stream-> used to exit the current stream it's in\n"
        "Like stream-> likes the stream it's in (can only like or dislike a stream and only once)\n"
        "Dislike stream-> dislikes the stream being watched\n"
        "Remove like-> removes the like given to a stream (can only be used if already liked)\n"
        "Remove dislike-> removes the dislike given to a stream (can only be used if already disliked)"
        "\n\n";

//5 functions with respective code encapsulated in order to avoid code repetition
void dateInput(Date& birthday){
    unsigned day, year, month;
    char sep;
    bool in_date_selection = true;

    while(in_date_selection) {
        in_date_selection = false;
        cout << "Enter the birthday in the format dd-mm-yy: ";
        cin >> day >> sep >> month >> sep >> year;// not yet implemented way to obey format

        try {
            birthday = Date(day, month, year);
        } catch (InvalidDate &) {
            in_date_selection = true;
        }

        if(!in_date_selection && getCurrentDate() < birthday) in_date_selection = true;
        if(cin.fail()){
            in_date_selection = true;
            cin.clear(); //reset failbit
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  //ignore bad input
        }

        if(in_date_selection) cout << "Input date again, invalid date" << endl;
    }
}

void create_streamer(StreamZ* sz_selected){
    string nickname, password;
    Date birthday;

    cout << "Enter the streamer nickname: ";
    cin >> nickname;  //not checking equal names yet

    dateInput(birthday);

    cout << "Enter the password: ";
    cin >> password;

    if(!sz_selected->addStreamer(nickname, birthday, password)) cout << "Unable to create streamer! :( Username already used or not minimum age" << endl;
    else cout << "Streamer created successfully, go back to sign in" << endl;
}

void create_viewer(StreamZ* sz_selected){
    string inp;

    string nickname, password;
    Date birthday;

    cout << "Enter the viewer nickname: ";
    cin >> nickname;

    dateInput(birthday);

    cout << "Enter the password: ";
    cin >> password;

    if(!sz_selected->addViewer(nickname, birthday, password)) cout << "Unable to create viewer! :(" << endl;
    else cout << "Viewer created successfully, go back to sign in!" << endl;
}

void streamer_menu_loop(Menu streamerMenu, Streamer* s_selected, StreamZ* sz_selected){
    bool streamer_loop = true;

    streamerMenu.changeTitle("Streamer " + s_selected->getName());

    while (streamer_loop) {
        streamerMenu.startMenu();

        switch (streamerMenu.getSelected()) {
            //streamer info
            case 0: {
                cout << s_selected->getInfo();
                break;
            }
                //start public stream
            case 1: {
                if (s_selected->isActive()) {
                    cout << "This streamer is already streaming!" << endl;
                    cout << "If you want to start a new one you have to stop this first!" << endl;
                    stopConsole();
                } else {
                    string title, lang;
                    unsigned min_age;
                    cout << "Input the stream title: ";
                    cin >> title;
                    cout << "Input the stream language: ";
                    cin >> lang;
                    cout << "Input the minimum age: ";
                    cin >> min_age;
                    cout << "Starting Stream..." << endl;
                    try {
                        sz_selected->startPublicStream(s_selected, title, lang, min_age);
                    } catch (AlreadyStreaming&) {
                        cout << "The streamer is already streaming!" << endl;
                    } catch (FullCapacity&) {
                        cout << "StreamZ has reached is full capacity of active streams: " << sz_selected->getCapacity() << endl;
                    } catch (InvalidLanguage& invalidL) {
                        cout << "User have inputed an invalid language: " << invalidL.getLanguage() << endl;
                    }
                }
                break;
            }
                //start private stream
            case 2: {
                string title, lang;
                unsigned min_age;
                string inp;
                unsigned id_selected;
                vector<unsigned> authorized_viewers;

                if(s_selected->isActive()){
                    cout << "This streamer is already streaming!" << endl;
                    cout << "If you want to start a new one you have to stop this first!" << endl;
                    stopConsole();
                } else {
                    cout << "Input the stream title: ";
                    cin >> title;
                    cout << "Input the stream language: ";
                    cin >> lang;
                    cout << "Input the minimum age: ";
                    cin >> min_age;
                    cout << "Input the autorized viewers by id: (to stop enter 'stop')" << endl << endl;
                    cout << "Viewers" << endl;

                    vector<Viewer*> viewers = sz_selected->getViewers();
                    vector<Viewer*>::const_iterator viewer;

                    for (viewer = viewers.begin(); viewer != viewers.end(); viewer++) {
                        cout << "Id: " << to_string((*viewer)->getID()) + "  ";
                        cout << "Name: " << (*viewer)->getName() << endl;
                    }

                    while(true){
                        cout << "Input the id: ";
                        cin >> inp;

                        if (inp == "stop") break;

                        try {
                            id_selected = stoi(inp);
                        } catch (invalid_argument&) {
                            cout << "Invalid input!" << endl;
                            continue;
                        }

                        if(find(authorized_viewers.begin(), authorized_viewers.end(), id_selected) != authorized_viewers.end())
                            cout << "Already inputed id" << endl;
                        else if(sz_selected->getViewerByID(id_selected) == nullptr)
                            cout <<"Invalid viewer id inputed!" << endl;
                        else {
                            authorized_viewers.push_back(id_selected);
                            cout << "Successfully inputed viewer" << endl;
                        }
                    }

                    try {
                        sz_selected->startPrivateStream(s_selected, title, lang, min_age, authorized_viewers);
                    } catch (AlreadyStreaming&) {
                        cout << "The streamer is already streaming!" << endl;
                    } catch (FullCapacity&) {
                        cout << "StreamZ has reached is full capacity of active streams: " << sz_selected->getCapacity() << endl;
                    } catch (InvalidLanguage& invalidL) {
                        cout << "User have inputed an invalid language: " << invalidL.getLanguage() << endl;
                    }

                    cout << "Starting Stream..." << endl;
                }
                break;
            }
                //stop stream
            case 3:{
                if (!s_selected->isActive()) {
                    cout << "There is now stream to stop!" << endl;
                } else {
                    cout << "Stoping stream..." << endl;
                    sz_selected->stopStream(s_selected);
                    cout << "Stream stoped!" << endl;
                }
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

void viewer_menu_loop(Menu viewerMenu, Viewer* v_selected, StreamZ* sz_selected){
    viewerMenu.changeTitle("Viewer " + v_selected->getName());

    bool viewer_loop = true, inInput = true;

    while (viewer_loop) {
        viewerMenu.startMenu();
        switch (viewerMenu.getSelected()) {
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
                    cout << "Active streams:" << endl << endl;
                    sz_selected->printActiveStreams();
                    cout << endl << "Chose the stream you want to enter" << endl;
                    cout << "Enter the respective streamer id" << endl;

                    cin >> choice;

                    if(!cinFail()) {
                        if(sz_selected->getStreamerByID(choice) == nullptr) {
                            cout << "Invalid streamer id inputed!" << endl;
                            break;
                        }
                    }
                    else{
                        cout << "No valid number inputed!" << endl;
                        break;
                    }

                    sz_selected->enterStream(sz_selected->getStreamerByID(choice), v_selected);
                    cout << "Entered stream successfully!" << endl;
                }
                break;
            }
                //exit stream
            case 2: {
                if (!v_selected->isActive()) {
                    cout << "This viewer is not in a stream!" << endl;
                } else {
                    unsigned choice;
                    sz_selected->exitStream(v_selected);
                    cout << "Exiting stream successfully!" << endl;
                }
                stopConsole();
                break;
            }
                //like stream
            case 3: {
                if (!v_selected->isActive()) {
                    cout << "Not viweing any stream!" << endl;
                } else {
                    if (sz_selected->likeStream(v_selected))
                        cout << "Liked the stream that viewer is watching!" << endl;
                    else
                        cout << "You have already liked or disliked the stream!" << endl;
                }
                break;
            }
                //dislike stream
            case 4: {
                if (!v_selected->isActive()) {
                    cout << "Not viweing any stream!" << endl;
                } else {
                    if (sz_selected->dislikeStream(v_selected))
                        cout << "Disliked the stream that viewer is watching!" << endl;
                    else
                        cout << "You have already liked os disliked the stream!" << endl;
                }
                break;
            }
                //remove like
            case 5: {
                if (!v_selected->isActive()) {
                    cout << "Not viweing any stream!" << endl;
                } else {
                    if (sz_selected->remlikeStream(v_selected))
                        cout << "Removed like to the stream that viewer is watching!"
                             << endl;
                    else
                        cout << "You haven't liked the stream!" << endl;
                }
                break;
            }
                //remove dislike
            case 6: {
                if (!v_selected->isActive()) {
                    cout << "Not viweing any stream!" << endl;
                } else {
                    if (sz_selected->remdislikeStream(v_selected))
                        cout << "Removed dislike to the stream that viewer is watching!"
                             << endl;
                    else
                        cout << "You haven't disliked the stream!" << endl;
                }
                break;
            }
                //comments
            case 7: {
                string comment;

                cout << "Input the comment you want to make: " << endl;
                cout <<" --> ";

                cin.get();
                getline(cin, comment);

                try {
                    v_selected->comment(comment);
                }
                catch (InactiveUser&) {
                    cout << "Not viweing any stream!" << endl;
                    break;
                }
                catch (NotInPrivateStream&) {
                    cout << "You can't comment on a public stream! Only on private ones." << endl;
                    break;
                }

                cout << "Your comment has been successfully made!" << endl;

                break;
            }
                //back
            case 8: {
                viewer_loop = false;
                break;
            }
        }
    }
}

/**
 * Main loop for interaction with the StreamZ framework
 */
void streamz_framework() { //TODO: Allow stream titles with more than one word. Gets stuck when choosing user, because of ID!
    unsigned cap;
    bool in_input = true;  //used for to loop input while wrong one is submitted with inputChecker function
    bool auto_save = false, setngs;  // (auto_save) used to enable disable the auto_save // (setngs) used with settings menu
    bool loop = true, sub_loop; //each one is used to in it's respective menu
    bool login_loop; //login loop
    bool admin_bool = false; //access control
    string choice;
    vector<StreamZ *> streamz_vector;

    Menu mainMenu("StreamZ Framework", 5);
    mainMenu.changeOption(0, "Help");
    mainMenu.changeOption(1, "Create StreamZ");
    mainMenu.changeOption(2, "Choose StreamZ");
    mainMenu.changeOption(3, "Settings");
    mainMenu.changeOption(4, "Exit");

    Menu loginMenu("Login", 3);
    loginMenu.changeOption(0, "SignIn");
    loginMenu.changeOption(1, "SignUp");
    loginMenu.changeOption(2, "Back");

    Menu settings("Settings", 4);
    settings.changeOption(0, "Auto Save");
    settings.changeOption(1, "Save");
    settings.changeOption(2, "Import");
    settings.changeOption(3, "Back");

    Menu subMenu("StreamZ default title", 7);
    subMenu.changeOption(0, "Help");
    subMenu.changeOption(1, "Create Streamer");
    subMenu.changeOption(2, "Choose Streamer");
    subMenu.changeOption(3, "Create Viewer");
    subMenu.changeOption(4, "Choose Viewer");
    subMenu.changeOption(5, "Best streams");
    subMenu.changeOption(6, "Back");

    Menu streamerMenu("Streamer default title", 5);
    streamerMenu.changeOption(0, "Streamer Info");
    streamerMenu.changeOption(1, "Start public stream");
    streamerMenu.changeOption(2, "Start private stream");
    streamerMenu.changeOption(3, "Stop stream");
    streamerMenu.changeOption(4, "Back");

    Menu viewerMenu("Viewer default title", 9);
    viewerMenu.changeOption(0, "Viewer info");
    viewerMenu.changeOption(1, "Enter stream");
    viewerMenu.changeOption(2, "Exit stream");
    viewerMenu.changeOption(3, "Like stream");
    viewerMenu.changeOption(4, "Dislike stream");
    viewerMenu.changeOption(5, "Remove Like");
    viewerMenu.changeOption(6, "Remove Dislike");
    viewerMenu.changeOption(7, "Comment");
    viewerMenu.changeOption(8, "Back");

    while (loop) {
        admin_bool = false;
        mainMenu.startMenu();

        switch (mainMenu.getSelected()) {
            //help
            case 0: {
                cout << help_main_menu << endl;
                stopConsole();
                break;
            }
                //create streamz
            case 1: {
                string inp;
                string nickname, password;
                Date birthday;
                bool inCreation = true;

                while (inCreation) {
                    cout << "Enter StreamZ active streamers capacity: " << endl;
                    cin >> cap;
                    if (cinFail()) {
                        cout << "Please input a number!!" << endl;
                        continue;
                    }
                    cout << "Enter admin name: ";
                    cin >> nickname;

                    dateInput(birthday);

                    cout << "Enter the password: ";
                    cin >> password;

                    StreamZ *sz1 = new StreamZ(cap, nickname, birthday, password);
                    streamz_vector.push_back(sz1);

                    cout << "StreamZ created successfully, go back to work with it!" << endl;
                    cout << "To create another one input anything, to go back input 'e'" << endl;
                    cout << "Input: ";
                    cin >> inp;
                    if (inp == "e")
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

                    login_loop = true;

                    while(login_loop) {

                    loginMenu.startMenu();

                        switch (loginMenu.getSelected()) {
                            //sign In
                            case 0: {
                                cout << "Enter the user nick name: ";
                                cin >> nickname;
                                cout << "Enter the user password: ";
                                cin >> password;
                                if (!sz_selected->loginVerifier(nickname, password))
                                    cout << "User not found, incorrect input given!!";
                                else {
                                    User *user = sz_selected->getUserByName(nickname);

                                    Streamer *s_selected = dynamic_cast<Streamer *>(user);
                                    Viewer *v_selected = dynamic_cast<Viewer *>(user);

                                    if (s_selected != nullptr) {
                                        streamer_menu_loop(streamerMenu, s_selected, sz_selected);
                                    } else if (v_selected != nullptr) {
                                        viewer_menu_loop(viewerMenu, v_selected, sz_selected);
                                    } else {
                                        admin_bool = true;
                                    }
                                    login_loop = false;
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
                                login_loop = false;
                                break;
                            }
                        }
                    }

                    subMenu.changeTitle("StreamZ " + to_string(sz_selected->getID()));

                    sub_loop = true;

                    while (sub_loop && admin_bool) {
                        subMenu.startMenu();

                        switch (subMenu.getSelected()) {
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
                                cout << endl << "Select the Streamer by it's id" << endl;

                                cout << "--> ";
                                cin >> input;

                                if(!cinFail()) {
                                    if(sz_selected->getStreamerByID(input) == nullptr) {
                                        cout << "Invalid streamer id inputed!" << endl;
                                        break;
                                    }
                                }
                                else{
                                    cout << "No valid number inputed!" << endl;
                                    break;
                                }

                                Streamer *s_selected = sz_selected->getStreamerByID(
                                        input);

                                streamer_menu_loop(streamerMenu, s_selected, sz_selected);

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
                                cout << endl << "Select the Viewer by it's id" << endl;

                                cout << "--> ";
                                cin >> input;

                                if(!cinFail()) {
                                    if(sz_selected->getViewerByID(input) == nullptr) {
                                        cout << "Invalid viewer id inputed!" << endl;
                                        break;
                                    }
                                }
                                else{
                                    cout << "No valid number inputed!" << endl;
                                    break;
                                }

                                Viewer *v_selected = sz_selected->getViewerByID(input);  //not treating exceptions yet

                                viewer_menu_loop(viewerMenu, v_selected, sz_selected);

                                break;
                            }
                                //best streams
                            case 5: {
                                cout << "Best streams" << endl << endl;
                                vector<Stream *> best_streams = sz_selected->getBestStreams();
                                vector<Stream *>::const_iterator stream;
                                for (stream = best_streams.begin(); stream != best_streams.end(); stream++) {
                                    cout << "Stream " + (*stream)->getTitle() << endl;
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
                setngs = true;

                while (setngs) {
                    settings.startMenu();

                    switch (settings.getSelected()) {
                        //auto save
                        case 0: {
                            if (!auto_save)
                                cout << "Auto save is off" << endl;
                            else
                                cout << "Auto save is on" << endl;
                            cout << endl << "Enter 'c' to change it's state and anything else to exit" << endl;
                            cout << "Input: ";
                            string off_or_on;
                            cin >> off_or_on;
                            if (off_or_on == "c" && auto_save)
                                auto_save = false;
                            else if (off_or_on == "c" && !auto_save)
                                auto_save = true;
                            break;
                        }
                            //TODO: save (save all StreamZ instances? or only one?)
                        case 1: {
                            vector<StreamZ*>::iterator it;
                            for(it = streamz_vector.begin(); it != streamz_vector.end();){
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
                                streamz_vector.emplace_back(new StreamZ(filename)); //TODO: if it fails the id still goes up. Messes up when choosing a stream
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
                            setngs = false;
                            break;
                        }
                    }
                }
                break;
            }
                //exit
            case 4: {
                if (!auto_save) {
                    cout << "Don't forget to save if you want!" << endl;
                } else {
                    vector<StreamZ*>::iterator it;
                    for(it = streamz_vector.begin(); it != streamz_vector.end();){
                        //(*it)->save();
                    }
                    cout << "Log file has been saved automatically." << endl;
                }
                cout << "Are you sure you want exit? (if yes enter 'y')" << endl;
                cout << "--> ";
                cin >> choice;

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
