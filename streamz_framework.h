//
// Created by sergi on 09/11/2020.
//
#include <string>
#ifndef STREAMZ_AEDA_2020_21_STREAMZ_FRAMEWORK_H
#define STREAMZ_AEDA_2020_21_STREAMZ_FRAMEWORK_H

std::string help_main_menu =
        "\nFramework Help Instructions"
        "\n\n"
        "Inputs:\n"
        "All inputs have the respective instructions above them, but generally all the framework user has to do is to "
        "input a number to choose an option in the menus or to input some kind of data.\n"
        "In the case of wrong input given, the user is warned and is able to try again."
        "\n\n"
        "StreamZ:\n"
        "To create a streamz you only have to input the capacity number (option 1).\n"
        "Then, to enter it and work with it just go back and choose it (option 2)."
        "\n\n"
        "Settings:\n"
        "Auto save-> if this is turned on, the framework will save all program data at the end, automatically.\n"
        "Save-> used to save the program data manually.\n"
        "Import-> it imports a program data file."
        "\n\n";

std::string help_submenu =
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

void streamz_framework();

#endif //STREAMZ_AEDA_2020_21_STREAMZ_FRAMEWORK_H
