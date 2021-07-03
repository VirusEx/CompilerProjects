#include "lexer.cpp"

const int outputSwitch = 1;
std::pair<int,string>token;
int lineCount = 1;

// function prototypes
template <class T>
void printLexeme(int i, T lexeme, ofstream& outfile);
std::pair<int,string> getToken(FILE* infile);
void Rat21SU (FILE* infile, ofstream& outfile);
void OptDeclarationList (FILE* infile, ofstream& outfile);
void DeclarationList (FILE* infile, ofstream& outfile);
void BackTraceDeclarationList(FILE* infile, ofstream& outfile);
void Declaration(FILE* infile, ofstream& outfile);
void Qualifier(FILE* infile, ofstream& outfile);
void StatementList(FILE* infile, ofstream& outfile);
void BackTraceStatementList(FILE* infile, ofstream& outfile);
void Statement(FILE* infile, ofstream& outfile);
void Compound(FILE* infile, ofstream& outfile);
void Assign(FILE* infile, ofstream& outfile);
void If(FILE* infile, ofstream& outfile);
void elseState(FILE* infile, ofstream& outfile);
void Put (FILE* infile, ofstream& outfile);
void Get(FILE* infile, ofstream& outfile);
void While(FILE* infile, ofstream& outfile);
void Condition(FILE* infile, ofstream& outfile);
void Relop(FILE* infile, ofstream& outfile);
void Expression(FILE* infile, ofstream& outfile);
void ExpressionPrime(FILE* infile, ofstream& outfile);
void Term(FILE* infile, ofstream& outfile);
void TermPrime(FILE* infile, ofstream& outfile);
void Factor(FILE* infile, ofstream& outfile);
void Primary(FILE* infile, ofstream& outfile);

int main(){

    // testcases
    FILE * testcase1;
    testcase1 = fopen("testcase1.txt","r");
    ofstream output1("A2-output1.txt"); 

    FILE * testcase2;
    testcase2 = fopen("testcase2.txt","r");
    ofstream output2("A2-output2.txt"); 

    FILE * testcase3;
    testcase3 = fopen("testcase3.txt","r");
    ofstream output3("A2-output3.txt"); 

    Rat21SU(testcase1, output1);
	lineCount = 1;
	Rat21SU(testcase2, output2);
    lineCount = 1;
    Rat21SU(testcase3, output3);


    return 0;
}

template <class T>
void printLexeme(int i, T lexeme, ofstream& outfile){
    switch (i){
        case 0:
        break;
        case 1:
        outfile <<"Token:identifier\t\t\t\tLexeme:"  << lexeme  <<std::endl;
        break;
        case 2:
        outfile << "Token:integer\t\t\t\t\tLexeme:"     << lexeme <<std::endl;
        break;
        case 3:
        outfile <<"Token:separator\t\t\t\t\tLexeme:"   << lexeme << std::endl;
        break;
        case 4:
        outfile <<"Token:operator\t\t\t\t\tLexeme:"    << lexeme<< std::endl;
        break;
        case 5:
        outfile <<"Token:keyword\t\t\t\t\tLexeme:"     << lexeme << std::endl;
        break;
        case 6:
        outfile <<"Token:unknown\t\t\t\t\tLexeme:"     << lexeme << std::endl;
        break;
        default:
        outfile << "Error" << lexeme << std::endl;
    }
}


std::pair<int,string> getToken(FILE* infile){
    token=lexer(infile);
    while(token.first==0 && !feof(infile)){
        if(token.second == "13"){
			lineCount++;
		}
        token = lexer(infile);
    }
    return token;
}

void Rat21SU (FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Rat21SU>  ::=   %%  <Opt Declaration List>    <Statement List>  %%" << endl;
    }

    token = getToken(infile);

    if (token.second!="%%"){
        outfile << lineCount <<" |\tSyntax error. This program doesn't start with %%.\nToken\t" << token.second << endl;
        std::exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

	OptDeclarationList(infile, outfile);
	StatementList(infile, outfile);

    if (token.second!="%%"){
        outfile << lineCount <<" |\tSyntax error. Missing %% at the end of the file.\nToken\t" << token.second << endl;
        std::exit(1);
    }

    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    if (feof(infile)==false){
        outfile << "Syntax error. Out of scope." << endl;
        std::exit(1);
    }
    
    
}


void OptDeclarationList (FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;
    }
    if (token.second=="integer" || token.second=="boolean"){
        DeclarationList(infile, outfile);
    }
}


void DeclarationList (FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Declaration List>  ::= <Declaration> ;     |      <Declaration> ; <Declaration List>" << endl;
    }

	Declaration(infile, outfile);
    if (token.second==";"){
        // outfile << lineCount <<" |\tSyntax error. Expecting ';'\nToken\t" << token.second << endl;
        // std::exit(1);
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
	    BackTraceDeclarationList(infile, outfile);
    }
    
}


void BackTraceDeclarationList(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<BackTraceDeclarationList> ::= <declaration List> | <Empty>" << endl;
    }
    if (token.second=="integer" || token.second=="boolean"){
        DeclarationList(infile, outfile);
    }
}


