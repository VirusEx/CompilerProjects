#include "lexer.cpp"
#include <stack>
#include <iostream>
#include <iomanip>

const int outputSwitch = 0;
std::pair<int,string>token;
int lineCount = 1;
pair<int, string>save;
std::string INSTR_TABLE[300][3];
int Instr_address = 1;
std::string Symbol_table[50][3];
int memory_address = 10000;
std::stack<int> jumpstack;
std::pair<int, string> OP;
int relop_case;
int numOfSymbol = 1;
int valueType =10;
std::pair<int, string>tempToken;
int whileAddr;

// function prototypes
template <class T>
void printLexeme(int i, T lexeme, ofstream &outfile);
std::pair<int,string> getToken(FILE* infile);
void Rat21SU (FILE* infile, ofstream& outfile);
void OptDeclarationList (FILE* infile, ofstream& outfile);
void DeclarationList (FILE* infile, ofstream& outfile);
void BackTraceDeclarationList(FILE* infile, ofstream& outfile);
void Declaration(FILE* infile, ofstream& outfile);
string Qualifier(FILE* infile, ofstream& outfile);
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
void gen_instr(string op, string oprnd);
int find_type(std::pair<int, string> t);
void add_symbol(std::pair<int, string> t, string type, ofstream& outfile);
string get_address(std::pair<int, string> t, ofstream& outfile);
void print_symbolTable(ofstream& outfile);
void print_instrTable(ofstream& outfile);
void back_patch(int jump_addr);

int main(){

    // testcases
    FILE * testcase1;
    testcase1 = fopen("testcase1.txt","r");
    ofstream output1("A3-output1.txt"); 

    FILE * testcase2;
    testcase2 = fopen("testcase2.txt","r");
    ofstream output2("A3-output2.txt"); 

    FILE * testcase3;
    testcase3 = fopen("testcase3.txt","r");
    ofstream output3("A3-output3.txt"); 

    INSTR_TABLE[0][0] = "address";
    INSTR_TABLE[0][1] = "Op";
    INSTR_TABLE[0][2] = "Oprnd";

    Symbol_table[0][0] = "Identifier";
    Symbol_table[0][1] = "Memory Location";
    Symbol_table[0][2] = "Type";
    
    Rat21SU(testcase1, output1);
    lineCount = 1;
    Instr_address = 1;
    memory_address = 10000;
    numOfSymbol = 1;
    valueType =10;
    Rat21SU(testcase2, output2);
    lineCount = 1;
    Instr_address = 1;
    memory_address = 10000;
    numOfSymbol = 1;
    valueType =10;
    Rat21SU(testcase3, output3);
    lineCount = 1;
    Instr_address = 1;
    memory_address = 10000;
    numOfSymbol = 1;
    valueType = 10;
    

	return 0;
}

template <class T>
void printLexeme(int i, T lexeme, ofstream& outfile){
    //switch (i){
    //    case 0:
    //    break;
    //    case 1:
    //    outfile <<"Token:identifier\t\t\t\tLexeme:"  << lexeme  <<std::endl;
    //    break;
    //    case 2:
    //    outfile << "Token:integer\t\t\t\t\tLexeme:"     << lexeme <<std::endl;
    //    break;
    //    case 3:
    //    outfile <<"Token:separator\t\t\t\t\tLexeme:"   << lexeme << std::endl;
    //    break;
    //    case 4:
    //    outfile <<"Token:operator\t\t\t\t\tLexeme:"    << lexeme<< std::endl;
    //    break;
    //    case 5:
    //    outfile <<"Token:keyword\t\t\t\t\tLexeme:"     << lexeme << std::endl;
    //    break;
    //    case 6:
    //    outfile <<"Token:unknown\t\t\t\t\tLexeme:"     << lexeme << std::endl;
    //    break;
    //    default:
    //    outfile << "Error" << lexeme << std::endl;
    //}
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
    print_instrTable(outfile);
    print_symbolTable(outfile);

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
    string type = Qualifier(infile, outfile);
    if(token.first!=1){
        outfile << lineCount <<" |\tSyntax error. Expecting an idenifier. \nToken\t" << token.second << endl;
        std::exit(1);
    }
    add_symbol(token, type, outfile);

    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    
}


