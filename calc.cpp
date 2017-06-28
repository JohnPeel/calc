#include <iostream>
#include "ShuntingYard.h"

using namespace std;

class InputParser{
protected:
    vector<string> tokens;
public:
    InputParser(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++)
            tokens.push_back(string(argv[i]));
    }

    bool hasParameter(string option){
        return find(tokens.begin(), tokens.end(), option) != tokens.end();
    }
};

int main(int argc, char* argv[]) {
    InputParser input(argc, argv);

    // TODO: Write program.
    cout << "Hello world!" << endl;

    return 0;
};