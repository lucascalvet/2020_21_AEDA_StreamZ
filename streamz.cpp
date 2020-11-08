#include "streamz.h"
#include <iostream>
#include "menu.h"

using namespace std;

int StreamZ::counter = 0;

StreamZ::StreamZ(unsigned capacity) {
    id = counter++;
    this->capacity = capacity;
}

StreamZ::~StreamZ() {
}

bool StreamZ::startStream(Streamer *streamer, string title, Language lang, unsigned min_age){
    if(!streamer->isActive() && getActiveStreams() <= capacity){
        Stream s(title, lang, min_age);
        streamer->s = &s;
        return true;
    }
    else{
        return false;  //already has a stream
    }
}

bool StreamZ::stopStream(Streamer* streamer){
    if (streamer->isActive()){
        streamer->s = NULL;
        return true;
    }
    else
        return false;
}

unsigned StreamZ::getActiveStreams() const {
    unsigned counter = 0;
    for(int i = 0; i < streamers.size(); i++){
        if(streamers.at(i)->s != NULL)
            counter++;
    }
    return counter;
}

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

bool StreamZ::exitStream(Viewer *v){
    if(!v->isActive())
        cout << "User is not viewing any stream!" << endl;
    else
        v->s = NULL;  //exiting stream
        v->alreadyLiked = false;
        v->alreadyDisliked = false;
}

bool StreamZ::addStreamer(Streamer* s) {
    streamers.push_back(s);
}

bool StreamZ::addViewer(Viewer* v) {
    viewers.push_back(v);
}

bool StreamZ::likeStream(Viewer *v) {
    if(v->alreadyLiked || v->alreadyDisliked)
        return false;
    else
        v->s->addLike();
    return true;
}

bool StreamZ::dislikeStream(Viewer *v) {
    if(v->alreadyDisliked || v->alreadyLiked)
        return false;
    else
        v->s->addDislike();
    return true;
}

bool StreamZ::remlikeStream(Viewer *v) {
    if(v->alreadyLiked) {
        v->s->remLike();
        v->alreadyLiked = true;
        return true;
    }
    else
        return false;
}

bool StreamZ::remdislikeStream(Viewer *v) {
    if(v->alreadyDisliked) {
        v->s->remDislike();
        v->alreadyDisliked = false;
        return true;
    }
    else
        return false;
}

unsigned StreamZ::getStreamers() const {
    return streamers.size();
}

unsigned StreamZ::getViewers() const {
    return viewers.size();
}

Streamer *StreamZ::getStreamerByID(unsigned id) {
    for(int i = 0; i < getStreamers(); i++){
        if(streamers.at(i)->getID() == id)
            return streamers.at(i);
    }
    return nullptr;
}

Viewer *StreamZ::getViewerByID(unsigned id) {
    for(int i = 0; i < getViewers(); i++){
        if(viewers.at(i)->getID() == id)
            return viewers.at(i);
    }
    return nullptr;
}

void StreamZ::printActiveStreams() {
    for(int i = 0; i < streamers.size(); i++){
        if(streamers.at(i)->isActive())
            cout << "Streamer id: " << to_string(streamers.at(i)->getID()) << "   Stream: " << streamers.at(i)->s->getTitle() << endl;
    }
}

