#ifndef CALC_NUMERICALMENU_H__
#define CALC_NUMERICALMENU_H__

#include <string>
#include <vector>

class NumericalMenu {
private:
    std::vector<std::string> options;
    std::string cancelText;
    std::string errorText;
    std::string prompt;
    bool shouldRepeat;
    void printPrompt() const;
public:
    NumericalMenu();
    int addOption(std::string option);
    void setCancelText(std::string cancelText);
    void setErrorText(std::string errorText);
    void setPrompt(std::string prompt);
    void setRepeatPromptOnError(bool shouldRepeat);
    int run() const;
    int size() const;
};

#endif // CALC_NUMERICALMENU_H__