void Declaration(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Declaration> ::=   <Qualifier >  <identifier>" << endl;
    }
    Qualifier(infile, outfile);
    if(token.first!=1){
        outfile << lineCount <<" |\tSyntax error. Expecting an idenifier. \nToken\t" << token.second << endl;
        std::exit(1);
    }

    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    
}


void Qualifier(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Qualifier> ::=  integer |  boolean" << endl;
    }

    if(token.second == "integer" || token.second == "boolean"){
        
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        
    }
    // else{
    //     outfile << lineCount <<" |\tSyntax error. Expecting an integer or boolean. \nToken\t" << token.second << endl;
	// 	std::exit(1);
	// }
    
}


void StatementList(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Statement List> ::=   <Statement>   | <Statement> <Statement List>" << endl;
    }
    Statement(infile, outfile);
    BackTraceStatementList(infile, outfile);
}


void BackTraceStatementList(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<BackTraceStatementList> ::= <Statement List> | <Empty>" << endl;
    }
    if(token.second=="begin" || token.first==1 || token.second=="if" || token.second=="put" || token.second=="get" || token.second=="while"){
        StatementList(infile, outfile);
    }
}


void Statement(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Get>   |   <Put>   |  <While> " << endl;
    }
	Compound(infile, outfile);
	Assign(infile, outfile);
	If(infile, outfile);
	Get(infile, outfile);
	Put(infile, outfile);
	While(infile, outfile);
}


void Compound(FILE* infile, ofstream& outfile){
    if (outputSwitch){
            outfile << "\t<Compound> ::=   begin <Statement List>  end" << endl;
    }
    if (token.second =="begin"){

        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

		StatementList(infile, outfile);

        if(token.second != "end"){
            outfile << lineCount <<" |\tSyntax error. Expecting 'end'. \nToken\t" << token.second << endl;
	        std::exit(1);
	    }
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

        
	}
}


void Assign(FILE* infile, ofstream& outfile){
    if (outputSwitch){ 
            outfile << "\t<Assign> ::=     <Identifier> = <Expression>;" << endl; 
    }
    if (token.first ==1){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

        if (token.second == "="){
            printLexeme(token.first, token.second, outfile);
            token = getToken(infile);

            Expression(infile, outfile);

            if (token.second == ";"){
                printLexeme(token.first, token.second, outfile);
                token = getToken(infile);
                // if (outputSwitch){ 
                //     outfile << "\t<Assign> ::=     <Identifier> = <Expression>;" << endl; 
                //     }
            }      
            else{
                outfile << lineCount <<" |\tSyntax error. Expecting ';'. \nToken\t" << token.second << endl;
                std::exit(1);
            }
        }
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting '='. \nToken\t" << token.second << endl;
            std::exit(1);
        }
        
    }
}

void If(FILE* infile, ofstream& outfile){
    if (outputSwitch){
		outfile << "\t<If> ::= if ( <Condition> ) <Statement >[elseState] endif" << endl;
	}

    if (token.second =="if"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

        if (token.second =="("){
           printLexeme(token.first, token.second, outfile);
           token = getToken(infile);

           Condition(infile, outfile);
           
           if(token.second == ")"){
                printLexeme(token.first, token.second, outfile);
                token = getToken(infile);
                
                Statement(infile, outfile);
		        elseState(infile, outfile);

                if(token.second == "endif"){
                    printLexeme(token.first, token.second, outfile);
                    token = getToken(infile);
                }
                else{
                    outfile << lineCount <<" |\tSyntax error. Expecting 'endif'. \nToken\t" << token.second << endl;
                    std::exit(1);
                }
           }
           else{
               outfile << lineCount <<" |\tSyntax error. Expecting ')'. \nToken\t" << token.second << endl;
                std::exit(1);
           }           
        }
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
            std::exit(1);
        }
    }

    
}


void elseState(FILE* infile, ofstream& outfile){
    if (outputSwitch){
            outfile << "\t[elseState] ::= else <Statement> | epsilon" << endl;
        }     
    if (token.second =="else"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

		Statement(infile, outfile);   
    }
}


void Put (FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Put> ::=    put ( <Identifier> );" << endl;
    }
    if (token.second =="put"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

        if(token.second == "("){
            printLexeme(token.first, token.second, outfile);
            token = getToken(infile);

            if(token.first == 1){
                printLexeme(token.first, token.second, outfile);
                token = getToken(infile);
                if(token.second == ")"){
                    printLexeme(token.first, token.second, outfile);
                    token = getToken(infile);

                    if(token.second == ";"){
                        printLexeme(token.first, token.second, outfile);
                        token = getToken(infile);
                    }
                    else{
						outfile << lineCount << " |\tSyntax error. Expecting ';'. \nToken\t" << token.second << endl;
						std::exit(1);
                    }
                }
                else{
                    outfile << lineCount << " |\tSyntax error. Expecting ')'. \nToken\t" << token.second << endl;
		            std::exit(1);
                }
            }
            else{
                outfile << lineCount <<" |\tSyntax error. Expecting 'identifier'. \nToken\t" << token.second << endl;
                std::exit(1);
            }  
        }
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
            std::exit(1);
        }
    }
}


