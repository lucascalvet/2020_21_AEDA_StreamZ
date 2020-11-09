//
// Created by sergi on 09/11/2020.
//
#include "menu.h"
#include "streamz.h"

/**
 * Main loop for interaction with the StreamZ framework
 */
void streamz_framework() {
    unsigned cap;
    bool inInput = true;  //used for to loop input while wrong one is submitted with inputChecker function
    bool auto_save = false, setngs;  // (auto_save) used to enable disable the auto_save // (setngs) used with settings menu
    bool loop = true, sub_loop, streamer_loop, viewer_loop; //each one is used to in it's respective menu
    string choice;
    vector<StreamZ *> streamz_vector;

    Menu mainMenu("StreamZ Framework", 5);
    mainMenu.changeOption(0, "Help");
    mainMenu.changeOption(1, "Create StreamZ");
    mainMenu.changeOption(2, "Choose StreamZ");
    mainMenu.changeOption(3, "Settings");
    mainMenu.changeOption(4, "Exit");

    Menu settings("Settings", 4);
    settings.changeOption(0, "Auto Save");
    settings.changeOption(1, "Save");
    settings.changeOption(2, "Import");
    settings.changeOption(3, "Back");

    Menu subMenu("StreamZ default title", 7);
    subMenu.changeOption(0, "Help");
    subMenu.changeOption(1, "Create Streamer");
    subMenu.changeOption(2, "Choose Streamer");  //this must show if the streamers are active or not
    subMenu.changeOption(3, "Create Viewer");
    subMenu.changeOption(4, "Choose Viewer");
    subMenu.changeOption(5, "Best streams");
    subMenu.changeOption(6, "Back");

    Menu streamerMenu("Streamer default title", 4);
    streamerMenu.changeOption(0, "Streamer Info");
    streamerMenu.changeOption(1, "Start stream");
    streamerMenu.changeOption(2, "Stop stream");
    streamerMenu.changeOption(3, "Back");

    Menu viewerMenu("Viewer default title", 8);
    viewerMenu.changeOption(0, "Viewer info");
    viewerMenu.changeOption(1, "Enter stream");
    viewerMenu.changeOption(2, "Exit stream");
    viewerMenu.changeOption(3, "Like stream");
    viewerMenu.changeOption(4, "Dislike stream");
    viewerMenu.changeOption(5, "Remove Like");
    viewerMenu.changeOption(6, "Remove Dislike");
    viewerMenu.changeOption(7, "Back");

    while (loop) {
        mainMenu.startMenu();

        switch (mainMenu.selected) {
            case 0: {
                cout << "\nhelp instructions for main menu here" << endl;
                stopConsole();
                break;
            }
            case 1: {
                string inp;
                bool inCreation = true;

                while (inCreation) {
                    cout << "Enter StreamZ active streamers capacity: " << endl;
                    cin >> cap;
                    if (numberInputFail()) {
                        cout << "Please input a number!!" << endl;
                        continue;
                    }

                    StreamZ *sz1 = new StreamZ(cap);
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
            case 2: {
                unsigned input;

                if (streamz_vector.size() == 0) {
                    cout << "No StreamZ's created yet!" << endl;
                    stopConsole();
                } else {

                    for (int i = 0; i < streamz_vector.size(); i++) {
                        cout << "StreamZ " + to_string(streamz_vector.at(i)->id) + "  ";
                        cout << "Streamers: " + to_string(streamz_vector.at(i)->getStreamers()) + "  ";
                        cout << "Viewers: " + to_string(streamz_vector.at(i)->getViewers()) + "  ";
                        cout << "Capacity: " + to_string(streamz_vector.at(i)->capacity) << endl;
                    }
                    cout << endl << "Select the StreamZ by it's id" << endl;
                    inputChecker(input, streamz_vector.size(), inInput);

                    StreamZ *sz_selected = streamz_vector.at(input);  //the vector must be ordered by id

                    subMenu.changeTitle("StreamZ " + to_string(sz_selected->id));

                    sub_loop = true;

                    while (sub_loop) {
                        subMenu.startMenu();

                        switch (subMenu.selected) {
                            case 0: {
                                cout << "\nhelp instructions for sub menu here" << endl;
                                stopConsole();
                                break;
                            }
                            case 1: {
                                string inp;
                                bool inCreation = true;

                                while (inCreation) {
                                    string nickname, date;
                                    Date birthday;

                                    cout << "Enter the streamer nickname: " << endl;
                                    cin >> nickname;  //not checking equal names yet

                                    cout << "Enter the streamer birthday in the format dd-mm-yy: ";
                                    cin >> date;  // not yet implemented way to obey format

                                    sz_selected->addStreamer(nickname, birthday);
                                    cout << "Streamer created successfully, go back to work with it!" << endl;
                                    cout << "To go back input 'e', to create another one input anything else" << endl;
                                    cout << "Input: ";
                                    cin >> inp;
                                    if (inp == "e")
                                        inCreation = false;
                                }
                                break;
                            }
                            case 2: {
                                unsigned input;

                                cout << "Streamers" << endl;

                                for (int i = 0; i < sz_selected->getStreamers(); i++) {
                                    cout << "Id: " + to_string(sz_selected->streamers.at(i)->getID()) + "  ";
                                    cout << "Name: " + sz_selected->streamers.at(i)->getName() << endl;
                                }
                                cout << endl << "Select the Streamer by it's id" << endl;
                                inputChecker(input, sz_selected->getStreamers(), inInput);

                                Streamer *s_selected = sz_selected->getStreamerByID(input);  //not treating exceptions yet

                                streamerMenu.changeTitle("Streamer " + s_selected->getName());

                                streamer_loop = true;

                                while (streamer_loop) {
                                    streamerMenu.startMenu();

                                    switch (streamerMenu.selected) {
                                        case 0: {
                                            cout << s_selected->getInfo();
                                            break;
                                        }
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
                                                sz_selected->startStream(s_selected, title, lang, min_age);
                                            }
                                            break;
                                        }
                                        case 2: {
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
                                        case 3: {
                                            streamer_loop = false;
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                            case 3: {
                                string inp;
                                bool inCreation = true;

                                while (inCreation) {
                                    string nickname, date;
                                    Date birthday;

                                    cout << "Enter the viewer nickname: " << endl;
                                    cin >> nickname;

                                    cout << "Enter the viewer birthday in the format dd-mm-yy: ";
                                    cin >> date;

                                    sz_selected->addViewer(nickname, birthday);
                                    cout << "Viewer created successfully, go back to work with it!" << endl;
                                    cout << "To go back input 'e', to create another one input anything else" << endl;
                                    cout << "Input: ";
                                    cin >> inp;
                                    if (inp == "e")
                                        inCreation = false;
                                }
                                break;
                            }
                            case 4: {
                                unsigned input;

                                cout << "Viewers" << endl;

                                for (int i = 0; i < sz_selected->getViewers(); i++) {
                                    cout << "Id: " + to_string(sz_selected->viewers.at(i)->getID()) + "  ";
                                    cout << "Name: " + sz_selected->viewers.at(i)->getName() << endl;
                                }
                                cout << endl << "Select the Viewer by it's id" << endl;
                                inputChecker(input, sz_selected->getViewers(), inInput);

                                Viewer *v_selected = sz_selected->getViewerByID(input);  //not treating exceptions yet

                                viewerMenu.changeTitle("Viewer " + v_selected->getName());

                                viewer_loop = true;

                                while (viewer_loop) {
                                    viewerMenu.startMenu();
                                    switch (viewerMenu.selected) {
                                        case 0: {
                                            cout << v_selected->getInfo();
                                            break;
                                        }
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
                                                inputChecker(choice, sz_selected->getStreamers(), inInput);
                                                numberInputFail();
                                                sz_selected->enterStream(sz_selected->getStreamerByID(choice), v_selected);  //not treating exceptions
                                                cout << "Entered stream successfully!" << endl;
                                            }
                                            break;
                                        }
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
                                        case 3: {
                                            if (!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            } else {
                                                if (sz_selected->likeStream(v_selected))
                                                    cout << "Liking the stream that viewer is watching!" << endl;
                                                else
                                                    cout << "You have already liked os disliked the stream!" << endl;
                                            }
                                            break;
                                        }
                                        case 4: {
                                            if (!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            } else {
                                                if (sz_selected->dislikeStream(v_selected))
                                                    cout << "Disliking the stream that viewer is watching!" << endl;
                                                else
                                                    cout << "You have already liked os disliked the stream!" << endl;
                                            }
                                            break;
                                        }
                                        case 5: {
                                            if (!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            } else {
                                                if (sz_selected->remlikeStream(v_selected))
                                                    cout << "Removing like to the stream that viewer is watching!"
                                                         << endl;
                                                else
                                                    cout << "You haven't liked the stream!" << endl;
                                            }
                                            break;
                                        }
                                        case 6: {
                                            if (!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            } else {
                                                if (sz_selected->remdislikeStream(v_selected))
                                                    cout << "Removing dislike to the stream that viewer is watching!"
                                                         << endl;
                                                else
                                                    cout << "You haven't disliked the stream!" << endl;
                                            }
                                            break;
                                        }
                                        case 7: {
                                            viewer_loop = false;
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                            case 5: {
                                cout << "Best streams" << endl << endl;
                                for (int i = 0; i < sz_selected->best_streams.size(); i++) {
                                    cout << "Stream " + sz_selected->best_streams.at(i)->getTitle() << endl;
                                }
                                break;
                            }
                            case 6: {
                                sub_loop = false;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 3: {
                setngs = true;

                while (setngs) {
                    settings.startMenu();

                    switch (settings.selected) {
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
                        case 1: {
                            break;
                        }
                        case 2: {
                            break;
                        }
                        case 3: {
                            setngs = false;
                            break;
                        }
                    }
                }
                break;
            }
            case 4: {
                if (!auto_save) {
                    cout << "Don't forget to save if you want!" << endl;
                } else {
                    //save files here
                    cout << "Log file has been saved automatically." << endl;
                }
                cout << "Are you sure you want exit? (if yes enter 'y')" << endl;
                cout << "Input: ";
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
}