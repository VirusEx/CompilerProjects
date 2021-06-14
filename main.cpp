#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;


//Function prototypes
int lexer(string s);
template <class T>
void printToken(int token, T lexeme);
int idFSM(string s);
int intFSM(string s);

//Globle variables
const char separators[] = {'(', ')', ';'};
const char op[] = {'=', '>', '<', '+', '-', '*'};
const std::string keyword[] = {"if", "endif",  "else",  "put", "get", "while", "true",  "false", "boolean", "integer", "begin", "end"};

int main (){

    string lexeme; //To store lexeme, push c
    char c = ' ';
    string s = "";
    int tokenNum;

    //Reads the text file
    FILE * infile;
    infile = fopen("input.txt","r");
    ofstream outfile("output.txt"); 

    std::cout << "token\t\t\t\t\t\tlexeme" << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    if (infile==NULL){
        std::cout << "Error opening input.txt";
	}
	else {
        do {
            c = getc (infile);

            //If the char in c is not operators, separators, and not comments. Keep reading the next char and store into string s
                s.push_back(c);
            //Ignore newline
            if (c == '\n' || c == '\r' || c == '\t'){
                if(s.length()==1){
                    s.clear();
                }
                else {
                    s.pop_back();
                    tokenNum = lexer(s);
                    printToken(tokenNum,s);
                    s.clear();
                    }
            }

            //Ignore white space
            if (c == ' '){
                if (s.length()>1){
                    s.pop_back();
				    tokenNum = lexer(s);
                    printToken(tokenNum,s);
                }
                //cout << "TokenNum:" << tokenNum << ". String:" << s;
                s.clear();
            }
            
            //Compares with the separators   four%%asd = four %%  asd
            if (c=='%'){           
                c = getc(infile);
                
				if (c == '%'){
					s.pop_back();
                    if(s.length()>0){
                        tokenNum = lexer(s);
                        printToken(tokenNum,s);
                        s.clear();
                    }
                    printToken(3,"%%");
                }
                else {
                    ungetc(c, infile);
                    }
            }
            for (auto it: separators){
                if (c == it){
                    s.pop_back();
					if(s.length()>0){
                        tokenNum = lexer(s);
                        printToken(tokenNum,s);
                        s.clear();
                    }
                    printToken(3, it);
				}
            }
    
            //delete comments             
            if (c=='/'){
                c = getc(infile);
				if (c == '*')
                {
                    s.clear();
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
                    printToken(4,"/=");
				}
                else {
                    printToken(4, '/');
                }
            }
            endComment:

            //Compares with operators
            for (auto it: op){
                if (c == it){
                    tokenNum = lexer(s);
                    s.clear();
                    printToken(4, it);
				}
            }


           
				//std::cout << c << ',';

				/*if (find / ){
                if (find =){ /= is op}
                else if (find *){ /* is commented}; -> we read until we find ;
                }*/

				//Compare with the integers

				//Compare with the keywords if it is a identifier
				/*for (auto it: keyword){
                if (c == it){
                    printToken(5, it);
                }
            }*/


        } while (c != EOF);
        
    }
    return 0;
}

template <class T>
void printToken(int token, T lexeme){
    switch (token){
        case 1:
        std::cout << "identifier\t\t\t\t\t"  << lexeme << std::endl;
        break;
        case 2:
        std::cout << "integer\t\t\t\t\t\t"     << lexeme << std::endl;
        break;
        case 3:
        std::cout << "separator\t\t\t\t\t"   << lexeme << std::endl;
        break;
        case 4:
        std::cout << "operator\t\t\t\t\t"    << lexeme << std::endl;
        break;
        case 5:
        std::cout << "keyword\t\t\t\t\t\t"     << lexeme << std::endl;
        break;
        case 6:
        std::cout << "unknown\t\t\t\t\t\t"     << lexeme << std::endl;
        break;
        default:
        std::cout << "Error" << lexeme << std::endl;
    }
}

int lexer(string s){


    // FSM for id
    if (idFSM(s)==1){
        for(auto it: keyword){
            if (s == it){
                return 5;
            }
        }
		return 1;
    }
    else if (intFSM(s)==1){
        return 2;
    }
    return 6;
}



int idFSM(string s){
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
                return 0;
		}
        if(state == 1 || state == 2 || state == 3)
        {
			return 1;
		}
        else
			return 0;
	}
}

int intFSM(string s){
	int table[2][1] = {{1},{1}};
    int state = 0;
    for(char& c : s) {
        if(isdigit(c))
        {
		    state = table[state][0];
        }
        else
            return 0;
	}
    if(state == 1)
    {
		return 1;
	}
    return 0;
}




/* Test cases

* * * *
integer123
/ * * /
/* * * / /* 

/=      => one op 
/  =    => two op
$       => --unknown
12t2    => unknown


123inte



*/