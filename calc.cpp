#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "Utility.h"
#include "NumericalMenu.h"
#include "Method.h"

using namespace std;

int main() {
    ExpressionList history;
    cout << endl << "Type \"help\" to see help, or \"quit\" to quit." << endl << endl;

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
            cout << "c, cls, clear\tclear the console." << endl;
            cout << "q, quit\t\tquit the program" << endl << endl;
            cout << "Methods: ";
            for (auto method : registeredMethods.map)
                cout << method.first << " ";
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
            }
        } else if ((command == "c") || (command == "clear") || (command == "cls")) {
            system("cls"); // FIXME: Windows only...
        } else if ((command == "q") || (command == "quit")) {
            cout << endl;
            break;
        }else {
            try {
                Expression *expr = strToExpr(data);
                cout << expr->simplify()->getString() << endl;

                history.push_front(expr);
                if (history.size() > 10)
                    history.pop_back();
            } catch (const char *x) {
                cout << x << endl;
            }
        }
    }

    return 0;
}