void Get(FILE* infile, ofstream& outfile){
if (outputSwitch){
	outfile << "\t<Get> ::=    get ( <Identifier> );" << endl;
	}
    if (token.second =="get"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

        if(token.second == "("){
            printLexeme(token.first, token.second, outfile);
            token = getToken(infile);

            if(token.first == 1){
                printLexeme(token.first, token.second, outfile);
                token = getToken(infile);
                if(token.second == ")"){
                    printLexeme(token.first, token.second, outfile);
                    token = getToken(infile);

                    if(token.second == ";"){
                        printLexeme(token.first, token.second, outfile);
						token = getToken(infile);
					}
                    else{
						outfile << lineCount << " |\tSyntax error. Expecting ';'. \nToken\t" << token.second << endl;
						std::exit(1);
                    }
                }
                else{
                    outfile << lineCount << " |\tSyntax error. Expecting ')'. \nToken\t" << token.second << endl;
		            std::exit(1);
                }
            }
            else{
                outfile << lineCount <<" |\tSyntax error. Expecting 'identifier'. \nToken\t" << token.second << endl;
                std::exit(1);
            }  
        }
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
            std::exit(1);
        }
    }
}


void While(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<While> ::=  while ( <Condition>  )  <Statement>  " << endl;
    }
    if (token.second=="while"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        if(token.second=="("){
            printLexeme(token.first, token.second, outfile);
            token = getToken(infile);

			Condition(infile, outfile);
            if(token.second == ")"){
                printLexeme(token.first, token.second, outfile);
                token = getToken(infile);
                
                Statement(infile, outfile);
            }
            else{
                outfile << lineCount <<" |\tSyntax error. Expecting ')'. \nToken\t" << token.second << endl;
                std::exit(1);
            }
		}
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
            std::exit(1);
        }
    }
}


void Condition(FILE* infile, ofstream& outfile){
    if (outputSwitch){
	    outfile << "\t<Condition> ::=     <Expression>  <Relop>   <Expression>" << endl;
	}
    Expression(infile, outfile);
    Relop(infile, outfile);
    Expression(infile, outfile);
}

void Relop(FILE* infile, ofstream& outfile){
    if (outputSwitch){
		    outfile << "\t<Relop> ::=        ==     |    >     |    <     |    /=" << endl;
	}

    if (token.second=="==" || token.second==">" || token.second=="<" || token.second=="/="){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        
    }
    else{
        outfile << lineCount <<" |\tSyntax error. Expecting  '== | > | < | /=  '. \nToken\t" << token.second << endl;
        std::exit(1);
    }
}


void Expression(FILE* infile, ofstream& outfile){
	if (outputSwitch){
		outfile << "\t<Expression> ::= <Term><Expression>’" << endl;
	}
    Term(infile, outfile);
	ExpressionPrime(infile, outfile);
	
}


void ExpressionPrime(FILE* infile, ofstream& outfile){
    if (outputSwitch){
            outfile << "\t<Expression>’ ::=   + <Term><Expression>’|   - <Term><Expression>’" << endl;
    }
    if(token.second == "+" || token.second == "-"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

        Term(infile, outfile);
	    ExpressionPrime(infile, outfile);
    }
}


void Term(FILE* infile, ofstream& outfile){
    if (outputSwitch){
		outfile << "\t<Term> ::= <Factor><Term>’" << endl;
	}
	Factor(infile, outfile);
	TermPrime(infile, outfile);
}


void TermPrime(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Term>’ ::= *  <Factor> <Term>’  |   /  <Factor> <Term>’ | <Empty>" << endl;
        }
    if(token.second == "*" || token.second == "/"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);

        Factor(infile, outfile);
	    TermPrime(infile, outfile);
    }
}


void Factor(FILE* infile, ofstream& outfile){
    if (outputSwitch){
            outfile << "\t<Factor> ::=      -  <Primary>    |    <Primary>" << endl;
        }
    if(token.second == "-"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
		Primary(infile, outfile);
	}
    else{
        Primary(infile, outfile);
        //if (outputSwitch){
        //    outfile << "\t<Factor> ::=      -  <Primary>    |    <Primary>" << endl;
        //}
    }
}


void Primary(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Primary> ::=      <Identifier>  |  <Integer>  |  ( <Expression> )   |  true   |  false  " << endl;
    }
    // id | int | true | false
    if(token.first == 1 || token.first == 2 || token.second == "true" || token.second == "false"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        
    }
    else if(token.second == "("){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        Expression(infile, outfile);

        if(token.second == ")"){
            printLexeme(token.first, token.second, outfile);
            token = getToken(infile);
            // if (outputSwitch){
            //     outfile << "\t<Primary> ::=      <Identifier>  |  <Integer>  |  ( <Expression> )   |  true   |  false  " << endl;
            // }
            
        }
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting  ')'. \nToken\t" << token.second << endl;
            std::exit(1);
        }
    }
    else{
        outfile << lineCount <<" |\tSyntax error. Expecting  '- | identifier | integer | ( | true | false'. \nToken\t" << token.second << endl;
        std::exit(1);
    }
}


    





















