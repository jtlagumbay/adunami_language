#include <iostream>
#include <cstdlib>

#include "tokenEnum.hpp"
#include "scanner.cpp"
#include "asmInstructions.cpp"
#include "symbolTable.cpp"

using namespace std;

class Parser{
  /****** TOKENS AND ITERATOR ********/
  vector<vector<TokenInfo>> tokens;                 // Tokens per line
  vector<vector<TokenInfo>>::iterator curr_line;   // Current line
  vector<vector<TokenInfo>>::iterator end_line;   // End of the line
  vector<TokenInfo>::iterator curr_token;   // Start of token per line
  vector<TokenInfo>::iterator end_token;   // End of token per line

  /****** TOKENS MOVEMENT HELPER ********/
  void moveNext();                              // Move to next token
  void moveNextLine();                              // Move to next line
  TokenInfo peekNext();
  bool isEnd();
  bool isEndLine();

  /****** PARSING TOKENS ********/
  void expect(Token);
  void expectStatement();
  void expectInstruction();
  void expectAssign();
  void parseInstruction(vector<TokenInfo>::iterator);

  /****** ASM FILE WRITING ******/
  string adm_file_name; // adm_file
  string asm_file_name;
  ofstream asm_file_writer;
  void initAsmFile();
  void appendData(AsmDataType, string, string);
  void appendLoadAddress(AsmRegisters, string);
  void appendLoadImmediate(AsmRegisters, int);
  void appendSyscall();

  /****** SYMBOL TABLE ******/
  SymbolTable symbol_table;

public:
  Parser(vector<vector<TokenInfo>>&, string);
  ~Parser();
  void start();
  void generateAsm();
  void printSymbolTable();
};

int main() {
  string file;
  cout << "Enter file name (without file extension): ";
  cin >> file;

  string file_name = file + ".adm"; // file path of the source code
  string asmCommand = "java -jar mars.jar " + file + ".asm";
  
  try {

    Scanner m_scanner(file_name);
    vector<vector<TokenInfo>> tokens = m_scanner.start();
    m_scanner.printTokenList(); // comment later on

    Parser m_parser(tokens, file_name);
    m_parser.start();
    m_parser.printSymbolTable(); // comment later on

    // run asm
    int result = system(asmCommand.c_str());
  } catch (Error& e) {
      cerr << e << endl;
      e.debug();
  }
  return 0;
}


Parser::Parser(vector<vector<TokenInfo>>& m_tokens, string m_file_name){

  tokens = m_tokens;
  tokens.erase(
    remove_if(tokens.begin(), tokens.end(),
        [](const std::vector<TokenInfo>& innerVec) {
            return innerVec.empty();
        }
    ),
    tokens.end()
  );
  curr_line = tokens.begin();
  end_line = tokens.end();
  adm_file_name = m_file_name;

  initAsmFile();
}

Parser::~Parser(){
  if(asm_file_writer.is_open()){
    asm_file_writer.close();
  }
}

void Parser::printSymbolTable(){
  symbol_table.printSymbols();
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
                  << ".text\n";
}

void Parser::moveNextLine(){
  if(!isEndLine()){
    curr_line++;
  }
}
void Parser::moveNext(){
  if(!isEnd()){
    curr_token++;
  }
}

void Parser::start(){


  if((*curr_line++)[0].type==PROG_BEGIN){
    expect(PROG_BEGIN);
  } else {
    throw Error(
      SYNTAX,
      "Unrecognized program. Adunami files should start with \'sa adm:\'",
      "parser.cpp > Parser::start",
      "User error or Wa natarong og save."
    );
  }
  if ((*--end_line)[0].type != END)
  {
    throw Error(
      SYNTAX,
      "Program did not end properly. Adunami files should end with \'hmn\'",
      "parser.cpp > Parser::start",
      "User error or Naputol ang file pag save."
    );
  }

  while(curr_line!=end_line && (*curr_line)[0].type!=END){
    if((*curr_line)[0].depth<1){
      throw Error(
      SYNTAX,
      "Expecting indentation on line "+to_string((*curr_line)[0].line_number),
      "parser.cpp > Parser::start",
      "User error or Wrong ang count sa depth."
    );
    }
    expectInstruction();
    moveNextLine();
  }

  if(curr_line!=end_line){
    throw Error(
      SYNTAX,
      "Instructions after hmn. Adunami files should end with \'hmn\'",
      "parser.cpp > Parser::start",
      "User error or Naputol ang file pag save."
    );
  }

  expect(END);
  appendLoadImmediate(V0, 10);
  appendSyscall();
 


}

