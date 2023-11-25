#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>

using namespace std;

struct Token {
    string type;
    string lexeme;
};

vector<Token> token_list;
Token checkType(const string&);
vector<Token> scan(const string&);

//string getOperand(string);

int main(){
    string file_name = "sample.txt"; // file path of the source code
    ifstream file_reader(file_name);

    if (!file_reader.is_open()) {
        cerr << "Error: Unable to open file" << file_name << endl;
        return 1;
    }

    string line;
  
    while(getline(file_reader, line)){
      scan(line); // scan per line
    }
  
    file_reader.close();

    // display tokens
    for (const auto& token : token_list) {
        cout << token.type << ": " << token.lexeme << endl;
    }

    return 0; 

    //string sample = "fdcv_asdas";
    //Token sampleToken = checkType(sample);
    //cout << sampleToken.type << ": " << sampleToken.lexeme << endl;
}

Token checkType(const string& expr) {
    vector<pair<regex, string>> regex_patterns = {
        { regex("^(isuwat|isulod|igawas|kuptan|kon|kondili|mana)$"), "keyword" },
        { regex("^(sama|mas_dako|mas_gamay|labing_dako|labing_gamay)$"), "logical_ops" },
        // { regex("^[+\\-*/]$"), "arithmetic_ops" },
        { regex("^-?[0-9]+$"), "integer" },
        { regex("^-?[0-9]+.[0-9]+$"), "decimal" },
        // { regex("^=$"), "assignment" },
        // { regex("^sulat$"), "output" },
        // { regex("^basa$"), "input" },
        // { regex("^gihimo$"), "loop_start" },
        // { regex("^bukid$"), "loop_end" },
        // { regex("^tapos$"), "if_end" },
        { regex("^[a-zA-Z0-9.]+$"), "word" },
        { regex("^[a-zA-Z0-9.]$"), "character" },
        { regex("^[a-zA-Z][a-zA-Z0-9]*(_[a-zA-Z0-9]+)*$"), "var_name" },
        {regex("^(\\(|\\)|:|::|,|.)$"), "punctuation"},
    };

    for(const auto& pattern : regex_patterns) {
        if(regex_match(expr, pattern.first)) {
            return Token{ pattern.second, expr};
        }
    }
    
    return Token{ "NO_MATCH", expr};
}

vector<Token> scan(const string& line) {
    vector<Token> tokens;

    // if (line == "sa adm:") {
    //     tokens.push_back(Token{"PROG_BEGIN", "sa adm:"});
    //     return tokens;
    // }

    string expr;
    Token tokenized;
    stringstream SS(line);

    while (getline(SS, expr, ' ')) {

        cout << "token: " << expr << endl;

        if (expr.empty()) {
            continue;
        }

        tokenized = checkType(expr);

        if (tokenized.type != "NO_MATCH") {
            tokens.push_back(tokenized);
            token_list.push_back(tokenized); // add token to the global list
        } else {
            // when a token doesn't match any pattern
            cerr << "Error: Unrecognized token '" << expr << "'\n";
        }
    }

    return tokens;
}

// string getKeyword(string expr){
//   string keywords[] = {"kuptan", "kon", "kondili", "isuwat", "isulod"};
//   int idx;

//   for(int i = 0; i < sizeof(keywords); i++) {
//       if(expr.find(keywords[i])!= string::npos) { //npos = no position, meaning naay keyword
//         return keywords[i];
//       }
//   }

//   return "NO_KEYWORD";
// }

// string getOperand(string expr){
//   string operands[] = {"::", ":", "+", "-", "*", "/", "="};
//   int idx;

//   for(int i = 0; i < sizeof(operands); i++) {
//       if(expr.find(operands[i])!= string::npos) { //npos = no position, meaning naay operand
//         return operands[i];
//       }
//   }

//   return "NO_OPS";
// }