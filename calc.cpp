#include <iostream>
#include "ShuntingYard.h"

using namespace std;

int main(void) {
    cout << "> ";
    string data = "(4x - 2)/(2x)";
    cout << data << endl;//getline(cin, data);

    ShuntingYard parser = ShuntingYard(data);
    Expression* expr = parser.process();

    cout << "Original:\t" << expr->getString() << endl;
    cout << "Simplified:\t" << expr->simplify()->getString() << endl;

    return 0;
}