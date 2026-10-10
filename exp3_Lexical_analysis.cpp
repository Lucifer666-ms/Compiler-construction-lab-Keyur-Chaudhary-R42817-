#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <iomanip>

using namespace std;

// Set of standard C keywords
const unordered_set<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

// Set of operators
const unordered_set<string> operators = {
    "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "++", "--"
};

// Set of special symbols
const unordered_set<char> specialSymbols = {
    ';', ',', '(', ')', '{', '}', '[', ']', '#', ':'
};

bool isKeyword(const string& str) {
    return keywords.find(str) != keywords.end();
}

bool isNumber(const string& str) {
    if (str.empty()) return false;
    bool hasDecimal = false;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// Function to perform lexical analysis
void parseLexemes(const string& code) {
    int n = code.length();
    int i = 0;

    cout << left << setw(20) << "TOKEN" << setw(20) << "CATEGORY" << endl;
    cout << string(40, '-') << endl;

    while (i < n) {
        // Skip whitespaces
        if (isspace(code[i])) {
            i++;
            continue;
        }

        // Handle string literals
        if (code[i] == '"') {
            string strLit = "\"";
            i++;
            while (i < n && code[i] != '"') {
                strLit += code[i];
                i++;
            }
            if (i < n) {
                strLit += '"';
                i++;
            }
            cout << left << setw(20) << strLit << setw(20) << "String Literal" << endl;
            continue;
        }

        // Handle multi-character operators (e.g. ==, <=, >=, ++, --)
        if (i + 1 < n) {
            string op2 = code.substr(i, 2);
            if (operators.find(op2) != operators.end()) {
                cout << left << setw(20) << op2 << setw(20) << "Operator" << endl;
                i += 2;
                continue;
            }
        }

        // Handle single-character operators
        string op1(1, code[i]);
        if (operators.find(op1) != operators.end()) {
            cout << left << setw(20) << op1 << setw(20) << "Operator" << endl;
            i++;
            continue;
        }

        // Handle Special Symbols
        if (specialSymbols.find(code[i]) != specialSymbols.end()) {
            cout << left << setw(20) << string(1, code[i]) << setw(20) << "Special Symbol" << endl;
            i++;
            continue;
        }

        // Handle Identifiers and Keywords
        if (isalpha(code[i]) || code[i] == '_') {
            string token = "";
            while (i < n && (isalnum(code[i]) || code[i] == '_')) {
                token += code[i];
                i++;
            }
            if (isKeyword(token)) {
                cout << left << setw(20) << token << setw(20) << "Keyword" << endl;
            } else {
                cout << left << setw(20) << token << setw(20) << "Identifier" << endl;
            }
            continue;
        }

        // Handle Constants / Numbers
        if (isdigit(code[i])) {
            string token = "";
            while (i < n && (isdigit(code[i]) || code[i] == '.')) {
                token += code[i];
                i++;
            }
            if (isNumber(token)) {
                cout << left << setw(20) << token << setw(20) << "Constant" << endl;
            } else {
                cout << left << setw(20) << token << setw(20) << "Invalid Token" << endl;
            }
            continue;
        }

        // Handle unrecognized character
        string unknown(1, code[i]);
        cout << left << setw(20) << unknown << setw(20) << "Unknown Symbol" << endl;
        i++;
    }
}

int main() {
    string sampleCode = "int main() { int a = 10; float b = 20.5; int sum = a + b; return 0; }";

    cout << "--- INPUT SOURCE CODE ---\n";
    cout << sampleCode << "\n\n";

    cout << "--- LEXICAL ANALYSIS RESULT ---\n";
    parseLexemes(sampleCode);

    return 0;
}
