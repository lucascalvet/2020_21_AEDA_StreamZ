#include "streamz_framework.h"
#include "exceptions.h"
#include <limits>
#include <algorithm>
#include <fstream>

using namespace std;

/*
 * Framework for interacting with the StreamZ platform in the console.
 */

//encapsulated in a function to maintain readability of the code where they are used and avoiding having them global
//help instructions to main framework menu and to the admin, the other menus are all self explanatory
//adapted to 120 characters clion console max character size
void help_strings(string &help_main_menu, string &help_submenu) {
    help_main_menu =
            "\nFramework Help Instructions"
            "\n---------------------------"
            "\n\n"
            "Inputs:\n"
            "All inputs have the respective instructions above them, but generally all the framework user has to do is to input\n"
            "a number to choose an option in the menus or to input some kind of data.\n"
            "In the case of wrong input given, the user is warned and is able to try again."
            "\n\n"
            "StreamZ:\n"
            "To create a streamz you only have to input the capacity number and create an admin (option 1).\n"
            "Then, to enter it and work with it just have to choose it in the main menu (option 2).\n"
            "After a streamz selected you will have to enter the credentials of admin or create other user and sign in."
            "\n\n"
            "Settings:\n"
            "Auto save-> if this is turned on, the framework will save all program data at the end, automatically.\n"
            "(turned on by default)\n"
            "Save-> used to save the program data manually.\n"
            "Import-> it imports a program data file.\n"
            "For more detailed information about the program architecture consult the doxygen files.";

    help_submenu =
            "\nAdmin Help Instruction"
            "\n----------------------"
            "\n\n"
            "The admin has access to everything, and can enter in all accounts by choosing them\n"
            "and it can also create more users."
            "\n\n"
            "Create streamer or viewer:\n"
            "Only have to input the viewer's or streamer's data, that is requested."
            "\n\n"
            "Choose streamer or viewer:\n"
            "Just input the id of the streamer or viewer wanted. If any is created yet, it will warn the admin."
            "\n\n"
            "Statistics:\n"
            "Submenu with several options, with statistical information of the platform."
            "\n\n"
            "The other functionalities are all self explanatory, as well has inside the streamer and viewer menus.";
}

//banner displayed only once when framework executed, at the beginning
void banner(){
    ifstream banner_file;
    string banner_line;

    banner_file.open("banner.txt");

    if (banner_file.fail()) cout << "Unable to show initial banner" << endl; //doesn't throw exception, because it's not essential
    else{
        while(banner_file.peek() != EOF){
            getline(banner_file, banner_line);
            cout << banner_line;
        }
    }
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
        cout << "-----------------------------------" << endl;
        cout << "The password has to have least one" << endl;
        cout << "upper case and one lower case letter" << endl;
        cout << "And be at least 6 characters long." << endl;
        cout << "-----------------------------------" << endl;
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

        cout << endl;  //used to prevent console reformatting

        if (!in_date_selection && getCurrentDate() < birthday) in_date_selection = true;

        if (in_date_selection) cout << "Input date again, invalid date" << endl;
    }
}

//displays best streams to console
void printBestStreams(StreamZ* sz_selected){
    cout << "Best streams: (downwards order)" << endl << endl;

    vector<Stream *> best = sz_selected->getBestStreams();

    cout << "Most viewed streams: " << endl << endl;

    for (int i = 0; i < 10; i++) {
        if (best.at(i) != nullptr) cout << "--> " << best.at(i)->getInfo();
    }

    cout << endl << "Most liked streams: " << endl << endl;

    for (int i = 10; i < 20; i++) {
        if (best.at(i) != nullptr) cout << "--> " << best.at(i)->getInfo();
    }

    stopConsole();
}

