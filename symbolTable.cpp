#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "tokenEnum.hpp"
#include "asmInstructions.cpp"
#include "errorHandler.hpp"

using namespace std;

struct Symbol {
  Token type; // if UNKNOWN, meaning uninitialized pa ang variable
  string var_name;
  string value;
  AsmRegisters reg; // If zero gani nakaassign, meaning ana walay register nga naka assign
};

class SymbolTable{
  vector<Symbol> symbols;
  vector<Symbol>::iterator searchSymbol(string);

public: 
  void addSymbol(Token m_type, string m_var_name, string m_value = "", AsmRegisters m_reg = ZERO);
  void editSymbol(Token m_type, string m_var_name, string m_value, AsmRegisters m_reg = ZERO);
  void printSymbols();
  void resetSymbol(TokenInfo m_token_info, string m_var_name); // if magreassign sa value, ireset daan
  Symbol getSymbol(string m_var_name, TokenInfo m_token_info =  
        TokenInfo{
          -1,
          -1,
          -1,
          UNKNOWN,
          ""
        }
  );
  AsmRegisters giveUnusedRegister();
};

ostream& operator<<(std::ostream& os, const Symbol& m_symbol) {
    os << tokenToString(m_symbol.type) << "\t" << m_symbol.var_name << "\t" << m_symbol.value << "\t" << asmRegToString(m_symbol.reg);
    return os;
}

void SymbolTable::printSymbols(){
  cout << "------ START OF SYMBOL TABLE ------" << endl;
  for (const auto& symbol : symbols){
    cout << symbol <<endl;
  }
  cout << "------ END OF SYMBOL TABLE ------" << endl << endl;
}

vector<Symbol>::iterator SymbolTable::searchSymbol(string m_var_name){
  auto it = find_if(symbols.begin(), symbols.end(), 
    [m_var_name](const Symbol &m_symbol)
      { return m_symbol.var_name == m_var_name; });

  return it;
}

// Token m_type, string m_var_name, string m_value = "", AsmRegisters m_reg = ZERO
void SymbolTable::addSymbol(Token m_type, string m_var_name, string m_value, AsmRegisters m_reg){

  if((m_type == CHARACTER || m_type == INTEGER || m_type == DOUBLE) && m_value==""){
    string m_type_str = tokenToString(m_type);
    throw Error(
        SEMANTIC,
        m_type_str + " \'" + m_var_name + "\' cannot be empty.",
        "symbolTable.cpp > SymbolTable::addSymbol > if((m_type == CHARACTER || m_type == INTEGER || m_type == DOUBLE) && m_value==\"\")",
        "Cannot be user error. Wala na tarong og pasa ang parameter. Ga pasa og data type pero walay value gi pasa.");
  }

  if(!(m_type == CHARACTER || m_type == INTEGER || m_type == DOUBLE || m_type == STRING || m_type == UNKNOWN)){
    throw Error(
        SEMANTIC,
        "Variable \'" + m_var_name + "\' can only be character, integer, double, or string.",
        "symbolTable.cpp > SymbolTable::addSymbol > if(!(m_type == CHARACTER || m_type == INTEGER || m_type == DOUBLE || m_type == STRING || m_type == UNKNOWN))",
        "Cannot be user error. Wala na tarong og pasa ang parameter.");
  }

  auto it = searchSymbol(m_var_name);

  if(it != symbols.end()){
    throw Error(
      SEMANTIC,
      "Variable \'"+ m_var_name + "\' has already been used.",
      "symbolTable.cpp > SymbolTable::addSymbol > if(it != symbols.end())",
      "User error or wa na tarong store ang symbol.");
  } else {
    symbols.push_back(
        Symbol{
            m_type,
            m_var_name,
            m_value,
            m_reg});
  }

}

