#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

// Structure to hold Macro details
struct Macro {
    string name;
    vector<string> params;
    vector<string> body;
};

// Global Macro Name Table (MNT)
unordered_map<string, Macro> mnt;

// Helper function to tokenize string input
vector<string> tokenize(const string& str) {
    stringstream ss(str);
    string token;
    vector<string> tokens;
    while (ss >> token) {
        if (!token.empty() && token.back() == ',') {
            token.pop_back(); // strip trailing comma
        }
        tokens.push_back(token);
    }
    return tokens;
}

// Function to perform recursive macro expansion
void expandMacro(const string& macroName, const vector<string>& actualArgs, vector<string>& expandedCode) {
    if (mnt.find(macroName) == mnt.end()) return;

    Macro m = mnt[macroName];

    // Map formal parameters to actual parameters
    unordered_map<string, string> paramMap;
    for (size_t i = 0; i < m.params.size() && i < actualArgs.size(); ++i) {
        paramMap[m.params[i]] = actualArgs[i];
    }

    // Process each instruction inside the macro body
    for (const string& line : m.body) {
        vector<string> tokens = tokenize(line);
        if (tokens.empty()) continue;

        // Substitute formal arguments with actual arguments
        string substitutedLine = "";
        for (size_t i = 0; i < tokens.size(); ++i) {
            string token = tokens[i];
            if (paramMap.count(token)) {
                token = paramMap[token];
            }
            substitutedLine += token + (i == tokens.size() - 1 ? "" : " ");
        }

        // Check for recursive or nested macro calls
        vector<string> lineTokens = tokenize(substitutedLine);
        if (!lineTokens.empty() && mnt.find(lineTokens[0]) != mnt.end()) {
            string nestedMacro = lineTokens[0];
            vector<string> nestedArgs(lineTokens.begin() + 1, lineTokens.end());
            expandMacro(nestedMacro, nestedArgs, expandedCode); // Recursive Expansion
        } else {
            expandedCode.push_back(substitutedLine);
        }
    }
}

// Pass 1: Identify Macro Definitions and Expand Assembly Code
void processAssemblyCode(const vector<string>& inputLines) {
    bool insideMacro = false;
    Macro currentMacro;
    vector<string> mainProgram;

    // 1. Identify Macros
    for (size_t i = 0; i < inputLines.size(); ++i) {
        string line = inputLines[i];
        vector<string> tokens = tokenize(line);
        if (tokens.empty()) continue;

        if (tokens[0] == "MACRO") {
            insideMacro = true;
            i++; // Move to macro header line
            vector<string> headerTokens = tokenize(inputLines[i]);
            currentMacro.name = headerTokens[0];
            currentMacro.params = vector<string>(headerTokens.begin() + 1, headerTokens.end());
            currentMacro.body.clear();
        } else if (tokens[0] == "MEND") {
            insideMacro = false;
            mnt[currentMacro.name] = currentMacro;
        } else if (insideMacro) {
            currentMacro.body.push_back(line);
        } else {
            mainProgram.push_back(line);
        }
    }

    // Display Macro Name Table
    cout << "\n=== IDENTIFIED MACRO DEFINITIONS ===\n";
    for (const auto& pair : mnt) {
        cout << "Macro Name: " << pair.first << "\nParameters: ";
        for (const auto& p : pair.second.params) cout << p << " ";
        cout << "\nBody:\n";
        for (const auto& b : pair.second.body) cout << "  " << b << "\n";
        cout << "------------------------------------\n";
    }

    // 2. Expand Main Program
    vector<string> expandedCode;
    for (const string& line : mainProgram) {
        vector<string> tokens = tokenize(line);
        if (!tokens.empty() && mnt.find(tokens[0]) != mnt.end()) {
            string macroName = tokens[0];
            vector<string> args(tokens.begin() + 1, tokens.end());
            expandMacro(macroName, args, expandedCode);
        } else {
            expandedCode.push_back(line);
        }
    }

    // Display Expanded Code
    cout << "\n=== EXPANDED ASSEMBLY CODE ===\n";
    for (const string& line : expandedCode) {
        cout << line << "\n";
    }
}

int main() {
    // Input Assembly Program containing macro definitions and macro calls
    vector<string> inputCode = {
        "MACRO",
        "ADD_MEM ARG1 ARG2",
        "MOVER AREG ARG1",
        "ADD AREG ARG2",
        "MOVEM AREG ARG1",
        "MEND",
        "MACRO",
        "CALCULATE X Y Z",
        "ADD_MEM X Y",
        "ADD_MEM X Z",
        "MEND",
        "START 100",
        "READ A",
        "READ B",
        "READ C",
        "CALCULATE A B C",
        "STOP",
        "END"
    };

    cout << "--- ORIGINAL INPUT PROGRAM ---\n";
    for (const string& line : inputCode) {
        cout << line << "\n";
    }

    processAssemblyCode(inputCode);

    return 0;
}
