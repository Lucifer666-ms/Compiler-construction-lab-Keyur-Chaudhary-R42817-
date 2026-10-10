#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Node structure for Parse Tree
struct TreeNode {
    string value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string val) : value(val), left(nullptr), right(nullptr) {}
};

// Recursive Descent Parser Class
class Parser {
    string expr;
    size_t pos;

    void skipWhitespace() {
        while (pos < expr.length() && isspace(expr[pos])) pos++;
    }

    string peekToken() {
        skipWhitespace();
        if (pos >= expr.length()) return "";
        if (isalnum(expr[pos])) {
            size_t start = pos;
            while (start < expr.length() && isalnum(expr[start])) start++;
            return expr.substr(pos, start - pos);
        }
        return string(1, expr[pos]);
    }

    string getToken() {
        skipWhitespace();
        if (pos >= expr.length()) return "";
        if (isalnum(expr[pos])) {
            size_t start = pos;
            while (pos < expr.length() && isalnum(expr[pos])) pos++;
            return expr.substr(start, pos - start);
        }
        string tok(1, expr[pos]);
        pos++;
        return tok;
    }

public:
    Parser(string s) : expr(s), pos(0) {}

    // Statement -> Identifier '=' Expression | Expression
    TreeNode* parseStatement() {
        size_t savePos = pos;
        string t1 = getToken();
        if (peekToken() == "=") {
            getToken(); // Consume '='
            TreeNode* root = new TreeNode("=");
            root->left = new TreeNode(t1);
            root->right = parseExpression();
            return root;
        }
        pos = savePos;
        return parseExpression();
    }

    // Expression -> Term { ('+' | '-') Term }
    TreeNode* parseExpression() {
        TreeNode* left = parseTerm();
        while (true) {
            string op = peekToken();
            if (op == "+" || op == "-") {
                getToken(); // Consume '+' or '-'
                TreeNode* newRoot = new TreeNode(op);
                newRoot->left = left;
                newRoot->right = parseTerm();
                left = newRoot;
            } else {
                break;
            }
        }
        return left;
    }

    // Term -> Factor { ('*' | '/') Factor }
    TreeNode* parseTerm() {
        TreeNode* left = parseFactor();
        while (true) {
            string op = peekToken();
            if (op == "*" || op == "/") {
                getToken(); // Consume '*' or '/'
                TreeNode* newRoot = new TreeNode(op);
                newRoot->left = left;
                newRoot->right = parseFactor();
                left = newRoot;
            } else {
                break;
            }
        }
        return left;
    }

    // Factor -> IDENTIFIER | NUMBER | '(' Expression ')'
    TreeNode* parseFactor() {
        string tok = peekToken();
        if (tok == "(") {
            getToken(); // Consume '('
            TreeNode* node = parseExpression();
            if (peekToken() == ")") getToken(); // Consume ')'
            return node;
        }
        tok = getToken();
        return new TreeNode(tok);
    }
};

// Function to print Parse Tree visually
void printTree(TreeNode* root, string prefix = "", bool isLast = true) {
    if (root == nullptr) return;

    cout << prefix;
    cout << (isLast ? "└── " : "├── ");
    cout << root->value << endl;

    vector<TreeNode*> children;
    if (root->left) children.push_back(root->left);
    if (root->right) children.push_back(root->right);

    for (size_t i = 0; i < children.size(); ++i) {
        printTree(children[i], prefix + (isLast ? "    " : "│   "), i == children.size() - 1);
    }
}

int main() {
    string statement = "a = b + c * d";

    cout << "--- INPUT ARITHMETIC STATEMENT ---\n";
    cout << statement << "\n\n";

    Parser parser(statement);
    TreeNode* root = parser.parseStatement();

    cout << "--- CONSTRUCTED PARSE TREE ---\n";
    printTree(root);

    return 0;
}