void SymbolTable::editSymbol(Token m_type, string m_var_name, string m_value, AsmRegisters m_reg)
{
  auto it = searchSymbol(m_var_name);

  if((m_type == CHARACTER || m_type == INTEGER || m_type == DOUBLE || m_type == ARITHMETIC_EXPRESSION || m_type == VAR_NAME) && m_value==""){
    string m_type_str = tokenToString(m_type);
    throw Error(
        SEMANTIC,
        m_type_str + " \'" + m_var_name + "\' cannot be empty.",
        "symbolTable.cpp > SymbolTable::editSymbol > if((m_type == CHARACTER || m_type == INTEGER || m_type == DOUBLE) && m_value==\"\")",
        "Cannot be user error. Wala na tarong og pasa ang parameter. Ga pasa og data type pero walay value gi pasa.");
  }

  if(!(m_type == CHARACTER || m_type == INTEGER || m_type == DOUBLE || m_type == STRING || m_type == ARITHMETIC_EXPRESSION || m_type == VAR_NAME || m_type == USER_INPUT)){
    throw Error(
        SEMANTIC,
        "Cannot initialize variable \'" + m_var_name + "\'.",
        "symbolTable.cpp > SymbolTable::editSymbol > if(m_type == UNKNOWN)",
        "Cannot be user error. Wala na tarong og pasa ang parameter. Kung maginitialize, dapat nahibaw-an na ang data type daan.");
  }

  if(it == symbols.end()){
    throw Error(
      SEMANTIC,
      "Variable \'" + m_var_name + "\' does not exist.",
      "symbolTable.cpp > SymbolTable::editSymbol > if(it == symbols.end())",
      "User error or wa na store ang symbol before.");
  } else {
    it->type = m_type;
    it->value = m_value;
    if(m_reg!=ZERO){
      it->reg = m_reg;
    }
  }
}

Symbol SymbolTable::getSymbol(string m_var_name, TokenInfo m_token_info){
  auto it = searchSymbol(m_var_name);
  
  if(it!=symbols.end()){
    return *it;
  } else {
    string msg = "Variable \'" + m_var_name + "\' does not exist.";
    if(m_token_info.line_number != -1){
      string check_line = " Check line " + to_string(m_token_info.line_number) + ":" +to_string( m_token_info.token_number) + ".";
      msg.append (check_line);
    }
    throw Error(
      SEMANTIC,
      msg,
      "symbolTable.cpp > Symbol SymbolTable::getSymbol > else",
      "User error or wa na store ang symbol before.");
  }
}

void SymbolTable::resetSymbol(TokenInfo m_token_info, string m_var_name){
  auto it = searchSymbol(m_var_name);
  
  if(it!=symbols.end()){
    it->reg = ZERO;
    it->type = UNKNOWN;
    it->value = "";
  } else {
    string msg = "Variable \'" + m_var_name + "\' does not exist.";
    if(m_token_info.line_number != -1){
      string check_line = " Check line " + to_string(m_token_info.line_number) + ":" +to_string( m_token_info.token_number) + ".";
      msg.append (check_line);
    }
    throw Error(
      SEMANTIC,
      msg,
      "symbolTable.cpp > Symbol SymbolTable::getSymbol > else",
      "User error or wa na store ang symbol before.");
  }
}

AsmRegisters SymbolTable::giveUnusedRegister(){
   static vector<AsmRegisters> unusedRegisters = {
        T0, T1, T2, T3, T4, T5, T6, T7,
        T8, T9
    };

    if (unusedRegisters.empty())
    {
        // All registers are used, handle this case as needed (throw an exception, return a special value, etc.)
        // For simplicity, let's return ZERO, but you might want to handle this differently.
        throw Error(RUNTIME, "No more memory.", "symbolTable.cpp > smRegisters SymbolTable::giveUnusedRegister() > if (unusedRegisters.empty())", "Limitation sa implementation. Nagamit na tanan register");
    }

    // Get the first unused register
    AsmRegisters result = unusedRegisters.back();

    // Remove it from the list of unused registers
    unusedRegisters.pop_back();

    return result;
}









