#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

/*  T = {!, +,*, (,), a, b}
    N = { А, В, В’, Т, Т’, М }, A - стартовый символ
    Продукции (e - пустая строка):
    1) А -> !В!
    2) В -> ТВ'
    3) В' -> e
    4) В' -> + ТВ'
    5) Т -> МТ'
    6) Т' -> e
    7) Т' -> * МТ'
    8) М -> a
    9) М -> b
    10) М -> (В) */


// Terminals
/* B' = P
   T' = I */

// Grammar
unordered_map<int, pair<string, string>> grammar = {
    {1, {"A", "!B!"}},
    {2, {"B", "TP"}},
    {3, {"P", "e"}},
    {4, {"P", "+TP"}},
    {5, {"T", "MI"}},
    {6, {"I", "e"}},
    {7, {"I", "*MI"}},
    {8, {"M", "a"}},
    {9, {"M", "b"}},
    {10, {"M", "(B)"}}
};

// LL(1) table
const int row = 15;
const int col = 9;
char table[row][col] = {
    {  0,   '!',  '+',  '*',  '(',  ')',  'a', 'b', 'e'},
    { 'A',   1,    0,    0,    0,    0,    0,   0,   0  },
    { 'B',   0,    0,    0,    2,    0,    2,   2,   0  },
    { 'P',   3,    4,    0,    0,    3,    0,   0,   0  },
    { 'T',   0,    0,    0,    5,    0,    5,   5,   0  },
    { 'I',   6,    6,    7,    0,    6,    0,   0,   0  },
    { 'M',   0,    0,    0,   10,    0,    8,   9,   0  },
    { '!',  -1,    0,    0,    0,    0,    0,   0,   0  },
    { '+',   0,   -1,    0,    0,    0,    0,   0,   0  },
    { '*',   0,    0,   -1,    0,    0,    0,   0,   0  },
    { '(',   0,    0,    0,   -1,    0,    0,   0,   0  },
    { ')',   0,    0,    0,    0,   -1,    0,   0,   0  },
    { 'a',   0,    0,    0,    0,    0,   -1,   0,   0  },
    { 'b',   0,    0,    0,    0,    0,    0,  -1,   0  },
    { 'e',   0,    0,    0,    0,    0,    0,   0,  -1  }
};

void printState(const string str, const vector<char> stack, const string action, int rule) {
    cout << "String: " << str << endl;
    cout << "Stack: ";
    for (char c : stack) {
        cout << c << " ";
    }
    cout << endl;
    cout << "Action: " << action << endl;
    cout << "Rule: " << rule << endl;
    cout << endl;
}

vector<int> LL1(string str) {
    vector<int> numOfRules;
    vector<char> stack = { 'A' };

    while (!stack.empty() && !str.empty()) {
        if (stack.front() == 'e') {
            stack.erase(stack.begin());
        }
        else {
            int i = 0, j = 0;
            while (i < row) {
                if (table[i][0] == stack.front())
                    break;
                i++;
            }
            while (j < col) {
                if (table[0][j] == str.front())
                    break;
                j++;
            }
            int rule = table[i][j];
            if (rule == 0) {
                //cout << "\nError" << endl;
                return {};
            }
            else if (rule == -1) {
                // Ejection
                str.erase(str.begin());
                stack.erase(stack.begin());
                //printState(str, stack, "Ejection", -1);

            }
            else {
                // Generation
                numOfRules.push_back(rule);
                string right = grammar[rule].second;
                stack.erase(stack.begin());
                for (int i = right.size() - 1; i >= 0; --i) {
                    stack.insert(stack.begin(), right[i]);
                }
                //printState(str, stack, "Generation", rule);
            }
        }
    }

    if (!stack.empty() || !str.empty()) {
        //cout << "\nError" << endl;
        return {};
    }
    return numOfRules;
}

int main() {
    vector<string> input = {
        "!a+b!", 
        "!a*b!",
        "!(a+b)*(b+a)!", 
        "!b*a+a*b!", 
        "!(a+b)*a+b*a!", 
        "!((a+b*a)*(b*b+a*(a+b+a)))*((a+b*a)*(b*b+a*(a+b+a)))+((a+b*a)*(b*b+a*(a+b+a)))!", 
        "!a+*b!", 
        "a+b*a+b",
        "a!b", 
        "!a(b+a()!" };
    
    for (int i = 0; i < input.size(); i++) {
        vector<int> result = LL1(input[i]);
        cout << "\nString: " << input[i] << endl;
        if (!result.empty()) {
            cout << "Rules: ";
            for (int rule : result) {
                cout << rule << " ";
            }
            cout << endl;
        }
        else {
            cout << "Error\n";
        }
    }

    return 0;
}
