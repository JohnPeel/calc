#include <iostream>
#include <algorithm>
#include "NumericalMenu.h"
#include "Utility.h"

using namespace std;

int main() {
    NumericalMenu menu;
    ExpressionList history;

    const int newExpr = menu.addOption("Compute new expression");
    const int help = menu.addOption("Help");
    const int review = menu.addOption("Review previous expressions");
    bool quit = false;

    while (!quit) {
        int reply = menu.run();

        if (reply == newExpr) {
            cout << endl << "Type \"quit\" to go back to the previous menu." << endl << endl;

            for (;;) {
                cin.clear();
                fflush(stdin);

                cout << "> ";
                string data;
                getline(cin, data);

                string command = data;
                std::transform(command.begin(), command.end(), command.begin(), ::tolower);

                if ((command != "quit") && (command != "q")) {
                    try {
                        Expression *expr = strToExpr(data);//->simplify(); // FIXME: Re-enable auto-simplify
                        cout << expr->getString() << endl;

                        history.push_back(expr);
                        if (history.size() > 10)
                            history.pop_front();
                    } catch(const char* x) {
                        cout << x << endl;
                    }
                } else
                    break;
            }
        }

        if (reply == help) {
            //TODO: Write this!!
        }

        if (reply == review) {
            //TODO: Write this!!
        }

        if (reply == -1) {
            cout << "Quiting..." << endl;
            quit = true;
        }
    }

    return 0;
}