//used by users to search streams by language or by min_age
void searchStreams(StreamZ *sz_selected){
    string choice;
    bool lang_bool = false, min_age_bool = false, all_bool = false;
    Language lang;
    unsigned min_age;
    vector<Streamer*> streams;

    if(sz_selected->getNumActiveStreamers() == 0){
        cout << "No active streams available right now!" << endl;
    }
    else {
        cout << "Search Engine: " << endl;

        cout << "-> Input 'all' to list all active streams" << endl;
        cout << "-> Input 'l' to search by language" << endl;
        cout << "-> Input 'm' to search by minimum age" << endl;
        cout << "-> Input 'lm' to search using both" << endl;

        cout << "Input: ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  //ignore bad input

        if (choice == "l" || choice == "L") lang_bool = true;
        else if (choice == "m" || choice == "M") min_age_bool = true;
        else if (choice == "lm" || choice == "LM") {
            min_age_bool = true;
            lang_bool = true;
        } else if (choice == "all") {
            all_bool = true;
        } else cout << "Wrong input given, no valid option!" << endl;


        if (lang_bool) {
            cout << "Choose a language in the predefined format (ex. en)" << endl;
            cout << "Input: ";
            cin >> lang;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  //ignore bad input
        }
        if (min_age_bool) {
            cout << "Choose the minimum age upper limit until you want to search" << endl;
            cout << "Input: ";
            cin >> min_age;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  //ignore bad input
        }

        if (min_age_bool && lang_bool) streams = sz_selected->getStreams(lang, min_age);
        else if (lang_bool) streams = sz_selected->getStreams(lang);
        else if (min_age_bool) streams = sz_selected->getStreams("", min_age);
        else if (all_bool) streams = sz_selected->getStreams();

        if(streams.empty()) cout << "No streams active with those parameters!" << endl;
        else if(min_age_bool || lang_bool || all_bool) StreamZ::printStreams(streams);
    }
    stopConsole();
}

