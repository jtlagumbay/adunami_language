#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

struct Token {
  string type;
  string value;
};

Token token_list[100];
int size = 0;
Token checkType(string);
void scan(string);
string getOperand(string);

int main(){
  // string file_name = "sample.txt"; // file path of the source code
  // ifstream file_reader;
  // file_reader.open(file_name);

  // string line;
  // while(getline(file_reader, line)){
  //   scan(line); // scan per line
  // }
  
  // file_reader.close();

  string sample = "fdcv_asdas";

  Token sampleToken = checkType(sample);
  cout << sampleToken.type << ": " << sampleToken.value << endl;
}

Token checkType(string expr) {
  vector<Token> tokens;

  vector<pair<regex, string>> regex_patterns = {
    { regex("^(isuwat|isulod|kuptan|kon|kondili|mana)$"), "keyword" },
    { regex("^(sama|mas dako|mas gamay|labing dako|labing gamay)$"), "logical_ops" },
    { regex("^[+\\-*/]$"), "arithmetic_ops" },
    { regex("^-?[0-9]+$"), "integer" },
    { regex("^-?[0-9]+.[0-9]+$"), "decimal" },
    { regex("^[a-zA-Z0-9.]+$"), "word" },
    { regex("^[a-zA-Z0-9.]$"), "character" },
    { regex("^[a-zA-Z][a-zA-Z0-9]*(_[a-zA-Z0-9]+)*$"), "var_name" }
  };

  for(const auto& pattern : regex_patterns) {
    if(regex_match(expr, pattern.first)) {
      return Token{ pattern.second, expr };
    }
  }
    
  return Token{ "NO_MATCH", expr };
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