string Qualifier(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << "\t<Qualifier> ::=  integer |  boolean" << endl;
    }
	string type = "Error Type from Qualifier";
	if(token.second == "integer" || token.second == "boolean"){
        if(token.second == "integer"){
            type = "integer";
        }
        else if(token.second == "boolean"){
            type = "boolean";
        }
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        
    }
    // else{
    //     outfile << lineCount <<" |\tSyntax error. Expecting an integer or boolean. \nToken\t" << token.second << endl;
	// 	std::exit(1);
	// }
    return type;
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

        gen_instr("JUMP",to_string(whileAddr));

        if(token.second == "end"){ 
            back_patch(Instr_address);
        }
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
        save = token;
        token = getToken(infile);

        if (token.second == "="){
            printLexeme(token.first, token.second, outfile);
            token = getToken(infile);
            std::pair<int, string> tempToken=token;
            Expression(infile, outfile);
            if (find_type(save)==0){
                if(valueType==2){
                    if(find_type(tempToken)!=0){
                        outfile << "invalid varaible type; integer expected" << std::endl;
                        exit(1);
                    }
                }
                else if(valueType!=0){
					outfile << "invalid value type; integer expected" << std::endl;
                    exit(1);
                }
            }
            else if (find_type(save)==1){
                if(valueType==2){
                    if(find_type(tempToken)!=1){
                        outfile << "invalid varaible type; boolean expected" << std::endl;
                        exit(1);
                    }
                }
                else if(valueType!=1){
                    outfile << "invalid value type; \"true\"|\"false\" expected" << std::endl;
                    exit(1);
                }
            }
            gen_instr("POPM", get_address(save,outfile));

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

        int addr=Instr_address;

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

                back_patch(Instr_address);

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
                gen_instr("PUSHM", get_address(token, outfile));
                token = getToken(infile);

                if(token.second == ")"){
                    printLexeme(token.first, token.second, outfile);
                    token = getToken(infile);

                    if(token.second == ";"){
                        printLexeme(token.first, token.second, outfile);
                        gen_instr("STDOUT","");
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
            gen_instr("STDIN","");
			token = getToken(infile);
            gen_instr("POPM", get_address(token, outfile));

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
        whileAddr = Instr_address;
        gen_instr("LABEL","");
        
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

                // back_patch(Instr_address);
                
                
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
    if (relop_case==1){
        gen_instr("LES","");
        jumpstack.push(Instr_address);
        gen_instr("JUMPZ","");
    }
    else if( relop_case==2){
		gen_instr("GRT", "");
        jumpstack.push(Instr_address);
        gen_instr("JUMPZ","");
	}
    else if( relop_case==3){
        gen_instr("EQU", "");
        jumpstack.push(Instr_address);
        gen_instr("JUMPZ","");
    }

}

void Relop(FILE* infile, ofstream& outfile){
    if (outputSwitch){
		    outfile << "\t<Relop> ::=        ==     |    >     |    <     |    /=" << endl;
	}

    if (token.second=="==" || token.second==">" || token.second=="<" || token.second=="/="){
        printLexeme(token.first, token.second, outfile);

        OP = token;

        if (token.second=="<"){
            relop_case=1;
        }
        else if (token.second==">"){
            relop_case=2;
        }
        else if (token.second=="=="){
            relop_case=3;
        }

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

        gen_instr("ADD","");

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

        gen_instr("MUL","");

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
        if(token.first == 1){
            gen_instr("PUSHM",get_address(token,outfile));
            valueType=2;
        }
        else if (token.first == 2){
            gen_instr("PUSHI",token.second);
            valueType=0;
        }
        else if (token.second=="true"){
			gen_instr("PUSHI","1");
            valueType=1;
        }
        else if(token.second =="false"){
            gen_instr("PUSHI","0");
            valueType=1;
        }
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

void gen_instr(string op, string oprnd){
    INSTR_TABLE[Instr_address][0]=to_string(Instr_address);
    INSTR_TABLE[Instr_address][1]=op;
    INSTR_TABLE[Instr_address][2]=oprnd;
    Instr_address++;
}

int find_type(std::pair<int, string> t){
    for (int i=0; i<numOfSymbol ;i++){
        if (Symbol_table[i][0]==t.second){
            if (Symbol_table[i][2]=="integer"){
                return 0;
        }
            else
                return 1;
        }
    }
    return 2;
}

void add_symbol(std::pair<int, string> t, string type, ofstream& outfile){
    for (int i=0; i<numOfSymbol ;i++){
        if (Symbol_table[i][0]==t.second){
            print_symbolTable(outfile);
            outfile << "Error: Identifier already exist: " << t.second << endl;
            exit(1);
        }
    }
    Symbol_table[numOfSymbol][0] = t.second;
	Symbol_table[numOfSymbol][1] = to_string(memory_address);
    Symbol_table[numOfSymbol][2] = type;
    numOfSymbol++;
    memory_address++;
}

string get_address(std::pair<int, string> t, ofstream& outfile){
    for (int i=1; i<numOfSymbol ;i++){
        if (Symbol_table[i][0]==t.second){
            return Symbol_table[i][1];
        }
    }
	outfile << "Error: Identifier is not declared" << endl;
    return "0";
    // exit(1);
}

void print_symbolTable(ofstream& outfile){
	outfile << "-------------------Symbol Table--------------------------" << std::endl;
	for(int x=0; x<numOfSymbol; x++) 
    {
        for(int y=0; y<3; y++)  
        {
            outfile << std::setw(20) << std::left << Symbol_table[x][y]; 
        }
    outfile << std::endl;  
    }
    outfile << "\n\n\n";
}

void print_instrTable(ofstream& outfile){
    outfile << "-------------------Instruction Table---------------------" << std::endl;
    for(int x=0; x<Instr_address; x++) 
    {
        for(int y=0; y<3; y++)  
        {
            outfile << std::setw(20) << std::left <<INSTR_TABLE[x][y]; 
        }
    outfile << std::endl;
    }
    outfile << "\n\n\n";
}

void back_patch(int jump_addr){
	int addr = jumpstack.top();
	jumpstack.pop();
	INSTR_TABLE[addr][2] = to_string(jump_addr);
}