//the following 4 functions are used in the users menus
void createStreamer(StreamZ *sz_selected) {
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

void createViewer(StreamZ *sz_selected) {
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

void streamerMenuLoop(Menu streamer_menu, Streamer *s_selected, StreamZ *sz_selected) {
    bool streamer_loop = true;

    streamer_menu.changeTitle("Streamer " + s_selected->getName());

    while (streamer_loop) {
        streamer_menu.startMenu();

        switch (streamer_menu.getSelected()) {
            //streamer info
            case 0: {
                cout << s_selected->getInfo() << endl;
                vector<Stream *> history = s_selected->getHistory();
                if (history.empty()){
                    cout << "No streams in history.";
                    break;
                }
                cout << "Streaming history:" << endl;
                vector<Stream *>::const_iterator stream;
                for (stream = history.begin(); stream != history.end(); stream++){
                    cout << "--> " << (*stream)->getInfo();
                }
                cout << endl;
                stopConsole();
                break;
            }
            //search streams
            case 1:{
                searchStreams(sz_selected);
                break;
            }
                //start public stream
            case 2: {
                if (s_selected->isActive()) { //also in exceptions, but here too to prevent user from inputing if is already active
                    cout << "This streamer is already streaming!" << endl;
                    cout << "If you want to start a new one, stop this first!" << endl;
                    stopConsole();
                } else {
                    string title, lang;
                    unsigned min_age;

                    cout << "Input the stream's title: ";
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
            case 3: {
                string title, lang, inp;
                unsigned min_age, id_selected;
                vector<unsigned> authorized_viewers;

                if (s_selected->isActive()) { //also in exceptions, but here too to prevent user from inputing if is already active
                    cout << "This streamer is already streaming!" << endl;
                    cout << "If you want to start a new one you have to stop this first!" << endl;
                    stopConsole();
                } else {
                    cout << "Input the stream's title: ";
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
            case 4: {
                bool exception_caught = true;

                try {
                    sz_selected->stopStream(s_selected);
                    exception_caught = false;
                } catch (InactiveUser &) {
                    cout << "There is no stream to stop! Streamer is not streaming!" << endl;
                }

                if (!exception_caught) cout << "Stream stopped..." << endl;

                stopConsole();

                break;
            }
            //best streams
            case 5:{
                printBestStreams(sz_selected);
                break;
            }
                //Sell Product
            case 6:{
                unsigned price, stock;

                cout << "Enter the price of your product: ";
                cin >> price;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cinFail()){
                    cout << "Please input a number!" << endl;
                    break;
                }

                cout << "Enter the stock you have to sell: ";
                cin >> stock;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cinFail()){
                    cout << "Please input a number!" << endl;
                    break;
                }

                sz_selected->sellProduct(s_selected, price, stock);

                cout << "You're product has been successfully sold!" << endl;

                break;
            }
                //back
            case 7: {
                streamer_loop = false;
                break;
            }
        }
    }
}


void viewerMenuLoop(Menu viewer_menu, Viewer *v_selected, StreamZ *sz_selected, Menu viewer_interaction_menu) {
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
            //search streams
            case 1:{
                searchStreams(sz_selected);
                break;
            }
                //enter stream
            case 2: {
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
                        sz_selected->printStreams(sz_selected->getActiveStreamers());
                        cout << endl << "Choose the stream you want to enter" << endl;
                        cout << "Enter the respective streamer id: ";
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
                            catch (UnauthorizedViewer&) { //only this two exceptions are needed to catch, to avoid redundancy
                                cout << "This is a private stream. You are not authorized to enter it." << endl;
                                break;
                            }
                            catch (NoMinimumAge&) {
                                cout << "You don't have the minimum age to enter this stream!" << endl;
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
            case 3: {
                if (!v_selected->isActive()) {
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
            //viewer interactions
            case 4: {
                interactions_loop = true;
                while (interactions_loop) {
                    viewer_interaction_menu.startMenu();

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
                                    v_selected->remLikeStream();
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
                                    v_selected->remDislikeStream();
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
                        //back
                        case 5: {
                            interactions_loop = false;
                            break;
                        }
                    }
                }
                break;
            }
            //best streams
            case 5:{
                printBestStreams(sz_selected);
                break;
            }
                //Make Order
            case 6:{
                if(sz_selected->getProducts().empty()) cout << "There are no products for sell!" << endl;
                else {
                    unsigned prod_id, quan, pri;

                    sz_selected->printAvailableProducts();

                    cout << "Enter an id: ";
                    cin >> prod_id;

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (!cinFail()) {
                        cout << "Enter the quantity wanted: ";
                        cin >> quan;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (cinFail()){
                            cout << "Please input a number!" << endl;
                            break;
                        }

                        cout << "Enter the priority from 1 to 5: ";
                        cin >> pri;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (cinFail() || pri < 1 || pri > 5){
                            cout << "Please input a number!" << endl;
                            break;
                        }

                        try{
                            sz_selected->makeOrder(v_selected, quan, pri, prod_id);
                        }
                        catch (QuantityOverTheStock &e){
                            cout << "Please choose a quantity below the stock!" << endl;
                            break;
                        }
                        catch (ExceededMaxQuantityPerPurchase &e){
                            cout << "You exceeded the max quantity per purchase! Quantity has to be below 10" << endl;
                            break;
                        }
                        catch(NotEnoughCapital &e){
                            cout << "You don't have enough money to do this purchase!" << endl;
                            break;
                        }
                        catch (OrderAlreadyExists &e){
                            cout << "You have already made a similar order!" << endl;
                            break;
                        }
                        cout << "You're order has been successfully done and the money it has been taken from your wallet!" << endl;
                    }
                    else{
                        cout << "Not inputted a number!!" << endl;
                    }
                }
                break;
            }
                //Delete Order
            case 7:{
                unsigned quan, pri;

                cout << "Enter the quantity you have inputted: ";
                cin >> quan;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cinFail()){
                    cout << "Please input a number!" << endl;
                    break;
                }

                cout << "Enter the priority from 1 to 5: ";
                cin >> pri;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cinFail() || pri < 1 || pri > 5){
                    cout << "Please input a number!" << endl;
                    break;
                }

                try{
                    sz_selected->deleteOrder(v_selected, quan, pri);
                }
                catch (OrderDoesNotExist &e) {
                    cout << "Please input the correct data!" << endl;
                    break;
                }
                cout << "You're order has been successfully deleted and 50% of the money spent was given to back to you!" << endl;

            break;
            }
                //Donate
            case 8:{
                unsigned id_choice;

                sz_selected->printStreamers();

                cout << "Chose the streamer inputting the id: ";
                cin >> id_choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cinFail()){
                    cout << "Please input a number!" << endl;
                    break;
                }

                unsigned amount, eval;

                cout << "Enter the amount to donate: ";
                cin >> amount;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cinFail()){
                    cout << "Please input a number!" << endl;
                    break;
                }

                cout << "Enter the evaluation of the streamer from 1 to 5: ";
                cin >> eval;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cinFail() || eval < 1 || eval > 5){
                    cout << "Please input a number from 1 to 5!" << endl;
                    break;
                }

                if(v_selected->getWalletAmount() < amount){
                    cout << "Please ensure that you have enough money in your e-wallet" << endl;
                    break;
                }
                else{
                    v_selected->cashWithdraw(amount);
                    sz_selected->makeDonation(sz_selected->getStreamerByID(id_choice), amount, eval);
                }
                cout << "You have successfully made the donation!" << endl;

                break;
            }
                //back
            case 9: {
                viewer_loop = false;
                break;
            }
        }
    }
}

/**
 * Main loop for interaction with the StreamZ framework
 */
