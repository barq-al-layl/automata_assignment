#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#define INPUT_FILE_NAME  "nfa.txt"

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
    DFA(
            const set<char> &symbols,
            const set<string> &states,
            const set<string> &finalStates,
            const map<pair<string, char>, string> &transitions,
            const string &initialState
    ) : symbols(symbols),
        states(states),
        finalStates(finalStates),
        transitions(transitions),
        initialState(initialState) {}

    bool isAccepted(const string &w) {
        currentState = initialState;
        for (char inputSymbol: w) {
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

    void showQuintuple() {
        cout << "Q: ";
        for (const string &state: states) {
            cout << state << ' ';
        }
        cout << "\ninitial state = " << initialState << '\n';
        cout << "final states: ";
        for (const string &state: finalStates) {
            cout << state << ' ';
        }
        cout << "\ntransition function:\n";
        for (auto [key, value]: transitions) {
            cout << key.first << ", " << key.second << " -> " << value << '\n';
        }
    }
};

class NFA {
    set<char> symbols;
    set<string> states, finalStates;
    map<pair<string, char>, vector<string>> transitions;
    string initialState;
    vector<pair<string, char>> path;
    string currentState;

public:
    NFA() {
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
            stringStream >> currentState >> inputSymbol;
            key = make_pair(currentState, inputSymbol);
            while (stringStream >> nextState) {
                transitions[key].emplace_back(nextState);
            }
        }
    }

    DFA toDFA() {
        set<string> dfaStates, dfaFinalStates;
        map<pair<string, char>, string> dfaTransitions;
        string tState, tNextState;
        pair<string, char> tKey;
        for (auto [key, value]: transitions) {
            tState = "";
            for (const string &s: value) {
                tState += s;
            }
            for (char ch: symbols) {
                tNextState = "";
                for (const string &s: value) {
                    tKey = make_pair(s, ch);
                    if (transitions[tKey].empty()) continue;
                    for (const string &s1: transitions[tKey]) {
                        tNextState += s1;
                    }
                    dfaStates.insert(tState);
                    tKey = make_pair(tState, ch);
                    dfaTransitions[tKey] = tNextState;
                }
            }
        }
        for (auto [key, value]: dfaTransitions) {
            if (dfaStates.find(value) != dfaStates.end()) continue;
            for (const string &state: states) {
                if (value.find(state) == string::npos) continue;
                for (char ch: symbols) {
                    tNextState = "";
                    for (const string& s: transitions[make_pair(state, ch)]) {
                        tNextState += s;
                    }
                    if (!tNextState.empty()) continue;
                    tKey = make_pair(value, ch);
                    dfaTransitions[tKey] = tNextState;
                }
            }
        }

        return DFA{
                symbols, dfaStates, dfaFinalStates,
                dfaTransitions, initialState
        };
    }
};

int main() {
    NFA nfa = NFA();
    DFA dfa = nfa.toDFA();
    dfa.showQuintuple();
    string w;
    cout << "Enter w: ";
//    getline(cin, w);
    if (dfa.isAccepted(w)) {
        cout << "Accepted, path:\n";
        dfa.showPath();
    } else {
        cout << "Rejected.\n";
    }
}
