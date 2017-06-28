#ifndef CALC_NUMERICALMENU_H__
#define CALC_NUMERICALMENU_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class NumericalMenu {
private:
    vector<string> options;
    string cancelText;
    string errorText;
    string prompt;
    bool shouldRepeat;
    void printPrompt() const;
public:
    NumericalMenu();
    int addOption(string option);
    void setCancelText(string cancelText);
    void setErrorText(string errorText);
    void setPrompt(string prompt);
    void setRepeatPromptOnError(bool shouldRepeat);
    int run() const;
    int size() const;
};

#endif // CALC_NUMERICALMENU_H__