void streamzFramework() {
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

    Menu streamer_menu("Streamer default title", 8);
    streamer_menu.changeOption(0, "Streamer Info");
    streamer_menu.changeOption(1, "Search Streams");
    streamer_menu.changeOption(2, "Start public stream");
    streamer_menu.changeOption(3, "Start private stream");
    streamer_menu.changeOption(4, "Stop stream");
    streamer_menu.changeOption(5, "Best streams");
    streamer_menu.changeOption(6, "Sell Product"); //Part 2
    streamer_menu.changeOption(7, "Back");

    Menu viewer_menu("Viewer default title", 10);
    viewer_menu.changeOption(0, "Viewer info");
    viewer_menu.changeOption(1, "Search Streams");
    viewer_menu.changeOption(2, "Enter stream");
    viewer_menu.changeOption(3, "Exit stream");
    viewer_menu.changeOption(4, "Stream interactions");
    viewer_menu.changeOption(5, "Best streams");
    viewer_menu.changeOption(6, "Make Order"); //Part 2
    viewer_menu.changeOption(7, "Delete Order"); //Part 2
    viewer_menu.changeOption(8, "Donate"); //Part 2
    viewer_menu.changeOption(9, "Back");

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

    banner(); //only showed at the beginning

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

                cout << endl;
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
                                        streamerMenuLoop(streamer_menu, s_selected, sz_selected);
                                    } else if (v_selected != nullptr) {
                                        viewerMenuLoop(viewer_menu, v_selected, sz_selected, viewer_interaction_menu);
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
                                        createStreamer(sz_selected);
                                        notCorrect = false;
                                    } else if (signUpChoice == "v") {
                                        createViewer(sz_selected);
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
                                createStreamer(sz_selected);
                                break;
                            }
                                //choose streamer
                            case 2: {
                                unsigned input;

                                cout << endl << "Streamers" << endl;
                                vector<Streamer *> streamers = sz_selected->getStreamers();
                                vector<Streamer *>::const_iterator streamer;
                                for (streamer = streamers.begin(); streamer != streamers.end(); streamer++) {
                                    cout << "Id: " << to_string((*streamer)->getID()) + "  ";
                                    cout << "Name: " << (*streamer)->getName();
                                    if (!(*streamer)->getAccountStatus()) cout << "   INACTIVE";
                                    cout << endl;
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

                                streamerMenuLoop(streamer_menu, s_selected, sz_selected);

                                break;
                            }
                                //create viewer
                            case 3: {

                                createViewer(sz_selected);
                                break;
                            }
                                //choose viewer
                            case 4: {

                                unsigned input;

                                cout << endl << "Viewers" << endl;
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

                                viewerMenuLoop(viewer_menu, v_selected, sz_selected, viewer_interaction_menu);

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
                                            Language lang = sz_selected->getMostUsedLanguage();

                                            cout << "StreamZ Capital: " << to_string(sz_selected->getStreamzCapital()) << endl;

                                            cout << "Most used languages: " << lang << endl;

                                            cout << "Total streams created: "
                                                 << to_string(sz_selected->getNumCreatedStreams()) << endl;
                                            cout << "Average views per stream: "
                                                 << to_string(sz_selected->getAverageViews()) << endl;
                                            break;
                                        }
                                            //best streams
                                        case 1: {
                                            printBestStreams(sz_selected);
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

                                            unsigned public_streams = sz_selected->getNumCreatedStreams(true, date1, date2);
                                            unsigned private_streams = sz_selected->getNumCreatedStreams(false, date1, date2);

                                            if(public_streams == 0 && private_streams == 0) cout << "Any streams at the given time!" << endl;
                                            else {
                                                cout << "Streams created between " << date1 << " and " << date2 << endl
                                                     << endl;

                                                cout << "Public streams: " << public_streams << endl;

                                                cout << "Private streams: " << private_streams << endl;
                                            }
                                            cout << endl;
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

                            break;
                        }
                        case 1: {
                            vector<StreamZ *>::iterator it;
                            if(streamz_vector.empty()) cout << "Nothing to save yet!" << endl;
                            else {
                                for (it = streamz_vector.begin(); it != streamz_vector.end(); it++) {
                                    string filename = "StreamZ_" + to_string((*it)->getID()) + ".txt";
                                    (*it)->save(filename);
                                }
                                cout << "Everything saved!" << endl;
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
                    try {
                        for (it = streamz_vector.begin(); it != streamz_vector.end(); it++) {
                            string filename = "StreamZ_" + to_string((*it)->getID()) + ".txt";
                            (*it)->stopAllStreams();
                            (*it)->save(filename);
                        }
                    }
                    catch (InvalidFile&) {
                        cout << "Error saving to files!";
                        break;
                    }
                    cout << "StreamZ's have been saved automatically with name StreamZ_[respective ID]." << endl;
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
    for (it = streamz_vector.begin(); it != streamz_vector.end(); it++) {
        delete *it;
    }
}
