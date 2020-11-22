#ifndef STREAMZ_FRAMEWORK_H
#define STREAMZ_FRAMEWORK_H

#include "menu.h"
#include "streamz.h"
#include <string>

void help_strings(std::string &help_main_menu, std::string &help_submenu);

void banner();

void minAgeInput(unsigned &min_age);

void passwordInput(std::string &password);

void dateInput(Date &birthday, bool birth_date);

void printBestStreams(StreamZ* sz_selected);

void searchStreams(StreamZ *sz_selected);

void createStreamer(StreamZ *sz_selected);

void createViewer(StreamZ *sz_selected);

void streamerMenuLoop(Menu streamer_menu, Streamer *s_selected, StreamZ *sz_selected);

void viewerMenuLoop(Menu viewer_menu, Viewer *v_selected, StreamZ *sz_selected, Menu viewer_interaction_menu);

void streamzFramework();

#endif //STREAMZ_FRAMEWORK_H
