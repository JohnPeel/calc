#include <iostream>
#include <algorithm>
#include "NumericalMenu.h"
#include "Expression.h"
#include "Utility.h"
#include "Variable.h"

using namespace std;

int main(int argc, char* argv[]) {
    NumericalMenu menu;

    const int newExpr = menu.addOption("Compute new expression");
    const int help = menu.addOption("Help");
    const int review = menu.addOption("Review previous expressions");
    bool quit = false;

    while (!quit) {
        int reply = menu.run();

        if (reply == newExpr)
            for (;;) {
                cout << "> ";
                string data, command;
                getline(cin, data);
                std::transform(data.begin(), data.end(), command.begin(), ::tolower);

                if (command != "quit") {
                    Expression *expr = strToExpr(data)->simplify();
                    cout << expr->getString() << endl;
                    Variable *ans = new Variable("ans", expr);
                } else
                    break;
            }

        if (reply == help)
            ; //TODO: Write this!!

        if (reply == review)
            ; //TODO: Write this

        if (reply == -1) {
            cout << "Quiting..." << endl;
            quit = true;
        }
    }

    return 0;
};