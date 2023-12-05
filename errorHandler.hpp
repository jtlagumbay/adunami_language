#pragma once
#include <iostream>
using namespace std;
// Enum of possible error codes
enum ErrorCode
{
  SYNTAX,       // Not starting with sa adm, or ending with hmn, unindentified token
  SEMANTIC,     // Misuse of operators
  TYPE,         // Arithmetic operations on incompatible data types
  REFERENCE,    // Undefined variables,
  RUNTIME,      // Division by zero
  INPUT_OUTPUT, // File not found.
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

class DevMsg{
  string trace; // What file and function gave the error
  string possible_cause; 
  public:
    DevMsg(string, string);
    friend ostream &operator<<(ostream &os, const DevMsg &dev){
      os << "\nTrace: " << dev.trace << "\nPossible Cause: " << dev.possible_cause <<"\n";
    };
};

class Error{
  public:
    ErrorCode error_code;
    string client_msg;
    const DevMsg& dev_msg;
  
    Error(ErrorCode, string,  const DevMsg&);
    friend ostream &operator<<(ostream &os, const Error &error){
    {
      os << ErrorCodeToString(error.error_code) << " ERROR: " << error.client_msg;
      return os;
    }
}
};

// void Error::debug() const{
//   cout << ErrorCodeToString(error_code) << " ERROR: " << dev_msg;
// }

Error::Error(ErrorCode code, string m_client_msg, const DevMsg& m_dev_msg)
    : error_code(code), client_msg(m_client_msg), dev_msg(m_dev_msg)
{
}

DevMsg::DevMsg(string m_trace, string m_possible_cause){
  trace = m_trace;
  possible_cause = m_possible_cause;
}