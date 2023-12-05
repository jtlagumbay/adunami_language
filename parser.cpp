#include <iostream>

#include "tokenEnum.hpp"
#include "scanner.cpp"

using namespace std;

class Parser{
  vector<TokenInfo> tokens;                 // Tokens from parser
  vector<TokenInfo>::iterator curr_token;   // Current token being investigated
  vector<TokenInfo>::iterator end_token;   // Current token being investigated
  
  void moveNext();                              // Move to next token
  TokenInfo peekNext();
  bool isEnd();

  void expect(Token);
  void expectStatement();
  void expectInstruction();

public:
  Parser(vector<TokenInfo>&);
  void start();
};

int main() {
    string file_name = "sample.txt"; // file path of the source code
    
    try {

      Scanner m_scanner(file_name);
      vector<TokenInfo> tokens = m_scanner.start();
      // m_scanner.printTokenList();

      Parser m_parser(tokens);
      m_parser.start();

    } catch (Error& e) {
        cerr << e << endl;
        e.debug();
    }
    return 0;
}


Parser::Parser(vector<TokenInfo>& m_tokens){
  tokens = m_tokens;
  curr_token = m_tokens.begin();
  end_token = m_tokens.end();

}

void Parser::moveNext(){
  if(!isEnd()){
    curr_token++;
  }
}

TokenInfo Parser::peekNext(){
  auto next_token = next(curr_token);
  if (next_token != tokens.end()){
    return *next_token;
  } else {
    return TokenInfo{
        -1,
        -1,
        UNKNOWN,
        "END OF TOKEN LIST",
        false
        };
  }
}

void Parser::start(){
  expectInstruction();
}

void Parser::expectInstruction(){
  if(isEnd()){
    return;
  }
  TokenInfo m_token = *curr_token;

  switch(m_token.type){
    case OUTPUT:
      expect(OUTPUT);
      expect(IN_OUT_OPERATOR);
      expectStatement();
      break;
    case INPUT:
      expect(INPUT);
      expect(IN_OUT_OPERATOR);
      expect(VAR_NAME);
      break;
    case DECLARE:
      expect(DECLARE);
      expect(VAR_NAME);
      if(!isEnd() and (*curr_token).type == ASSIGN_OPERATOR){
        expect(ASSIGN_OPERATOR);
        expectStatement();
      }
      break;
    default:
      throw Error(
          SYNTAX,
          "Unknown instruction on line "+to_string(m_token.line_number)+":"+to_string(m_token.token_number)+". Check Adunami syntax.",
          "parser.cpp > Parser::expectInstruction()",
          "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
  }
  expectInstruction();
}

void Parser::expect(Token expected_token){
  if(isEnd()){
    return;
  }
  TokenInfo m_token = *curr_token;


  if(m_token.type!=expected_token){
    string expected_token_string = tokenToString(expected_token);
    string error_msg = "Expected " + expected_token_string + " at line " + to_string(m_token.line_number)+".";
    throw Error(
      SYNTAX,
      error_msg,
      "parser.cpp > Parser::expect(Token expected_token)",
      "User error or token not scanned properly."
      );
  }

  moveNext();
}

void Parser::expectStatement(){
  if(isEnd()){
    return;
  }

  TokenInfo m_token = *curr_token;

  switch (m_token.type)
  {
  case VAR_NAME:
    expect(VAR_NAME);
    break;
  case STRING:
    expect(STRING);
    break;
  case CHARACTER:
    expect(CHARACTER);
    break;
  case INTEGER:
    expect(INTEGER);
    break;
  case DOUBLE:
    expect(DOUBLE);
    break;
  default:
    throw Error(
      SYNTAX,
      "Assignment or declaration statement expected. Check adunami syntax.",
      "parser.cpp > Parser::expectStatement()",
      "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
  }

  if ((*curr_token).type==ARITHMETIC_OPERATOR){
    expect(ARITHMETIC_OPERATOR);
    expectStatement();
  }
  
}

bool Parser::isEnd(){
  return curr_token == end_token;
}