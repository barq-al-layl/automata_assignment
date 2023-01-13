#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#define INPUT_FILE_NAME  "dfa.txt"

using namespace std;

/** Input file structure
 *  [set of all possible states]
 *  [language symbols]
 *  [initial state]
 *  [set of finale states]
 *  [transtion function]
 */
set<char> symbols;
set<string> states, finalStates;
map<pair<string, char>, string> transitions;
string initialState;

void readInputFile() {
    ifstream inputFile(INPUT_FILE_NAME);
    string line, temp;

    // reading set of states.
    getline(inputFile, line);
    istringstream stringStream(line);
    while (stringStream >> temp) {
        states.insert(temp);
    }

    // reading input symbols.
    getline(inputFile, line);
    for (char symbol: line) {
        symbols.insert(symbol);
    }

    // reading the initial state.
    getline(inputFile, line);
    initialState = line;

    // reading set of final states (accepted states).
    getline(inputFile, line);
    stringStream = istringstream(line);
    while (stringStream >> temp) {
        finalStates.insert(temp);
    }

    // reading transition function.
    string currentState, nextState;
    char inputSymbol;
    pair<string, char> key;
    while (getline(inputFile, line)) {
        stringStream = istringstream(line);
        stringStream >> currentState >> inputSymbol >> nextState;
        key = make_pair(currentState, inputSymbol);
        transitions[key] = nextState;
    }
}

int main() {
    readInputFile();
    string w;
    cout << "Enter w: ";
    getline(cin, w);

    string currentState = initialState;
    vector<pair<string, char>> path;
    for (auto inputSymbol: w) {
        path.emplace_back(currentState, inputSymbol);
        currentState = transitions[path.back()];
    }
    if (finalStates.find(currentState) != finalStates.end()) {
        cout << "Yes, the path is:\n";
        for (const auto &it: path) {
            cout << it.first << ", " << it.second << " -> ";
        }
        cout << currentState << '\n';
    } else {
        cout << "No!\n";
    }
}
