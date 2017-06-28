#include "NumericalMenu.h"

NumericalMenu::NumericalMenu(): cancelText("Cancel"), errorText("Error!"), prompt("Choose an option:"), shouldRepeat(true) {
    // Stud
}

int NumericalMenu::addOption(string option) {
    options.push_back(option);
    return (int)options.size();
}

void NumericalMenu::setCancelText(string cancelText) {
    this->cancelText = cancelText;
}

void NumericalMenu::setErrorText(string errorText) {
    this->errorText = errorText;
}

void NumericalMenu::setPrompt(string prompt) {
    this->prompt = prompt;
}

void NumericalMenu::setRepeatPromptOnError(bool shouldRepeat) {
    this->shouldRepeat = shouldRepeat;
}

void NumericalMenu::printPrompt() const {
    cout << prompt << endl << endl;
    for (int i = 0; i < options.size(); i++)
        cout << i + 1 << " - " << options[i] << endl;
    cout << options.size() + 1 << " - " << cancelText << endl << endl;
}

int NumericalMenu::run() const {
    int result = 0;

    printPrompt();

    while ((result < 1) or (result > options.size())) {
        cin >> result;

        if (result == options.size() + 1)
            return -1;

        if ((result < 1) or (result > options.size())) {
            cout << endl << errorText << endl << endl;
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