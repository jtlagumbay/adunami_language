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
  int depth;
  Token type;
  string lexeme;
};

class Scanner {
  private:
    ifstream adm_file_reader;
    string adm_file;
    // vector<TokenInfo> token_list;
    vector<vector<TokenInfo>> token_list;
    vector<TokenInfo> scanLine(const string&, int, int);
    Token checkTokenType(const string&);
    bool hasIndentation(const std::string&);
    int countDepth(const std::string&);
    string stripWhiteSpace(string);

  public:
    Scanner(string file_name);
    ~Scanner();
    vector<vector<TokenInfo>> start();
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

int Scanner::countDepth(const string& line){
  int space = 0;
  for (char c : line)
  {
    if (!isspace(c)){
      break;
    } else if (c == '\t' || c == ' '){
      space++;
    }
  }
  return space / 2;
}

vector<vector<TokenInfo>> Scanner::start(){
  string stream_line="";
  int line_number = 0;
  bool program_finished = false;

  // Check if adm is started properly
  do{
    getline(adm_file_reader, stream_line);
    line_number++;
  } while (stream_line == "");

  // Gilahi ang sa adm kay arun di maapil sa getline na separated by space kay dapat di buwag ang "sa" og "adm" 
  if(stream_line !="sa adm:"){
    throw Error(
      SYNTAX,
      "Unrecognized program. Adunami files should start with \'sa adm:\'",
      "scanner.cpp > Scanner::start",
      "User error or Naputol ang file pag save."
    );
  } else {
    vector<TokenInfo> token_list_this_line;
    token_list_this_line.push_back(
      TokenInfo{
        line_number++,
        1,
        countDepth(stream_line),
        PROG_BEGIN,
        stream_line
      }
    );
    token_list.push_back(token_list_this_line);
  }

  while(getline(adm_file_reader, stream_line)){
    // remove all comments
    string line = regex_replace(stream_line, regex("\\*\\*\\s*([^*]+)\\s*\\*\\*"), "");
    // add space to print for easier shit
    line = regex_replace(line, regex("::|=|\\+|\\-|/|\\*"), " $& ");

    if(line.empty()){
      line_number++;
      continue;
    }

    token_list.push_back(
      scanLine(
        line,
        countDepth(stream_line),
        line_number++
      )
    );
  }

  return token_list;
}


void Scanner::printTokenList(){
  cout << "------ START OF TOKEN LIST ------" << endl;
  for (const auto& token_per_line : token_list) {
    for (const auto& element : token_per_line)
      cout << element.line_number  <<":"<<element.token_number<<" " << element.depth << " " << tokenToString(element.type) << " " << element.lexeme << endl;
  }
  cout << "------ END OF TOKEN LIST ------" << endl << endl;
}

vector<TokenInfo> Scanner::scanLine(const string& inputLine, int depth, int line_number){
  vector<TokenInfo> tokens_this_line;
  int token_number = 0;
  string expr;
  Token token;
  string prevExpr;
  stringstream SS(inputLine);

  while (getline(SS, expr, ' ')) {
    if (expr.empty()) {
        continue;
    }

    string temp_expr = expr;
    
    if (expr == "-" && prevExpr=="=") {
       
        getline(SS, expr);
        temp_expr += "" + expr;
        expr = temp_expr;

        tokens_this_line.push_back(TokenInfo{
          line_number,
          ++token_number,
          depth,
          INTEGER,
          expr
        });

        continue;
    }

    if (expr == "mas" || expr == "labing") {
      string logical_expr = expr;
      while (getline(SS, expr, ' ')) {
        temp_expr += " " + expr;
        logical_expr += " " + expr;
        break;
      }

      tokens_this_line.push_back(TokenInfo{
        line_number,
        ++token_number,
        depth,
        LOGICAL_EXPRESSION,
        logical_expr
      });

      continue;
    }

    if (isdigit(expr.front()) && expr.back() == ':') {
      string digit = expr.substr(0, expr.size() - 1);
      string colon = ":";
      
      tokens_this_line.push_back(TokenInfo{
        line_number,
        ++token_number,
        depth,
        INTEGER,
        digit
      });

      tokens_this_line.push_back(TokenInfo{
        line_number,
        ++token_number,
        depth,
        PUNCTUATION,
        colon
      });

      tokens_this_line.push_back(TokenInfo{
        line_number,
        ++token_number,
        depth,
        IF_STATEMENT,
        "\t"
      });

      continue;
    }

    if (expr == "kondili:") {
      tokens_this_line.push_back(TokenInfo{
        line_number,
        ++token_number,
        depth,
        ELSE,
        "kondili"
      });

      tokens_this_line.push_back(TokenInfo{
        line_number,
        ++token_number,
        depth,
        PUNCTUATION,
        ":"
      });

      tokens_this_line.push_back(TokenInfo{
        line_number,
        ++token_number,
        depth,
        ELSE_STATEMENT,
        "\t"
      });

      continue;
    }

    if (expr.front() == '\"') {
      while (getline(SS, expr, ' ')) {
        temp_expr += " " + expr;
        if (expr.back() == '\"') {
          break;
        }   
      }
      expr = temp_expr;   
    } 

    prevExpr = expr;

    token = checkTokenType(expr);

    tokens_this_line.push_back(TokenInfo{
      line_number,
      ++token_number,
      depth,
      token,
      expr});
  }
  return tokens_this_line;
}


string Scanner::stripWhiteSpace(string input){
  string line = regex_replace(input, regex("^\\s+"), "");
  return line;
}


Token Scanner::checkTokenType(const string& expr) {

  vector<pair<regex, Token>> regex_patterns = {
      {regex("^(sa adm:)$"), PROG_BEGIN},
      {regex("^(hmn)$"), END},
      {regex("^(isuwat)$"), OUTPUT},
      {regex("^(isulod)$"), INPUT},
      {regex("^(kuptan)$"), DECLARE},
      {regex("^(kon)$"), IF},
      {regex("^(kondili)$"), ELSE},
      {regex("^(sama|mas dako|mas gamay|labing dako|labing gamay)$"), LOGICAL_EXPRESSION},      
      {regex("^-?[0-9]+$"), INTEGER},
      {regex("^-?[0-9]+.[0-9]+$"), DOUBLE},
      {regex("^\".*\"$"), STRING},
      {regex("^(=)$"), ASSIGN_OPERATOR},
      {regex("^[a-zA-Z][a-zA-Z0-9]*(_[a-zA-Z0-9]+)*$"), VAR_NAME},
      {regex("^(::)$"), IN_OUT_OPERATOR},
      {regex("^(\\+|\\-|\\*|/)$"), ARITHMETIC_OPERATOR},
      {regex("^(\\(|\\)|:|,|.)$"), PUNCTUATION},
      // {regex("^[a-zA-Z0-9]$"), CHARACTER},
      // { regex("^[a-zA-Z0-9.]+$"), "word" },
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
    os << "Token Number: " << tokenInfo.token_number << "\n";
    os << "Depth: " << tokenInfo.depth << "\n";
    os << "Token Type: " << tokenToString(tokenInfo.type) << "\n"; // Adjust based on your enum or use a switch statement
    os << "Lexeme: " << tokenInfo.lexeme << "\n";\
    return os;
}