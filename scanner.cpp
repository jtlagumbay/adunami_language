#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// class Scanner{
//   private:

//   public:
//     void Scanner();
// }
string token_list[100];
int size = 0;
void scan(string line);
string getOperand(string expr);

int main(){
  string file_name = "sample.txt"; // file path of the source code
  ifstream file_reader;
  file_reader.open(file_name);
  string line;
  while(getline(file_reader, line)){
    scan(line); // scan per line
  }
  file_reader.close();
}

void scan(string line){
  if(line == "sa adm:"){
    token_list[size++] = "PROG_BEGIN";
    return;
  } 

  string keyword = getKeyword(line);
  
  if (keyword != "NO_KEYWORD"){
    
  } else {

  }

  string var;

  stringstream SS(line);

  while(getline(SS, var, ' ')){
    
  }
}

string getKeyword(string expr){
  string keywords[] = {"kuptan", "kon", "kondili", "isuwat", "isulod"};
  int idx;

  for(int i = 0; i < sizeof(keywords); i++) {
      if(expr.find(keywords[i])!= string::npos) { //npos = no position, meaning naay keyword
        return keywords[i];
      }
  }

  return "NO_KEYWORD";
}

string getOperand(string expr){
  string operands[] = {"::", ":", "+", "-", "*", "/", "="};
  int idx;

  for(int i = 0; i < sizeof(operands); i++) {
      if(expr.find(operands[i])!= string::npos) { //npos = no position, meaning naay operand
        return operands[i];
      }
  }

  return "NO_OPS";
}