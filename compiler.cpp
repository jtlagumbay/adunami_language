#include <iostream>
#include "tokenEnum.h"
#include "scanner.cpp"

using namespace std;

int main(){
    string file_name = "sample.txt"; // file path of the source code
    try {
      Scanner mScanner(file_name);
      mScanner.start();
      mScanner.printTokenList();
    } catch (const std::runtime_error& e) {
        cerr << "Caught exception: " << e.what() << endl;
    }
    return 0; 

}
