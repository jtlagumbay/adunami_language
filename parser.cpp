#include <iostream>
#include <cstdlib>

#include "tokenEnum.hpp"
#include "scanner.cpp"
#include "asmInstructions.cpp"
#include "symbolTable.cpp"
#include "simpleCalculator.cpp"


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
  void expectStatement(string);
  void expectInstruction();
  void expectAssign(string);
  void parseInstruction(vector<TokenInfo>::iterator);

  /****** ASM FILE WRITING ******/
  string adm_file_name; // adm_file
  string asm_file_name;
  ofstream asm_file_writer;
  void initAsmFile();
  void appendData(AsmDataType, string, string);
  void appendLoadWord(AsmRegisters, string);
  void appendLoadAddress(AsmRegisters, string);
  void appendLoadImmediate(AsmRegisters, int);
  void appendMove(AsmRegisters, AsmRegisters);
  void appendAdd(AsmRegisters, AsmRegisters, AsmRegisters);
  void appendAddI(AsmRegisters, AsmRegisters, int);
  void appendJal(string);
  void appendBeq(AsmRegisters, int, string);
  void appendBne(AsmRegisters, int, string);
  void appendBranchName(string);
  void appendJump(string);
  void appendSyscall();
  void printInt(TokenInfo, string);
  void printStr(TokenInfo, string);

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
  // cout << "Enter file name (without file extension): ";
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
    m_parser.generateAsm();
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
 

  return;
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
        TokenInfo token = *curr_token;
        string m_var_name = (*curr_token).lexeme;
        Symbol m_symbol = symbol_table.getSymbol(m_var_name);
        expect(VAR_NAME);

        if(m_symbol.type == INTEGER){
          printInt(token, m_symbol.value);
        } else if(m_symbol.type == STRING){
          printStr(token, m_symbol.value);
        } else if(m_symbol.type == USER_INPUT){
          string temp_key = m_symbol.var_name + to_string(token.line_number) + to_string(token.token_number);

          appendLoadAddress(A0, m_symbol.var_name);
          appendMove(S2, A0);
          appendJal("adm_remove_last_new_line");
          appendMove(A0, S2);
          appendLoadImmediate(V0, 4);
          appendSyscall();
        } else if (m_symbol.type == VAR_NAME){

          Symbol m_var_symbol = symbol_table.getSymbol(m_symbol.value);

          if(m_var_symbol.type==ARITHMETIC_EXPRESSION){
            int integer_to_print = calculate(m_var_symbol.value);
            printInt(*--curr_token, to_string(integer_to_print));
            curr_token++;
          } else if (m_var_symbol.type==STRING){
            printStr(*curr_token, m_var_symbol.value);
          } else if (m_var_symbol.type==INTEGER){
            printInt(*curr_token, m_var_symbol.value);
          } else {
            throw Error(SEMANTIC, "Variable can only be of type integer or string or arithmetic expression", "parser.cpp > void Parser::expectInstruction(){ else if (m_symbol.type == VAR_NAME) ", "Most probably uncaught na case.");
          }


        } else if (m_symbol.type == ARITHMETIC_EXPRESSION){
          symbol_table.printSymbols();
          int result = calculate(m_symbol.value);
          printInt(token, to_string(result));
        } else {
          throw Error(SEMANTIC, "Variable can only be of type integer or string", "parser.cpp > void Parser::expectInstruction(){ >  case OUTPUT: > if((*curr_token).type==VAR_NAME){ > else", "Most probably uncaught na case.");
        }
      } else if((*curr_token).type==STRING){
        TokenInfo string_token = *curr_token;

        expect(STRING);

        printStr(string_token, string_token.lexeme);

      } else if((*curr_token).type==INTEGER){
        TokenInfo int_token = *curr_token;

        expect(INTEGER);

        printInt(int_token, int_token.lexeme);

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
        expect(VAR_NAME);
        
        AsmRegisters m_reg = symbol_table.giveUnusedRegister();

        symbol_table.editSymbol(USER_INPUT, m_var_name,"", m_reg);

        appendData(SPACE, m_var_name, "128");
        appendLoadAddress(A0, m_var_name);

        // Saving
        appendLoadAddress(m_reg, m_var_name);
        appendLoadAddress(A1, "128");
        appendLoadImmediate(V0, 8);
        appendSyscall();

        // Checking if integer
        appendMove(A2, m_reg);
        appendAddI(V0, V0, 0); // If Integer or not
        appendAddI(V1, V1, 0); // Returns the Integer if integer
        appendJal("adm_check_type");
        appendMove(m_reg, V1);

        
      }

      break;
    case DECLARE:
      {
        expect(DECLARE);
        string m_var_name = (*curr_token).lexeme;
        expect(VAR_NAME);
        symbol_table.addSymbol(UNKNOWN, m_var_name);
        if(!isEnd() and (*curr_token).type == ASSIGN_OPERATOR){
          expectAssign(m_var_name);
        }
      }
      break;
    case END:
      expect(END);
      return;
    case VAR_NAME:
      {
        string m_var_name = (*curr_token).lexeme;
        expect(VAR_NAME);
        symbol_table.resetSymbol(*curr_token,m_var_name);
        expectAssign(m_var_name);
      }
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

