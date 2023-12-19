#include <iostream>
#include <queue>
#include <stack>

using namespace std;

queue<string> q;

int leastPrec(string x) {
    for(int i = x.length() - 1; i >= 0; i--)
        if(!isdigit(x[i]))
            if(x[i] == '+' || x[i] == '-')
                return i;
    
    for(int i = x.length() - 1; i >= 0; i--)
        if(!isdigit(x[i]))
            return i;
    
    return -1;
}

void postfix(string x) {
    int pos = leastPrec(x);
    string prnt;

    if(pos >= 0)
        prnt = x.substr(pos, 1);
    else {
        prnt = x;
    }
    
    if(pos >= 0) // left
        postfix(x.substr(0, pos));

    if(pos < x.length()) // right
        postfix(x.substr(pos + 1));

    // cout << prnt << " "; // print
    q.push(prnt);
}

bool isDigit(string x) {
    for (char c : x) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

string solveArithmetic(string x, string y, string op) {
    int a = stol(x);
    int b = stol(y);
    int answer;

    if(op == "-")
        answer = a - b;

    else if(op == "+")
        answer = a + b;

    else if(op == "*")
        answer = a * b;

    else
        answer = a / b;

    return to_string( answer );
}

int solveQ() {
    string x, y;
    stack<string> s;

    while(!q.empty()) {
        if(!isDigit( q.front() )) {
            y = s.top();
            s.pop();
            x = s.top();
            s.pop();

            s.push( solveArithmetic(x, y, q.front()) );
        } else
            s.push( q.front() );
        
        q.pop();
    }

    return stol( s.top() );
}

string removeSpaces(string expr) {
    expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

    return expr;
}

int calculate(string expr) {
    expr = removeSpaces(expr);

    postfix(expr);

    return solveQ();
}