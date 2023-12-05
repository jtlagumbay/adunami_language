#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <cctype>

#include "tokenEnum.hpp"
#include "errorHandler.hpp"

using namespace std;

struct TokenInfo {
  int line_number;
  int token_number;
  Token type;
  string lexeme;
  bool is_indented;
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


Scanner::Scanner(string file_name){
  adm_file = file_name;
  adm_file_reader.open(adm_file);
  if(!adm_file_reader.is_open()){
    throw Error(
      INPUT_OUTPUT,
      "Adm file not found.",
      "scanner.cpp > Scanner::Scanner",
      "Incorrect file path."
    );
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
    throw Error(
      SYNTAX,
      "Unrecognized program. Adunami files should start with \'sa adm:\'",
      "scanner.cpp > Scanner::start",
      "User error or Naputol ang file pag save."
    );
  }

  while(getline(adm_file_reader, stream_line)){
    // remove all comments
    string line = regex_replace(stream_line, regex("\\*\\*\\s*([^*]+)\\s*\\*\\*"), "");
    // add space to print for easier shit
    line = regex_replace(line, regex("::"), " :: ");
    line = regex_replace(line, regex("="), " = ");

    if(line.empty()){
      line_number++;
      continue;
    }
    if(line.find("hmn")!=string::npos) {
      program_finished = true;
      break;
    }
    
    scanLine(
      line, 
      hasIndentation(stream_line), 
      line_number++);
    
  }

  if(!program_finished){
    throw Error(
      SYNTAX,
      "Program did not end properly. Adunami files should end with \'hmn\'",
      "scanner.cpp > Scanner::start",
      "User error or Naputol ang file pag save."
    );
  }

  return token_list;
}


void Scanner::printTokenList(){
  for (const auto& element : token_list) {
      cout << element.line_number  << " " << tokenToString(element.type) << " " << element.lexeme << endl;
  }
}

void Scanner::scanLine(const string& inputLine, bool hasIndentation, int line_number){

  int token_number = 0;
  string expr;
  Token token;
  stringstream SS(inputLine);

  while (getline(SS, expr, ' ')) {
    if (expr.empty()) {
        continue;
    }
    token = checkTokenType(expr);
    // string errorMsg = "Unknown token on line " + to_string(line_number);
    // if(token==UNKNOWN){
    //   throw Error(
    //   SYNTAX,
    //   errorMsg,
    //   "scanner.cpp > Scanner::scanLine",
    //   "User error or Wala na catch ang token properly."
    // );
    // }
    token_list.push_back(TokenInfo{
      line_number,
      ++token_number,
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
      {regex("^(isuwat)$"), OUTPUT},
      {regex("^(isulod)$"), INPUT},
      {regex("^(kuptan)$"), DECLARE},
      // { regex("^(sama|mas_dako|mas_gamay|labing_dako|labing_gamay)$"), "logical_ops" },
      // {regex("^[\\+\\-\\*/]$"), ARITHMETIC_OPERATOR},
      {regex("^-?[0-9]+$"), INTEGER},
      {regex("^-?[0-9]+.[0-9]+$"), DOUBLE},
      {regex("^\".*\"$"), STRING},
      {regex("^(=)$"), ASSIGN_OPERATOR},
      // {regex("^[a-zA-Z0-9]$"), CHARACTER},
      // { regex("^sulat$"), "output" },
      // { regex("^basa$"), "input" },
      // { regex("^gihimo$"), "loop_start" },
      // { regex("^bukid$"), "loop_end" },
      // { regex("^tapos$"), "if_end" },
      // { regex("^[a-zA-Z0-9.]+$"), "word" },
      // { regex("^[a-zA-Z0-9.]+$"), "" },
      {regex("^[a-zA-Z][a-zA-Z0-9]*(_[a-zA-Z0-9]+)*$"), VAR_NAME},
      {regex("^(::)$"), IN_OUT_OPERATOR},
      // {regex("^(\\(|\\)|:|::|,|.)$"), PUNCTUATION},
      {regex("^(\\+|\\-|\\*|/)$"), ARITHMETIC_OPERATOR},
  };


  for (const auto &pattern : regex_patterns)
  {

    if (regex_match(expr, pattern.first))
    {
      return pattern.second;
    }
    }

  return UNKNOWN;
}

ostream& operator<<(std::ostream& os, const TokenInfo& tokenInfo) {
    os << "Line Number: " << tokenInfo.line_number << "\n";
    os << "Token Type: " << tokenToString(tokenInfo.type) << "\n"; // Adjust based on your enum or use a switch statement
    os << "Lexeme: " << tokenInfo.lexeme << "\n";
    os << "Is Indented: " << (tokenInfo.is_indented ? "true" : "false") << "\n";
    return os;
}