void streamz_framework() {
    unsigned cap;
    bool auto_save = false, setngs;
    bool loop = true, sub_loop, streamer_loop, viewer_loop;
    string choice;
    vector<StreamZ*> streamz_vector;

    Menu mainMenu("StreamZ Framework", 5);
    mainMenu.changeOption(0, "Help");
    mainMenu.changeOption(1, "Create StreamZ");
    mainMenu.changeOption(2, "Choose StreamZ");
    mainMenu.changeOption(3, "Settings");
    mainMenu.changeOption(4, "Exit");

    Menu settings("Settings", 4);
    settings.changeOption(0,"Auto Save");
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

    while(loop) {
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
                    cout << "Input: ";
                    cin >> input;

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
                                    cin >> nickname;

                                    cout << "Enter the streamer birthday in the format dd-mm-yy: ";
                                    cin >> date;

                                    Streamer *s1 = new Streamer(nickname, birthday);
                                    sz_selected->addStreamer(s1);
                                    cout << "Streamer created successfully, go back to work with it!" << endl;
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

                                cout << "Streamers" << endl;

                                for (int i = 0; i < sz_selected->getStreamers(); i++) {
                                    cout << "Id: " + to_string(sz_selected->streamers.at(i)->getID()) + "  ";
                                    cout << "Name: " + sz_selected->streamers.at(i)->getName() << endl;
                                }
                                cout << endl << "Select the Streamer by it's id" << endl;
                                cout << "Input: ";
                                cin >> input;

                                Streamer *s_selected = sz_selected->getStreamerByID(
                                        input);  //not treating exceptions yet

                                streamerMenu.changeTitle("Streamer " + s_selected->getName());

                                streamer_loop = true;

                                while (streamer_loop) {
                                    streamerMenu.startMenu();

                                    switch (streamerMenu.selected) {
                                        case 0: {
                                            s_selected->printInfo();
                                            break;
                                        }
                                        case 1: {
                                            if(s_selected->isActive()){
                                                cout << "This streamer is already streaming!" << endl;
                                                cout << "If you want to start a new one you have to stop this first!" << endl;
                                                stopConsole();
                                            }
                                            else {
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
                                            if(!s_selected->isActive()){
                                                cout << "There is now stream to stop!" << endl;
                                            }
                                            else{
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

                                    Viewer *v1 = new Viewer(nickname, birthday);
                                    sz_selected->addViewer(v1);
                                    cout << "Viewer created successfully, go back to work with it!" << endl;
                                    cout << "To create another one input anything, to go back input 'e'" << endl;
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
                                cout << "Input: ";
                                cin >> input;

                                Viewer *v_selected = sz_selected->getViewerByID(input);  //not treating exceptions yet

                                viewerMenu.changeTitle("Viewer " + v_selected->getName());

                                viewer_loop = true;

                                while (viewer_loop) {
                                    viewerMenu.startMenu();
                                    switch (viewerMenu.selected) {
                                        case 0: {
                                            v_selected->printInfo();
                                            break;
                                        }
                                        case 1: {
                                            if(v_selected->isActive()){
                                                cout << "This viewer is already in a stream!" << endl;
                                                cout << "If you want to enter a new one you have to exit this one first!" << endl;
                                                stopConsole();
                                            }
                                            else {
                                                unsigned choice;
                                                cout << "Active streams:" << endl << endl;
                                                sz_selected->printActiveStreams();
                                                cout << endl << "Chose the stream you want to enter" << endl;
                                                cout << "Enter the respective streamer id" << endl;
                                                cout << "Input: ";
                                                cin >> choice;
                                                numberInputFail();
                                                sz_selected->enterStream(sz_selected->getStreamerByID(choice), v_selected);  //not treating exceptions
                                                cout << "Entered stream successfully!" << endl;
                                            }
                                            break;
                                        }
                                        case 2: {
                                            if(!v_selected->isActive()){
                                                cout << "This viewer is not in a stream!" << endl;
                                            }
                                            else {
                                                unsigned choice;
                                                sz_selected->exitStream(v_selected);
                                                cout << "Exintig stream successfully!" << endl;
                                            }
                                            stopConsole();
                                            break;
                                        }
                                        case 3: {
                                            if(!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            }
                                            else{
                                                if(sz_selected->likeStream(v_selected))
                                                    cout << "Liking the stream that viewer is watching!" << endl;
                                                else
                                                    cout << "You have already liked os disliked the stream!" << endl;
                                            }
                                            break;
                                        }
                                        case 4: {
                                            if(!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            }
                                            else{
                                                if(sz_selected->dislikeStream(v_selected))
                                                    cout << "Disliking the stream that viewer is watching!" << endl;
                                                else
                                                    cout << "You have already liked os disliked the stream!" << endl;
                                            }
                                            break;
                                        }
                                        case 5: {
                                            if(!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            }
                                            else{
                                                if(sz_selected->remlikeStream(v_selected))
                                                    cout << "Removing like to the stream that viewer is watching!" << endl;
                                                else
                                                    cout << "You haven't liked the stream!" << endl;
                                            }
                                            break;
                                        }
                                        case 6: {
                                            if(!v_selected->isActive()) {
                                                cout << "Not viweing any stream!" << endl;
                                            }
                                            else{
                                                if(sz_selected->remdislikeStream(v_selected))
                                                    cout << "Removing dislike to the stream that viewer is watching!" << endl;
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
            case 3:{
                setngs = true;

                while(setngs){
                    settings.startMenu();

                    switch(settings.selected){
                        case 0:{
                            if(!auto_save)
                                cout << "Auto save is off" << endl;
                            else
                                cout << "Auto save is on" << endl;
                            cout << endl << "Enter 'c' to change it's state" << endl;
                            cout << "Input: ";
                            string off_or_on;
                            cin >> off_or_on;
                            if(off_or_on == "c" && auto_save)
                                auto_save = false;
                            else if(off_or_on == "c" && !auto_save)
                                auto_save = true;
                            break;
                        }
                        case 1:{
                            break;
                        }
                        case 2:{
                            break;
                        }
                        case 3:{
                            setngs = false;
                            break;
                        }
                    }
                }
                break;
            }
            case 4: {
                if(!auto_save){
                    cout << "Don't forget to save if you want!" << endl;
                }
                else{
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
