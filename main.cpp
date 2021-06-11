#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

string lexer(char c[]);

int main (){
    //Tokens
    int x[] = {1, 2, 3, 4, 5};
    char separators[] = {'(', ')', ';'};
    char operators[] = {'==', '/', '>', '<', '/=', '+', '-', '*'};
    string keyword[] = {"if", "endif",  "else",  "put", "get", "while", "true",  "false", "boolean", "integer", "begin", "end"};
    
    string myText;
    char c = '';

    //Reads the text file
    FILE * infile;
    infile = fopen("input.txt","r");
    //ifstream infile("input.txt");
    ofstream outfile("output.txt"); 

    if (infile==NULL){
        cout << "Error opening input.txt";}
    else {
        do {
            c = getc (infile);


        } while (c != EOF);
    }



    cout << "token                     lexeme";
    cout << "________________________________";

    return 0;
}

string lexer(char c[]){
    string token ="";

    return token;
}