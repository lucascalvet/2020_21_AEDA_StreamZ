#include "menu.h"

using namespace std;

bool numberInputFail() { //checks if user didn't input a number
    if (!cin) {
        cin.clear(); //reset failbit
        cin.ignore(10000, '\n');  //ignore bad input
        return true;
    }
    return false;
}

//checks if user didn't input a number or a number in a determinated range
void inputChecker(unsigned &selected, unsigned options, bool &inMenu) {
    cout << "Input: ";
    while (inMenu) {
        cin >> selected;

        if (numberInputFail()) {
            cout << "Please input a number!" << endl;
            cout << "Input: ";
        } else if (selected < options && selected >= 0)
            inMenu = false;
        else {
            cout << "Input a number in the options range!" << endl;
            cout << "Input: ";
        }
    }
    inMenu = true;
}

//stops the console until user presses anything
void stopConsole() {
    /*
    std::string stop;
    getline(std::cin, stop);
     */
    system("pause");
}

Menu::Menu(const string &title, unsigned options) {
    this->title = title;
    this->options = options;
    for (int i = 0; i < options; i++) {
        optionNaming.emplace_back("Default");
    }
}

unsigned int Menu::getSelected() const {
    return selected;
}

void Menu::changeOption(unsigned int index, const string &opName) {
    if (index > options) {
        cout << "ERROR" << endl;
    } else {
        optionNaming[index] = to_string(index) + ": " + opName;
    }
}

void Menu::changeTitle(const string &newTitle) {
    title = newTitle;
}

void Menu::addOption(const string &opName) {
    optionNaming.push_back(opName);
    options++;
}

void Menu::startMenu() {
    //gotoxy(0,0);
    cout << endl << string(title.size(), '*') << endl;
    cout << title << endl;
    cout << string(title.size(), '*') << endl << endl;


    for (int i = 0; i < options; i++) {
        cout << optionNaming[i] << endl;
    }
    inputChecker(selected, options, inMenu);

    //what to do for each option is coded next
}
