#include <iostream>

#include "tokenEnum.hpp"
#include "scanner.cpp"
#include "asmInstructions.cpp"

using namespace std;

class Parser{
  /****** TOKENS AND ITERATOR ********/
  vector<TokenInfo> tokens;                 // Tokens from parser
  vector<TokenInfo>::iterator curr_token;   // Current token being investigated
  vector<TokenInfo>::iterator end_token;   // Current token being investigated

  /****** TOKENS MOVEMENT HELPER ********/
  void moveNext();                              // Move to next token
  TokenInfo peekNext();
  bool isEnd();

  /****** PARSING TOKENS ********/
  void expect(Token);
  void expectStatement();
  void expectInstruction();

  /****** ASM FILE WRITING ******/
  string adm_file_name; // adm_file
  string asm_file_name;
  ofstream asm_file_writer;
  void initAsmFile();
  void appendData(AsmDataType, string, string);
  void appendText();

public:
  Parser(vector<TokenInfo>&, string);
  ~Parser();
  void start();
  void generateAsm();
};

int main() {
    string file_name = "sample.txt"; // file path of the source code
    
    try {

      Scanner m_scanner(file_name);
      vector<TokenInfo> tokens = m_scanner.start();
      // m_scanner.printTokenList();

      Parser m_parser(tokens, file_name);
      m_parser.start();

    } catch (Error& e) {
        cerr << e << endl;
        e.debug();
    }
    return 0;
}


Parser::Parser(vector<TokenInfo>& m_tokens, string m_file_name){
  tokens = m_tokens;
  curr_token = m_tokens.begin();
  end_token = m_tokens.end();
  adm_file_name = m_file_name;

  initAsmFile();
}

Parser::~Parser(){
  if(asm_file_writer.is_open()){
    asm_file_writer.close();
  }
}

void Parser::initAsmFile(){

  istringstream iss(adm_file_name);
  getline(iss, asm_file_name, '.');
  asm_file_name.append(".asm");

  asm_file_writer.open(asm_file_name, ios::trunc);

  if(!asm_file_writer.is_open()){
    throw Error(
      INPUT_OUTPUT,
      "Error creating asm file",
      "parser.cpp > Parser::initAsmFile()",
      "Pagopen or create sa asm file."
    );
  }

  asm_file_writer << ".data\n\n\n\n"
                  << ".text\n\n\n\n";
  appendData(ASCIIZ, "hello", "hello, world!");
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
  // cout << m_token << endl;
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

void Parser::generateAsm(){

}

void Parser::appendData(AsmDataType data_type, string data_name, string data_value){
  string to_append = 
      data_name 
      + ":\t" 
      + asmDataToString(data_type) 
      + "\t\""
      + data_value 
      + "\"\n";
  cout << to_append;

  ifstream temp_file_reader(asm_file_name);

  if (!temp_file_reader.is_open()) {
    throw Error(
      ASM_GENERATION,
      "Error generating asm code.",
      "parser.cpp > Parser::appendData()",
      "error sa pagcreate og temporary input file");
  }
  cout << "here "<<asm_file_name << endl;

  vector<std::string> temp_lines;
  string temp_curr_line;

  while (getline(temp_file_reader, temp_curr_line)) {
    cout << "debug: " << temp_curr_line;
    temp_lines.push_back(temp_curr_line);
    if(temp_curr_line==".data"){
      temp_lines.push_back(to_append);
    }
  }

  temp_file_reader.close();
  asm_file_writer.open(asm_file_name, ios::trunc);
  for (const auto& updatedLine : temp_lines) {
      asm_file_writer << updatedLine << std::endl;
  }
}