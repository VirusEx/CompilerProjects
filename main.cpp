#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;


//Function prototypes
int DFSM(string s);
template <class T>
void printToken(int token, T lexeme, ofstream & outfile);
int idFSM(string s);
int intFSM(string s);
void lexer(FILE * infile, ofstream & outfile);

//Globle variables
const char separators[] = {'(', ')', ';'};
const char op[] = {'=', '>', '<', '+', '-', '*'};
const std::string keyword[] = {"if", "endif",  "else",  "put", "get", "while", "true",  "false", "boolean", "integer", "begin", "end"};

int main (){
    //Testcase 1
    FILE * testcase1;
    testcase1 = fopen("testcase1.txt","r");
    ofstream output1("output1.txt"); 
    lexer(testcase1, output1);

    //Testcase 2
    FILE * testcase2;
    testcase2 = fopen("testcase2.txt","r");
    ofstream output2("output2.txt"); 
    lexer(testcase2, output2);

    //Testcase 3
    FILE * testcase3;
    testcase3 = fopen("testcase3.txt","r");
    ofstream output3("output3.txt"); 
    lexer(testcase3, output3);

    
    return 0;
}

template <class T>
void printToken(int token, T lexeme, ofstream & outfile){
    switch (token){
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

int DFSM(string s){


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

void lexer(FILE * infile, ofstream & outfile){
    string lexeme; //To store lexeme, push c
    char c = ' ';
    string s = "";
    int tokenNum;

    
    outfile << "token\t\t\t\t\t\tlexeme" << std::endl;
    outfile << "-------------------------------------------------------------------------------" << std::endl;


    if (infile==NULL){
        outfile << "Error opening input.txt";
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
                    tokenNum = DFSM(s);
                    printToken(tokenNum,s,outfile);
                    s.clear();
                    }
            }

            //Ignore white space
            if (c == ' '){
                if (s.length()>1){
                    s.pop_back();
				    tokenNum = DFSM(s);
                    printToken(tokenNum,s,outfile);
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
                        tokenNum = DFSM(s);
                        printToken(tokenNum,s,outfile);
                        s.clear();
                    }
                    printToken(3,"%%",outfile);
                }
                else {
                    ungetc(c, infile);
                    }
            }
            for (auto it: separators){
                if (c == it){
                    s.pop_back();
					if(s.length()>0){
                        tokenNum = DFSM(s);
                        printToken(tokenNum,s,outfile);
                        s.clear();
                    }
                    printToken(3, it,outfile);
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
                    printToken(4,"/=",outfile);
				}
                else {
                    printToken(4, '/',outfile);
                }
            }
            endComment:

            //Compares with operators
            for (auto it: op){
                if (c == it){
                    tokenNum = DFSM(s);
                    s.clear();
                    printToken(4, it,outfile);
				}
            }

        } while (c != EOF);
    }
}