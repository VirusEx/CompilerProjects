#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// function prototype
std::pair<int, string> lexer(FILE* infile);
template <class T>
void printToken(int token, T lexeme, ofstream &outfile);
std::pair<int, string> idFSM(char c, FILE* infile);
std::pair<int, string> intFSM(char c, FILE* infile);



const char separators[] = {'(', ')', ';'};
const char op[] = {'=', '>', '<', '+', '-', '*'};
const std::string keyword[] = {"if", "endif", "else", "put", "get", "while", "true", "false", "boolean", "integer", "begin", "end"};



// lexer reads the next lexeme from infile and return the token with lexeme
std::pair<int, string> lexer(FILE* infile){
    string s = "";
	char c = ' ';


	if (infile==NULL){
        cout << "Error opening input.txt";
        exit(1);
	}
    else {
        c = getc(infile);

        //Compares with the separators
        if (c=='%'){           
        c = getc(infile);
        if (c == '%'){
            return std::pair<int, string>(3, "%%");
        }
        else {
            ungetc(c, infile);
            return std::pair<int, string>(6, "%");
            }
        }
        for (auto it: separators){
            if (c == it){
                s.push_back(c);
                return std::pair<int, string>(3, s);
            }
        }

        //delete comments             
        if (c=='/'){
            c = getc(infile);
            if (c == '*')
            {
                loopagain:
                do {
                    c = getc(infile);
                } while(c!='*');
                c = getc(infile);
                if (c == '/'){
                    c = getc(infile);
                    goto endComment;
                }
                else
                    goto loopagain;
            }
            else if (c == '='){
                return std::pair<int, string>(4, "/=");
            }
            else {
                ungetc(c,infile);
                return std::pair<int, string>(4, "/");
            }
        }
        endComment:

        //Compares with operators
        if (c=='='){           
        c = getc(infile);
        if (c == '='){
            return std::pair<int, string>(4, "==");
        }
        else {
            ungetc(c, infile);
            return std::pair<int, string>(4, "=");
            }
        }
        

        
        for (auto it: op){
            if (c == it){
                s.push_back(c);
                return std::pair<int, string>(4, s);
            }
        }

        //intFSM
        if(isdigit(c)){
            std::pair<int, string> result = intFSM(c, infile);
            return result;
        }

        //idFSM
        if(isalpha(c)){
            std:pair<int, string> result = idFSM(c, infile);
            for (auto it: keyword){
                if (it == result.second){
					return std::pair<int, string>(5, result.second);
                }
            }

            return result;
        }
        s.push_back(c);
        if (c < 33){
			return std::pair<int, string>(0, to_string(char(c)));
        }
		return std::pair<int, string>(6, s);
    }
    return std::pair<int, string>(0,"");
}

template <class T>
void printToken(int token, T lexeme, ofstream& outfile){
    switch (token){
        case 0:
        break;
        case 1:
        outfile << "identifier\t\t\t\t\t"  << lexeme << std::endl;
        break;
        case 2:
        outfile << "integer\t\t\t\t\t\t"     << lexeme << std::endl;
        break;
        case 3:
        outfile << "separator\t\t\t\t\t"   << lexeme << std::endl;
        break;
        case 4:
        outfile << "operator\t\t\t\t\t"    << lexeme << std::endl;
        break;
        case 5:
        outfile << "keyword\t\t\t\t\t\t"     << lexeme << std::endl;
        break;
        case 6:
        outfile << "unknown\t\t\t\t\t\t"     << lexeme << std::endl;
        break;
        default:
        outfile << "Error" << lexeme << std::endl;
    }
}

std::pair<int, string> idFSM(char c, FILE* infile){
    string s;
    s.push_back(c);
    c=getc(infile);
    while(isalpha(c)||isdigit(c)){
        s.push_back(c);
        c=getc(infile);
    }
    ungetc(c, infile);

	int table[5][2] = {{1, 4}, {2, 3}, {2, 3}, {3, 3}, {4, 4}};
    {
        int state = 0;
        for(char& c : s) {
            if(isalpha(c))
            {
                state = table[state][0];
			}
            else if(isdigit(c))
            {
				state = table[state][1];
            }
            else
                return std::pair<int, string>(6,s);
        }
        if(state == 1 || state == 2 || state == 3)
        {
			return std::pair<int, string>(1,s);
		}
        else
			return std::pair<int, string>(6,s);
	}
}

std::pair<int, string> intFSM(char c, FILE* infile){
    string s;
    s.push_back(c);
    c=getc(infile);
    while(isdigit(c)){
        s.push_back(c);
        c=getc(infile);
    }
    ungetc(c, infile);
    
	int table[2][1] = {{1},{1}};
    int state = 0;
    for(char& c : s) {
        if(isdigit(c))
        {
		    state = table[state][0];
        }
        else
            return std::pair<int, string>(6,s);
	}
    if(state == 1)
    {
		return std::pair<int, string>(2,s);
	}
    return std::pair<int, string>(6,s);
}