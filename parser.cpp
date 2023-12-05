#include <iostream>

#include "tokenEnum.hpp"
#include "scanner.cpp"

using namespace std;

class Parser{
  vector<TokenInfo> tokens;                 // Tokens from parser
  vector<TokenInfo>::iterator curr_token;   // Current token being investigated
  void moveNext();                              // Move to next token
  TokenInfo peekNext();

public:
  Parser(vector<TokenInfo>);
  void start();
};

int main() {
    string file_name = "sample.txt"; // file path of the source code
    try {
      Scanner m_scanner(file_name);
      vector<TokenInfo> tokens = m_scanner.start();
      Parser m_parser(tokens);

      // for (auto it = tokens.begin(); it != tokens.end(); ++it) {
      //   // Peek at the next element without advancing the iterator
      //   auto nextIt = next(it);
        
      //   // Check if we are not at the end of the vector
      //   if (nextIt != tokens.end()) {
      //       cout << "Current: " << *it << ", Next: " << *nextIt << endl;
      //   } else {
      //       cout << "Current: " << *it << ", No next element" << endl;
      //   }
      // }

    } catch (Error& e) {
        cerr << e << endl;
        e.debug();
    }
    return 0;
}


Parser::Parser(vector<TokenInfo> m_tokens){
  tokens = m_tokens;
  curr_token = m_tokens.begin();
  cout << *curr_token << endl;
}

void Parser::moveNext(){
  ++curr_token;
}

TokenInfo Parser::peekNext(){
  auto next_token = next(curr_token);
  if (next_token != tokens.end()){
    return *next_token;
  } else {
    return TokenInfo{
        -1,
        UNKNOWN,
        "END OF TOKEN LIST",
        false
        };
  }
}