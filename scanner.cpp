#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

struct Token {
  string type;
  string value;
  int line; 
  int column;
};

Token token_list[100];
int size = 0;

Token checkType(string, int, int);
void scan(string);
//string getOperand(string);

int main(){
  string file_name = "sample.txt"; // file path of the source code
  ifstream file_reader;
  file_reader.open(file_name);

  string line;
  int line_number = 0; 

  while(getline(file_reader, line)){
    line_number++;
    scan(line, line_number); // scan per line
  }
  
  file_reader.close();

  // Print the scanned tokens 
  for (int i = 0; i<size; i++){
    cout << "Token type: " << token_list[i].type << ", Value: " << token_list[i].value
         << ", Line: " << token_list[i].line << ", Column: " << token_list[i].column << endl;
  }

  return 0; 

  //string sample = "fdcv_asdas";
  //Token sampleToken = checkType(sample);
  //cout << sampleToken.type << ": " << sampleToken.value << endl;
}

Token checkType(string expr, int line, int column) {

  //vector<Token> tokens;

  vector<pair<regex, string>> regex_patterns = {
    { regex("^(isuwat|isulod|kuptan|kon|kondili|mana)$"), "keyword" },
    { regex("^(sama|mas dako|mas gamay|labing dako|labing gamay)$"), "logical_ops" },
    { regex("^[+\\-*/]$"), "arithmetic_ops" },
    { regex("^-?[0-9]+$"), "integer" },
    { regex("^-?[0-9]+.[0-9]+$"), "decimal" },
    { regex("^[a-zA-Z][a-zA-Z0-9]*$"), "variable" },
    { regex("^=$"), "assignment" },
    { regex("^sulat$"), "output" },
    { regex("^basa$"), "input" },
    { regex("^gihimo$"), "loop_start" },
    { regex("^bukid$"), "loop_end" },
    { regex("^kung$"), "if_start" },
    { regex("^di$"), "else_start" },
    { regex("^tapos$"), "if_end" },
    { regex("^[a-zA-Z0-9.]+$"), "word" },
    { regex("^[a-zA-Z0-9.]$"), "character" },
    { regex("^[a-zA-Z][a-zA-Z0-9]*(_[a-zA-Z0-9]+)*$"), "var_name" }
  };

  for(const auto& pattern : regex_patterns) {
    if(regex_match(expr, pattern.first)) {
      return Token{ pattern.second, expr, line, column};
    }
  }
    
  return Token{ "NO_MATCH", expr, line, column};
}

void scan(string line, int line_number) {
    string expr;
    stringstream SS(line);

    int column_number = 0;

    while (getline(SS, expr, ' ')) {
        column_number += expr.size() + 1; // Add 1 for the space

        Token tokenized = checkType(expr, line_number, column_number);

        if (tokenized.type != "NO_MATCH") {
            token_list[size++] = tokenized;
        } else {
            // Handle unrecognized token
            cout << "Error: Unrecognized token '" << expr << "' at Line: " << line_number
                 << ", Column: " << column_number << endl;
        }
    }
}

// vector<Token> scan(string line){
//   vector<Token> tokens;

//   if(line == "sa adm:"){
//     tokens.push_back(Token{ "PROG_BEGIN", "sa adm:" });
//     return tokens;
//   } 

//   // process the line here
//   string expr;
//   Token tokenized;
//   stringstream SS(line);

//   while(getline(SS, expr, ' ')) {
//     tokenized = checkType(expr);

//     if(tokenized.type != "NO_MATCH")
//       tokens.push_back(tokenized);
    
//     else {
//       // do something
//     }

//   }

// }

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