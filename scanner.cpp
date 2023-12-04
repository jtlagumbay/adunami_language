#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include "tokenEnum.h"
#include <algorithm>
#include <cctype>

using namespace std;

struct TokenInfo {
  int lineNumber;
  Token type;
  string lexeme;
  bool isIndented;
};

class Scanner {
  private:
    ifstream adm_file_reader;
    string adm_file;
    vector<TokenInfo> token_list;
    void scanLine(const string&, bool, int);
    Token checkTokenType(const string&);
    bool hasIndentation(const std::string&);
    string stripWhiteSpace(string);

  public:
    Scanner(string file_name);
    ~Scanner();
    vector<TokenInfo> start();
    void printTokenList();
};
//string getOperand(string);


Scanner::Scanner(string file_name){
  adm_file = file_name;
  adm_file_reader.open(adm_file);
  if(!adm_file_reader.is_open()){
    throw runtime_error("Error: Cannot open adm file.");
  }
}

Scanner::~Scanner(){
  if(adm_file_reader.is_open()){
    adm_file_reader.close();
  }
}
bool Scanner::hasIndentation(const string& line) {
  int space = 0;
  for (char c : line)
  {
    if (!isspace(c)){
      break;
    } else if (c == '\t' || c == ' '){
      space++;
    }
  }
  if(space<4){ // 2 spaces per tab, naa nay tab daan ang sud tungod sa PROG_BEGIN
    return false;
  } else {
    return true;
  }
}

vector<TokenInfo> Scanner::start(){
  string stream_line;
  int line_number = 1;
  bool program_finished = false;

  // Check if adm is started properly
  getline(adm_file_reader, stream_line);
  if(stream_line !="sa adm:"){
    throw runtime_error("Error: Unrecognized program. Adunami files should start with \'sa adm:\'");
  }

  while(getline(adm_file_reader, stream_line)){
    // remove all comments
    string line = regex_replace(stream_line, regex("\\*\\*\\s*([^*]+)\\s*\\*\\*"), "");
    // add space to print for easier shit
    line = regex_replace(line, regex("::"), " :: ");
    line = regex_replace(line, regex("="), " :: ");

    if(line.empty()==SPACE){
      continue;
    }
    if(line.find("hmn")!=string::npos) {
      program_finished = true;
      break;
    }
    
    scanLine(
      line, 
      hasIndentation(stream_line), 
      line_number);
    line_number++;
  }

  if(!program_finished){
    throw runtime_error("Error: Program did not end properly. Check syntax");
  }

  return token_list;
}


void Scanner::printTokenList(){
  for (const auto& element : token_list) {
      cout << element.lineNumber  << " " << tokenToString(element.type) << " " << element.lexeme << endl;
  }
}

void Scanner::scanLine(const string& inputLine, bool hasIndentation, int line_number){

  string expr;
  Token token;
  stringstream SS(inputLine);

  while (getline(SS, expr, ' ')) {
    if (expr.empty()) {
        continue;
    }
    token = checkTokenType(expr);
    
    if(token==UNKNOWN){
      throw runtime_error("ERROR: Unknown token on line " + line_number);
    }
    token_list.push_back(TokenInfo{
      line_number,
      token,
      expr,
      hasIndentation});
  }
    
}


string Scanner::stripWhiteSpace(string input){
  string line = regex_replace(input, regex("^\\s+"), "");
  return line;
}


Token Scanner::checkTokenType(const string& expr) {
    vector<pair<regex, Token>> regex_patterns = {
        { regex("^(isuwat)$"), OUTPUT },
        // { regex("^(sama|mas_dako|mas_gamay|labing_dako|labing_gamay)$"), "logical_ops" },
        // { regex("^[+\\-*/]$"), "arithmetic_ops" },
        { regex("^-?[0-9]+$"), INTEGER },
        { regex("^-?[0-9]+.[0-9]+$"), DOUBLE },
        { regex("^\".*\"$"), STRING },
        // { regex("^=$"), "assignment" },
        // { regex("^sulat$"), "output" },
        // { regex("^basa$"), "input" },
        // { regex("^gihimo$"), "loop_start" },
        // { regex("^bukid$"), "loop_end" },
        // { regex("^tapos$"), "if_end" },
        // { regex("^[a-zA-Z0-9.]+$"), "word" },
        // { regex("^[a-zA-Z0-9.]+$"), "" },
        // { regex("^[a-zA-Z0-9.]$"), "character" },
        // { regex("^[a-zA-Z][a-zA-Z0-9]*(_[a-zA-Z0-9]+)*$"), "var_name" },
        {regex("^(\\(|\\)|:|::|,|.)$"), PUNCTUATION}
    };

    for(const auto& pattern : regex_patterns) {
        if(regex_match(expr, pattern.first)) {
            return pattern.second;
        }
    }

    return UNKNOWN;
}

// vector<Token> scan(const string& line) {
//     vector<Token> tokens;

//     // if (line == "sa adm:") {
//     //     tokens.push_back(Token{"PROG_BEGIN", "sa adm:"});
//     //     return tokens;
//     // }

//     string expr;
//     Token tokenized;
//     stringstream SS(line);

//     while (getline(SS, expr, ' ')) {

//         cout << "token: " << expr << endl;

//         if (expr.empty()) {
//             continue;
//         }

//         tokenized = checkType(expr);

//         if (tokenized.type != "NO_MATCH") {
//             tokens.push_back(tokenized);
//             token_list.push_back(tokenized); // add token to the global list
//         } else {
//             // when a token doesn't match any pattern
//             cerr << "Error: Unrecognized token '" << expr << "'\n";
//         }
//     }

//     return tokens;
// }

// // string getKeyword(string expr){
// //   string keywords[] = {"kuptan", "kon", "kondili", "isuwat", "isulod"};
// //   int idx;

// //   for(int i = 0; i < sizeof(keywords); i++) {
// //       if(expr.find(keywords[i])!= string::npos) { //npos = no position, meaning naay keyword
// //         return keywords[i];
// //       }
// //   }

// //   return "NO_KEYWORD";
// // }

// // string getOperand(string expr){
// //   string operands[] = {"::", ":", "+", "-", "*", "/", "="};
// //   int idx;

// //   for(int i = 0; i < sizeof(operands); i++) {
// //       if(expr.find(operands[i])!= string::npos) { //npos = no position, meaning naay operand
// //         return operands[i];
// //       }
// //   }

// //   return "NO_OPS";
// // }