void Parser::expectAssign(string m_var_name){
  expect(ASSIGN_OPERATOR);
  if(isEnd()){
    throw Error(
      SYNTAX,
      "Expecting a variable or value on line "+to_string((*curr_token).line_number)+":"+to_string((*--curr_token).token_number+1),
      "parser.cpp > Parser::expectInstruction() > expectAssign",
      "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
  }
  expectStatement(m_var_name);
      
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

void Parser::expectStatement(string m_var_name){

  TokenInfo m_token = *curr_token;

  Symbol m_symbol = symbol_table.getSymbol(m_var_name, *curr_token);

  string m_lexeme = (*curr_token).lexeme;
  string m_symbol_value;

  switch (m_token.type){
    case VAR_NAME:
      {
        cout << *curr_token << endl;
        Symbol m_var_symbol = symbol_table.getSymbol((*curr_token).lexeme, *curr_token);
        int var_value = calculate(m_var_symbol.value);
        m_lexeme = to_string(var_value);

        expect(VAR_NAME);
      }
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
        "Expecting a variable or value on line "+to_string((*curr_token).line_number)+":"+to_string((*--curr_token).token_number+1),
        "parser.cpp > Parser::expectStatement()",
        "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
  }

  if(m_symbol.type == UNKNOWN){ // meaning mao pay pag declare sa variable so ang data type sa variable kay ang data type sa sunod na tokem
    symbol_table.editSymbol(m_token.type, m_var_name, m_symbol.value+m_lexeme);
  } else {
    symbol_table.editSymbol(m_symbol.type, m_var_name, m_symbol.value+m_lexeme);
  }

  if(isEnd()){
    return;
  }

  if ((*curr_token).type==ARITHMETIC_OPERATOR){
    string m_operator = (*curr_token).lexeme;
    Symbol m_symbol = symbol_table.getSymbol(m_var_name, *curr_token);

    expect(ARITHMETIC_OPERATOR);

    symbol_table.editSymbol(ARITHMETIC_EXPRESSION, m_var_name, m_symbol.value + m_operator);

    if(isEnd()){
      throw Error(
      SYNTAX,
      "Expecting a variable or value on line "+to_string((*curr_token).line_number)+":"+to_string((*--curr_token).token_number+1),
      "parser.cpp > Parser::expectStatement() >  if ((*curr_token).type==ARITHMETIC_OPERATOR)",
      "Either wala na tarong separate ang tokens, or wala na tarong identify ang tokens.");
    }
    expectStatement(m_var_name);
  }

  
}

bool Parser::isEnd(){
  return curr_token == end_token;
}
bool Parser::isEndLine(){
  return curr_line == end_line;
}

void Parser::generateAsm(){
  asm_file_writer
      << "\tli   $v0, 10  \n"
      << "\tsyscall \n\n\n\n"
      << "adm_check_type:\n\tlbu $s1, 0($a2)\n\tbeq $s1, 10, adm_is_int\n\tblt $s1, 48, adm_is_string\n\tbgt $s1, 57, adm_is_string\n\taddi $a2, $a2, 1\n\tj adm_check_type\n\nadm_is_int:\n\tli $v0, 0\n\tjr $ra\n\nadm_is_string:\n\tli $v0, 1\n\tjr $ra\n\nadm_remove_last_new_line:\n\tlbu $s1, 0($a0)\n\tbeq $s1, 10, exit_adm_remove_last_new_line\n\taddi $a0, $a0, 1\n\tj adm_remove_last_new_line\n\nexit_adm_remove_last_new_line:\n\tli $t0, 0\n\tsb $t0, 0($a0)\n\tjr $ra\n\n";

  asm_file_writer.close();
}

void Parser::appendData(AsmDataType data_type, string data_name, string data_value){
  string to_append="\t";

  to_append += 
      data_name 
      + ":\t" 
      + asmDataToString(data_type) 
      + "\t"
      + data_value;

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
      asm_file_writer << updatedLine << endl;
  }
}

void Parser::appendLoadWord(AsmRegisters reg, string label){
  asm_file_writer<<"\tlw "<<asmRegToString(reg)<<", "<<label<<endl;
}

void Parser::appendLoadAddress(AsmRegisters reg, string label){
  asm_file_writer<<"\tla "<<asmRegToString(reg)<<", "<<label<<endl;
}

void Parser::appendLoadImmediate(AsmRegisters reg, int value){
  asm_file_writer<<"\tli "<<asmRegToString(reg)<<", "<<value<<endl;

}

void Parser::appendSyscall(){
  asm_file_writer<<"\tsyscall "<<endl<<endl;

}

void Parser::printInt(TokenInfo token, string val) {
  string int_label = "print_" + to_string(token.line_number) + "_" + to_string(token.token_number);
  appendData(WORD, int_label, val);
  appendLoadWord(A0, int_label);
  appendLoadImmediate(V0, 1);
  appendSyscall();
}

void Parser::printStr(TokenInfo token, string val) {
  string to_print_label = "print_" + to_string(token.line_number) + "_" + to_string(token.token_number);

  appendData(ASCIIZ, to_print_label, val);
  appendLoadAddress(A0, to_print_label);
  appendLoadImmediate(V0, 4);
  appendSyscall();
}

void Parser::appendMove(AsmRegisters dest, AsmRegisters src){
  asm_file_writer << "\tmove " << asmRegToString(dest) << ", " << asmRegToString(src) << endl;
}

void Parser::appendAdd(AsmRegisters dest, AsmRegisters src1, AsmRegisters src2){
  asm_file_writer << "\tadd " << asmRegToString(dest) << ", " << asmRegToString(src1) << ", " << asmRegToString(src2) << endl;
}
void Parser::appendAddI(AsmRegisters dest, AsmRegisters src, int immediate){
  asm_file_writer << "\taddi " << asmRegToString(dest) << ", " << asmRegToString(src) << ", " << to_string(immediate) << endl;
}
void Parser::appendJal(string branch){
  asm_file_writer << "\tjal " + branch <<endl<< endl;
}

void Parser::appendBeq(AsmRegisters src, int immediate, string branch){
  asm_file_writer<<"\tbeq "<<asmRegToString(src)<<", "<<to_string(immediate)<<", "<<branch<<endl;

}
void Parser::appendBne(AsmRegisters src, int immediate, string branch){
  asm_file_writer<<"\tbnq "<<asmRegToString(src)<<", "<<to_string(immediate)<<", "<<branch<<endl;

}
void Parser::appendBranchName(string branch){
  asm_file_writer <<"\n"<< branch << ":" << endl;
}
void Parser::appendJump(string branch){
  asm_file_writer << "\tj "<< branch << endl;
}