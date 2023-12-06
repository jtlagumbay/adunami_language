#pragma once
#include <iostream>
using namespace std;
// Enum of possible error codes
enum ErrorCode
{
  SYNTAX,       // Not starting with sa adm, or ending with hmn, unindentified token, Unexpected token
  SEMANTIC,     // Misuse of operators, 
  TYPE,         // Arithmetic operations on incompatible data types
  REFERENCE,    // Undefined variables,
  RUNTIME,      // Division by zero
  INPUT_OUTPUT, // File not found.
  ASM_GENERATION // Related to asm generation
};

const char* ErrorCodeToString(ErrorCode t) {
    switch (t) {
        case SYNTAX: return "SYNTAX";
        case SEMANTIC: return "SEMANTIC";
        case TYPE: return "TYPE";
        case REFERENCE: return "REFERENCE";
        case RUNTIME: return "RUNTIME";
        case INPUT_OUTPUT: return "INPUT_OUTPUT";
        default: "ErrorCodeToString ERROR";
    }
}

class Error{
  public:
    ErrorCode error_code;
    string client_msg; // Maoy makita sa compiler error 
    string trace; // What file and function gave the error
    string possible_cause;
  
    Error(ErrorCode, string,  string, string);
    void debug();
    friend ostream &operator<<(ostream &os, const Error &error){
    {
      os << ErrorCodeToString(error.error_code) << " ERROR: " << error.client_msg;
      return os;
    };
    
}
};

void Error::debug() {
  cout <<endl<< "----------DEBUG MESSAGE----------" << endl;
  cout << ErrorCodeToString(error_code) << " ERROR" << "\nTrace: " << trace << "\nPossible Cause: " << possible_cause <<"\n";
  cout << endl<<"** NOTE: Delete all debug on production **" << endl<<endl;
}

Error::Error(ErrorCode code, string m_client_msg, string m_trace, string m_possible_cause) : 
  error_code(code), 
  client_msg(m_client_msg), 
  trace(m_trace), possible_cause(m_possible_cause){
}
