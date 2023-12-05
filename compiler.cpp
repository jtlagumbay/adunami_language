#include <iostream>
#include "scanner.cpp"

using namespace std;

int main(){
    string file_name = "sample.txt"; // file path of the source code
    try {
      Scanner mScanner(file_name);
      mScanner.start();
      mScanner.printTokenList();
    } catch (const Error& e) {
        cerr << e << endl;
    }
    return 0; 

}
