#ifndef STREAMZ_MENU_H
#define STREAMZ_MENU_H

#include <vector>
#include <string>

bool cinFail();

void stopConsole();

void inputChecker(unsigned &selected, unsigned options);

class Menu {
private:
    std::string title;
    unsigned options;
    unsigned selected = 0;
    std::vector<std::string> optionNaming;
public:
    Menu(const std::string &title, unsigned options);
    unsigned int getSelected() const;
    void changeOption(unsigned index, const std::string &opName);
    void changeTitle(const std::string &newTitle);
    void addOption(const std::string &opName);
    void startMenu();
};

#endif //STREAMZ_MENU_H
