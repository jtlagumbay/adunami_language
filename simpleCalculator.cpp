#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include "errorHandler.hpp"
using namespace std;

int calculate(string);

// int main (){
//   cout << calculate("27-4") << endl;
//   cout << calculate("-28-4") << endl;
//   cout << calculate("27*4") << endl;
//   cout << calculate("27+4") << endl;
//   cout << calculate("-27/0") << endl;

//   return 0;
// }

/***
 * 
 * Calculates Arithmetic operation.
 * Limited to two terms only.
 * 
 * 
 * ***/ 
int calculate(string expr){
  int answer, term1, term2;
  string operation;
  string m_expr = regex_replace(expr, regex("\\+|\\-|/|\\*"), " $& ");

  istringstream iss(m_expr);
  string token;
  getline(iss, token, ' ');


  if(token == ""){
    getline(iss, token, ' ');
  }

  if(token == "-"){
    getline(iss, token, ' ');
    // cout << m_expr << " | "<< token + " pp" << endl;
    term1 = -(stoi(token));
    // cout << m_expr << " | "<< term1 + " term1" << endl;
  }
  else {
    // cout << m_expr << " | "<< token + " aa" << endl;

    term1 = stoi(token);
  }

  if(token == ""){
    getline(iss, token, ' ');
  }

  getline(iss, token, ' ');
  operation = token;
  
  if(token == ""){
    getline(iss, token, ' ');
  }


  getline(iss, token, ' ');
  // cout << m_expr << " | "<< token + " cc" << endl;
  term2 = stoi(token);

  if(operation == "-"){
    answer = term1 - term2;
  }
  else if(operation == "+"){
    answer = term1 + term2;
  }
  else if(operation == "*"){
    answer = term1 * term2;
  }
  else if(operation == "/"){
    if(term2==0){
      throw Error(
      RUNTIME,
      "Cannot divide by zero: "+expr+" Check Adunami supported features.",
      "simpleCalcular.cpp > calculate > else",
      "Operations kay dili + - * /"
       );
    }
    answer = term1 / term2;
  } else {
    throw Error(
      SYNTAX,
      "Operation not supported: "+expr+" Check Adunami supported features.",
      "simpleCalcular.cpp > calculate > else",
      "Operations kay dili + - * /"
       );
  }

  return answer;
}