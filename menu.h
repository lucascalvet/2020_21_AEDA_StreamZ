#ifndef STREAMZ_AEDA_2020_21_MENU_H
#define STREAMZ_AEDA_2020_21_MENU_H

#include <vector>
#include <iostream>
#include <string>

bool numberInputFail();

void stopConsole();

using namespace std;

class Menu{
public:
    string title;
    bool inMenu = true;
    unsigned options;
    unsigned selected = 0;
    vector<string> optionNaming;
    Menu(string title, unsigned options);
    //~Menu();
    void changeOption(unsigned index, string opName);
    void changeTitle(string newTitle);
    void addOption(string opName);
    void startMenu();
};

#endif //STREAMZ_AEDA_2020_21_MENU_H
