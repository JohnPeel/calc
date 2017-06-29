#include "NumericalMenu.h"
#include <limits>
#include <iostream>

NumericalMenu::NumericalMenu(): cancelText("Cancel"), errorText("Error!"), prompt("Choose an option:"), shouldRepeat(true) {
    // Stud
}

int NumericalMenu::addOption(std::string option) {
    options.push_back(option);
    return (int)options.size();
}

void NumericalMenu::setCancelText(std::string cancelText) {
    this->cancelText = cancelText;
}

void NumericalMenu::setErrorText(std::string errorText) {
    this->errorText = errorText;
}

void NumericalMenu::setPrompt(std::string prompt) {
    this->prompt = prompt;
}

void NumericalMenu::setRepeatPromptOnError(bool shouldRepeat) {
    this->shouldRepeat = shouldRepeat;
}

void NumericalMenu::printPrompt() const {
    std::cout << prompt << std::endl << std::endl;
    for (size_t i = 0; i < options.size(); i++)
        std::cout << i + 1 << " - " << options[i] << std::endl;
    std::cout << options.size() + 1 << " - " << cancelText << std::endl << std::endl;
}

int NumericalMenu::run() const {
    int result = 0;
    printPrompt();

    while ((result < 1) || (result > size())) {
        std::cin >> result;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (result == size() + 1)
            return -1;

        if ((result < 1) || (result > size())) {
            std::cout << std::endl << errorText << std::endl << std::endl;
            if (shouldRepeat)
                printPrompt();
            continue;
        }
    }

    return result;
}

int NumericalMenu::size() const {
    return (int)options.size();
}