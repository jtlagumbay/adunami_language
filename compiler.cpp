#include <iostream>
#include "scanner.cpp"

using namespace std;

int main(){
    string file_name = "sample.txt"; // file path of the source code
    try {
      Scanner mScanner(file_name);
      mScanner.start();
      mScanner.printTokenList();
    } catch (Error& e) {
        cerr << e << endl;
        e.debug();
    }
    return 0; 

}