void Parser::expectInstruction(){

  if(isEndLine()){
    return;
  }
  
  curr_token = (*curr_line).begin();
  end_token = (*curr_line).end();
  TokenInfo curr_token_info = *curr_token;
 
  switch(curr_token_info.type){
    case OUTPUT:
      expect(OUTPUT);
      expect(IN_OUT_OPERATOR);
      if((*curr_token).type==VAR_NAME){
        string m_var_name = (*curr_token).lexeme;
        Symbol m_symbol = symbol_table.getSymbol(m_var_name);
        expect(VAR_NAME);

      } else if((*curr_token).type==STRING){
        TokenInfo string_token = *curr_token;

        expect(STRING);

        string to_print_label = "print_" + to_string(string_token.line_number) + "_" + to_string(string_token.token_number);

        appendData(ASCIIZ, to_print_label, string_token.lexeme);
        appendLoadAddress(A0, to_print_label);
        appendLoadImmediate(V0, 4);
        appendSyscall();

      } else if((*curr_token).type==INTEGER){
        expect(INTEGER);

      } else if((*curr_token).type==DOUBLE){
        expect(INTEGER);
      } else if((*curr_token).type==CHARACTER){
        expect(INTEGER);
      } else {
        throw Error(
          SYNTAX,
          "Expects variable name or value on or before line "+to_string((*--curr_token).line_number)+":"+to_string((*--curr_token).token_number+1)+". Check Adunami syntax.",
          "parser.cpp > Parser::expectInstruction() > case OUTPUT > else",
          "User error or wala na properly identify or separate ang token.");
      }
      break;
    case INPUT:
      expect(INPUT);
      expect(IN_OUT_OPERATOR);
      {
        string m_var_name = (*curr_token).lexeme;
        Symbol m_symbol = symbol_table.getSymbol(m_var_name, *curr_token);
      }
      expect(VAR_NAME);
      break;
    case DECLARE:
      expect(DECLARE);
      expect(VAR_NAME);
      symbol_table.addSymbol(UNKNOWN, (*--curr_token).lexeme);
      curr_token++;
      if(!isEnd() and (*curr_token).type == ASSIGN_OPERATOR){
        expectAssign();
      }
      break;
    case END:
      expect(END);
      return;
    case VAR_NAME:
      // string m_var
      expect(VAR_NAME);
      symbol_table.getSymbol((*--curr_token).lexeme); // gacheck if ni exist ang variable
      curr_token++;
      expectAssign();
      break;
    default:
      throw Error(
          SYNTAX,
          "Unknown instruction on line "+to_string((*curr_token).line_number)+":"+to_string((*curr_token).token_number)+". Check Adunami syntax.",
          "parser.cpp > Parser::expectInstruction() > default",
          "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
  }
  
  return;
}

void Parser::expectAssign(){
  expect(ASSIGN_OPERATOR);
  if(isEnd()){
    throw Error(
      SYNTAX,
      "Expecting a variable or value on line "+to_string((*curr_token).line_number)+":"+to_string((*--curr_token).token_number+1),
      "parser.cpp > Parser::expectInstruction() > expectAssign",
      "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
  }
  expectStatement();
      
}

void Parser::expect(Token expected_token){
  if(isEnd()){
    return;
  }

  if((*curr_token).type!=expected_token){
    string expected_token_string = tokenToString(expected_token);
    string error_msg = "Expected " + expected_token_string + " at line " + to_string((*curr_token).line_number)+":"+to_string((*--curr_token).token_number+1)+".";
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

  TokenInfo m_token = *curr_token;

  switch (m_token.type)
  {
  case VAR_NAME:
    {
      string m_var_name = (*curr_token).lexeme;
      symbol_table.getSymbol(m_var_name, *curr_token);
    }
    expect(VAR_NAME);
    break;
  case STRING:
    // if((*curr_token).){
      
    // }
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
      "Expecting a variable or value on line "+to_string((*curr_token).line_number)+":"+to_string((*--curr_token).token_number+1),
      "parser.cpp > Parser::expectStatement()",
      "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
  }

  if ((*curr_token).type==ARITHMETIC_OPERATOR){
    expect(ARITHMETIC_OPERATOR);
    if(isEnd()){
      throw Error(
      SYNTAX,
      "Expecting a variable or value on line "+to_string((*curr_token).line_number)+":"+to_string((*--curr_token).token_number+1),
      "parser.cpp > Parser::expectStatement() >  if ((*curr_token).type==ARITHMETIC_OPERATOR)",
      "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
    }
    expectStatement();
  }
  
}

bool Parser::isEnd(){
  return curr_token == end_token;
}
bool Parser::isEndLine(){
  return curr_line == end_line;
}

void Parser::generateAsm(){

}

void Parser::appendData(AsmDataType data_type, string data_name, string data_value){
  string to_append="\t";
  if(data_type==ASCIIZ){
    to_append += 
        data_name 
        + ":\t" 
        + asmDataToString(data_type) 
        + "\t"
        + data_value;
  } else {
    to_append += 
        data_name 
        + ":\t" 
        + asmDataToString(data_type) 
        + "\t"
        + data_value;
  }

  asm_file_writer.close();

  ifstream temp_file_reader(asm_file_name);

  if (!temp_file_reader.is_open()) {
    throw Error(
      ASM_GENERATION,
      "Error generating asm code.",
      "parser.cpp > Parser::appendData()",
      "error sa pagcreate og temporary input file");
  }

  vector<std::string> temp_lines;
  string temp_curr_line;

  while (getline(temp_file_reader, temp_curr_line)) {
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

void Parser::appendLoadAddress(AsmRegisters reg, string label){
  asm_file_writer<<"\tla "<<asmRegToString(reg)<<", "<<label<<endl;
}

void Parser::appendLoadImmediate(AsmRegisters reg, int value){
  asm_file_writer<<"\tli "<<asmRegToString(reg)<<", "<<value<<endl;

}

void Parser::appendSyscall(){
  asm_file_writer<<"\tsyscall "<<endl;
}


