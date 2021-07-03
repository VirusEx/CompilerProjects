#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "lexer.cpp"

using namespace std;


int main(){
    //Testcase 1
    FILE * testcase1;
    testcase1 = fopen("testcase1.txt","r");
    ofstream output1("output1.txt"); 
    output1 << "token\t\t\t\t\t\tlexeme" << std::endl;
    output1 << "-------------------------------------------------------------------------------" << std::endl;
    while(!feof(testcase1)){
        std::pair<int, string> result = lexer(testcase1);
        printToken(result.first, result.second, output1);
    }

    //Testcase 2
    FILE * testcase2;
    testcase2 = fopen("testcase2.txt","r");
    ofstream output2("output2.txt"); 
    output2 << "token\t\t\t\t\t\tlexeme" << std::endl;
    output2 << "-------------------------------------------------------------------------------" << std::endl;
    while(!feof(testcase2)){
        std::pair<int, string> result = lexer(testcase2);
        printToken(result.first,result.second, output2);
    }
    
    //Testcase 3
    FILE * testcase3;
    testcase3 = fopen("testcase3.txt","r");
    ofstream output3("output3.txt"); 
    output3 << "token\t\t\t\t\t\tlexeme" << std::endl;
    output3 << "-------------------------------------------------------------------------------" << std::endl;
    while(!feof(testcase3)){
        std::pair<int, string> result = lexer(testcase3);
        printToken(result.first,result.second, output3);
    }

    return 0;

}
