#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "Utility.h"
#include "NumericalMenu.h"
#include "Method.h"
#include "Variable.h"

using namespace std;

class AnsVar : Variable {
public:
    AnsVar() : Variable("ans", nullptr, true) {};
    void update(Expression* expr) { value = expr; };
};

int main() {
    ExpressionList history;
    cout << endl << "Type \"help\" to see help, or \"quit\" to quit." << endl << endl;

    auto ans = new AnsVar();

    for (;;) {
        cin.clear();
        fflush(stdin);

        cout << "> ";
        string data;
        getline(cin, data);

        string command = data;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        if ((command == "h") || (command == "help")) {
            cout << endl << " - Commands - " << endl;
            cout << "h, help\t\tthis menu." << endl;
            cout << "r, review\treview the last ten successful expressions." << endl;
            cout << "a, ans\t\tshow previous answer." << endl;
            cout << "n, float\tshow floating point repr. of previous answer" << endl;
            cout << "q, quit\t\tquit the program" << endl << endl;

            cout << " - Supported Methods - " << endl;
            for (auto method : registeredMethods.map)
                cout << method.first << endl;
            cout << endl;

            cout << " - Supported Operations - " << endl;
            cout << "+\t\tAddition" << endl;
            cout << "-\t\tSubtraction" << endl;
            cout << "*\t\tMultiplication" << endl;
            cout << "/\t\tDivision" << endl;
            cout << "^, **\t\tExponents" << endl;
            cout << "rt\t\tRoots, n rt x" << endl;
            cout << "log, ln\t\tLogarithms, x log b" << endl;
            cout << endl;

            cout << " - Supported Symbols - " << endl;
            cout << "pi\t\tPi" << endl;
            cout << "e\t\tEuler's Number" << endl;
            cout << "i\t\tImaginary Number" << endl;
            cout << "ans\t\tUse previous answer in current expression." << endl;
            cout << endl;
        } else if ((command == "r") || (command == "review")) {
            NumericalMenu reviewMenu;
            for (Expression* expr : history)
                reviewMenu.addOption(expr->getString());

            int reply = reviewMenu.run();

            if (reply != -1) {
                cout << endl << history[reply - 1]->simplify()->getString() << endl;

                history.push_front(history[reply - 1]);
                history.erase(history.begin() + reply);
                ans->update(history[0]);
            }
        } else if ((command == "a") || (command == "ans")) {
            if (history.empty())
                cout << "There is no previous answer!" << endl;
            else
                cout << history[0]->getString() << endl;
        } else if ((command == "n") || (command == "float")) {
            if (history.empty())
                cout << "There is no previous answer!" << endl;
            else if (!history[0]->hasValue())
                cout << "Previous expression has no floating point value!" << endl;
            else
                cout << history[0]->getValue() << endl;
        } else if ((command == "q") || (command == "quit") || (command == "exit")) {
            cout << endl;
            break;
        }else {
            try {
                Expression *expr = strToExpr(data);
                cout << expr->simplify()->getString() << endl;

                history.push_front(expr);
                ans->update(history[0]);
                if (history.size() > 10)
                    history.pop_back();
            } catch (const char *x) {
                cout << x << endl;
            }
        }
    }

    return 0;
}
