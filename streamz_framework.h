//
// Created by sergi on 09/11/2020.
//
#include <string>
#ifndef STREAMZ_AEDA_2020_21_STREAMZ_FRAMEWORK_H
#define STREAMZ_AEDA_2020_21_STREAMZ_FRAMEWORK_H

std::string help_main_menu =
        "Framework Help Instructions"
        "\n\n"
        "Inputs:\n"
        "All inputs have the respective instructions above them"
        "but generally all the framework user has to do is to "
        "input a number to choose an option in the menus or"
        "or to input some kind of data.\n"
        "In case of wrong input the user is warned and can try"
        " again."
        "\n\n"
        "StreamZ:\n"
        "To create a streamz you only have to input the capacity number (option 1).\n"
        "Then to enter it and work with it just go back and choose it (option 2)."
        "\n\n"
        "Settings:\n"
        "Auto save-> if this is turned on the framework will save all program data "
        "at the end automatically.\n"
        "Save-> used to save the program data manually.\n"
        "Import-> it imports a program data file."
        "\n\n";

std::string help_submenu =
        "StreamZ Help Instruction"
        "\n\n"
        "Create Streamer or viewer:\n"
        "Only have to input the viewer's or streamer's data that is requested.\n\n"
        "Choose Streamer or viewer:\n"
        "Just input the id of the streamer or viewer wanted. If any is created yet it will warn the user.\n\n"
        "Best Stream:\n"
        "Prints the bests streams and their information.\n\n"
        "Inside Streamer (after streamer choosed):\n"
        ""
        ""
        ""
        "Inside Viewer (after viewer choosed):\n"
        ""
        ""
        "";

void streamz_framework();

#endif //STREAMZ_AEDA_2020_21_STREAMZ_FRAMEWORK_H
