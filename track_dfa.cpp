#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#define INPUT_FILE_NAME  "dfa.txt"

/** Input file structure
 *  [set of all possible states]
 *  [language symbols]
 *  [initial state]
 *  [set of finale states]
 *  [transtion function]
 */

using namespace std;

class DFA {
    set<char> symbols;
    set<string> states, finalStates;
    map<pair<string, char>, string> transitions;
    string initialState;
    vector<pair<string, char>> path;
    string currentState;

public:
    DFA() {
        ifstream file(INPUT_FILE_NAME);
        string line, temp;

        // reading set of states.
        getline(file, line);
        istringstream stringStream(line);
        while (stringStream >> temp) {
            states.insert(temp);
        }

        // reading input symbols.
        getline(file, line);
        for (char symbol: line) {
            symbols.insert(symbol);
        }

        // reading the initial state.
        getline(file, line);
        initialState = line;

        // reading set of final states (accepted states).
        getline(file, line);
        stringStream = istringstream(line);
        while (stringStream >> temp) {
            finalStates.insert(temp);
        }

        // reading transition function.
        string nextState;
        char inputSymbol;
        pair<string, char> key;
        while (getline(file, line)) {
            stringStream = istringstream(line);
            stringStream >> currentState >> inputSymbol >> nextState;
            key = make_pair(currentState, inputSymbol);
            transitions[key] = nextState;
        }
    }

    bool isAccepted(const string &w) {
        currentState = initialState;
        for (auto inputSymbol: w) {
            path.emplace_back(currentState, inputSymbol);
            currentState = transitions[path.back()];
        }
        return finalStates.find(currentState) != finalStates.end();
    }

    void showPath() {
        for (auto [state, input]: path) {
            cout << state << " -> ";
        }
        cout << currentState << '\n';
    }
};

int main() {
    DFA dfa = DFA();
    string w;
    cout << "Enter w: ";
    getline(cin, w);
    if (dfa.isAccepted(w)) {
        cout << "Accepted, path:\n";
        dfa.showPath();
    } else {
        cout << "Rejected.\n";
    }
}
