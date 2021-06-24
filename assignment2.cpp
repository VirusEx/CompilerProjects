#include "lexer.cpp"

const int outputSwitch = 1;
std::pair<int,string>token;
int lineCount = 1;

// function prototypes
template <class T>
void printLexeme(int i, T lexeme, ofstream& outfile);
std::pair<int,string> getToken(FILE* infile);
int Rat21SU (FILE* infile, ofstream& outfile);
int OptDeclarationList (FILE* infile, ofstream& outfile);
int DeclarationList (FILE* infile, ofstream& outfile);
int BackTraceDeclarationList(FILE* infile, ofstream& outfile);
int Declaration(FILE* infile, ofstream& outfile);
int Qualifier(FILE* infile, ofstream& outfile);
int StatementList(FILE* infile, ofstream& outfile);
int BackTraceStatementList(FILE* infile, ofstream& outfile);
int Statement(FILE* infile, ofstream& outfile);
int Compound(FILE* infile, ofstream& outfile);
int Assign(FILE* infile, ofstream& outfile);
int If(FILE* infile, ofstream& outfile);
int elseState(FILE* infile, ofstream& outfile);
int Put (FILE* infile, ofstream& outfile);
int Get(FILE* infile, ofstream& outfile);
int While(FILE* infile, ofstream& outfile);
int Condition(FILE* infile, ofstream& outfile);
int Relop(FILE* infile, ofstream& outfile);
int Expression(FILE* infile, ofstream& outfile);
int ExpressionPrime(FILE* infile, ofstream& outfile);
int Term(FILE* infile, ofstream& outfile);
int TermPrime(FILE* infile, ofstream& outfile);
int Factor(FILE* infile, ofstream& outfile);
int Primary(FILE* infile, ofstream& outfile);

int main(){
    FILE * testcase1;
    testcase1 = fopen("testcase2.txt","r");
    ofstream output1("output-new.txt"); 

    int x = Rat21SU(testcase1, output1);

    return 0;
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

template <class T>
void printLexeme(int i, T lexeme, ofstream& outfile){
    switch (i){
        case 0:
        break;
        case 1:
        outfile << "Token:identifier\t\t\t\tLexeme:"  << lexeme << std::endl;
        break;
        case 2:
        outfile << "Token:integer\t\t\t\t\tLexeme:"     << lexeme << std::endl;
        break;
        case 3:
        outfile << "Token:separator\t\t\t\t\tLexeme:"   << lexeme << std::endl;
        break;
        case 4:
        outfile << "Token:operator\t\t\t\t\tLexeme:"    << lexeme << std::endl;
        break;
        case 5:
        outfile << "Token:keyword\t\t\t\t\tLexeme:"     << lexeme << std::endl;
        break;
        case 6:
        outfile << "Token:unknown\t\t\t\t\tLexeme:"     << lexeme << std::endl;
        break;
        default:
        outfile << "Error" << lexeme << std::endl;
    }
    
    
}


int Rat21SU (FILE* infile, ofstream& outfile){
    outfile << "Rat21SU" << endl;
    token = getToken(infile);
    
    if (token.second!="%%"){
        outfile << lineCount <<" |\tSyntax error. This program doesn't start with %%.\nToken\t" << token.second << endl;
        exit(1);
    }

    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);
    if (!OptDeclarationList(infile, outfile)){
        outfile << "Error101" << endl;
        return 0;
    }

    if (!StatementList(infile, outfile)){
        outfile << "Syntax error. No Statement List" << endl;
        return 0;
    }

    if (token.second!="%%"){
        outfile << lineCount <<" |\tSyntax error. Missing %% at the end of the file.\nToken\t" << token.second << endl;
        exit(1);
    }

    if (feof(infile)==false){
        outfile << "Syntax error. Out of scope." << endl;
        exit(1);
    }

    if (outputSwitch){
        outfile << "\t<Rat21SU>  ::=   %%  <Opt Declaration List>    <Statement List>  %%" << endl;
    }
    return 1;
}

int OptDeclarationList (FILE* infile, ofstream& outfile){

    if (DeclarationList(infile, outfile)){
        if (outputSwitch){
        outfile << "\t<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;
        }
    }
    
    return 1;
}

int DeclarationList (FILE* infile, ofstream& outfile){

    if (!Declaration(infile, outfile)){
        return 0;
    }
    
    //for ;
    if (token.second!=";"){
        outfile << lineCount <<" |\tSyntax error. Expecting ';'\nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    if(!BackTraceDeclarationList(infile, outfile)){
        return 0;
    }

    if (outputSwitch){
        outfile << "\t<Declaration List>  ::= <Declaration> ;     |      <Declaration> ; <Declaration List>" << endl;
    }
    return 1;
	
}

int BackTraceDeclarationList(FILE* infile, ofstream& outfile){
    

    if (!StatementList(infile, outfile)){
        return 0;
    }

    if (outputSwitch){
        outfile << "\t<BackTraceStatementList> ::= <Statement List> | <Empty>" << endl;
    }
    return 1;

}




int Declaration(FILE* infile, ofstream& outfile){
    // qualifier function
    if(!Qualifier(infile, outfile)){
        return 0;
    }

    // id
    //outfile << "Declaration" << endl;
    //std::pair<int, string>token=getToken(infile);

    if(token.first!=1){
        printLexeme(token.first, token.second, outfile);
        outfile << lineCount <<" |\tSyntax error. Expecting an idenifier. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    if (outputSwitch){
        outfile << "\t<Declaration> ::=   <Qualifier >  <identifier>" << endl;
    }
    return 1;

}

int Qualifier(FILE* infile, ofstream& outfile){
    
    outfile << "Qualifier" << endl;
    //std::pair<int, string> token = getToken(infile);
	//printLexeme(token.first, token.second, outfile);
    if(token.second == "integer" || token.second == "boolean"){
        if (outputSwitch){
        outfile << "\t<Qualifier> ::=  integer |  boolean" << endl;
        }
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
		return 1;
    }
    
    return 0;
}

int StatementList(FILE* infile, ofstream& outfile){
    
    if(!Statement(infile, outfile)){
		outfile << lineCount <<" |\tSyntax error. Expecting 'begin|identifier|if|put|get|while'. \nToken\t" << token.second << endl;
		return 0;
    }

	BackTraceStatementList(infile, outfile);
    
    if (outputSwitch){
        outfile << "\t<Statement List> ::=   <Statement>   | <Statement> <Statement List>" << endl;
    }
	return 1;
 }

int BackTraceStatementList(FILE* infile, ofstream& outfile){
    
    if(StatementList(infile, outfile)){
        if (outputSwitch){
            outfile << "\t<BackTraceStatementList> ::= <Statement List> | <Empty>" << endl;
        }
        return 1;
    }
    
    return 0; 
 }

int Statement(FILE* infile, ofstream& outfile){
    // outfile << "Statement1" << endl;
    // std::pair<int, string> token = getToken(infile);
    
    if(Compound(infile, outfile)||Assign(infile, outfile)||If(infile, outfile)||Get(infile, outfile)||Put(infile, outfile)||While(infile, outfile)){
        if (outputSwitch){
        outfile << "\t<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Get>   |   <Put>   |  <While> " << endl;
        }
        return 1;
    }
        
    // outfile << "Syntax Error2" << endl;
    return 0;
    
 }

int Compound(FILE* infile, ofstream& outfile){
    
    //outfile << "Compound1" << endl;

    if (token.second !="begin"){
        // for (auto it = token.second.rbegin(); it != token.second.rend(); ++it){
        //     ungetc(*it, infile);
        // }
        return 0;
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);
    if(!StatementList(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting 'begin|identifier|if|put|get|while'. \nToken\t" << token.second << endl;
        exit(1);
    }

    //outfile << "Compound2" << endl;
    //token = getToken(infile);
	//printLexeme(token.first, token.second, outfile);
    if(token.second != "end"){
        outfile << lineCount <<" |\tSyntax error. Expecting 'end'. \nToken\t" << token.second << endl;
	    exit(1);
	}
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    if (outputSwitch){
        outfile << "\t<Compound> ::=   begin <Statement List>  end" << endl;
    }
    return 1;
 }

int Assign(FILE* infile, ofstream& outfile){
    
    // outfile << "Assign1" << endl;
    // std::pair<int, string> token = getToken(infile);
	

    if (token.first ==1){
        printLexeme(token.first, token.second, outfile);
        outfile << "Assign2" << endl;
        token = getToken(infile);

        if (token.second == "="){
            printLexeme(token.first, token.second, outfile);
            token = getToken(infile);
            if(Expression(infile, outfile)){
                //outfile << "Assign3" << endl;
                //printLexeme(token.first, token.second, outfile);
                if (token.second == ";"){
                    printLexeme(token.first, token.second, outfile);
                    token = getToken(infile);
                    if (outputSwitch){
                        outfile << "\t<Assign> ::=     <Identifier> = <Expression>;" << endl;
                        return 1;
                    }
                }
                else{
                    outfile << lineCount <<" |\tSyntax error. Expecting ';'. \nToken\t" << token.second << endl;
                    exit(1);
                }
            }
            else{
                outfile << lineCount <<" |\tSyntax error. Expecting some expression. \nToken\t" << token.second << endl;
				exit(1);
			}
        }
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting '='. \nToken\t" << token.second << endl;
		    exit(1);
        }
	}
	return 0;
 }

int If(FILE* infile, ofstream& outfile){
    
// 
    // if
    // outfile << "If1" << endl;
    // std::pair<int, string> token = getToken(infile);
    if (token.second !="if"){
        return 0;
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    // (
    outfile << "If2" << endl;
    //token = getToken(infile);
	
    if (token.second !="("){
        outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);
    
    // <Condition>
    if (!Condition(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting a condition. \nToken\t" << token.second << endl;
        exit(1);
    }

    // )
    if (token.second !=")"){
        outfile << lineCount <<" |\tSyntax error. Expecting ')'. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);
    
    // <Statement>
    if (!Statement(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting a statement start with 'begin|identifier|if|put|get|while'. \nToken\t" << token.second << endl;
        exit(1);
    }

    // [elseState]
	elseState(infile, outfile);
    
    // endif
    if (token.second!="endif"){
        outfile << lineCount <<" |\tSyntax error. Expecting 'endif'. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    if (outputSwitch){
		outfile << "\t<If> ::= if ( <Condition> ) <Statement >[elseState] endif" << endl;
	}
    return 1;
     
 }

int elseState(FILE* infile, ofstream& outfile){
    
    // outfile << "elseState1" << endl;
    // std::pair<int, string> token = getToken(infile);
    if (token.second =="else"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
		if (Statement(infile, outfile)){
            if (outputSwitch){
		        outfile << "\t[elseState] ::= else <Statement> | epsilon" << endl;
	        }
            return 1;
        }
        else{
            outfile << lineCount <<" |\tSyntax error. Expecting a statement start with 'begin|identifier|if|put|get|while'. \nToken\t" << token.second << endl;
			exit(1);
		}
    }

    return 0;
}

int Put (FILE* infile, ofstream& outfile){
    

    // put
    //outfile << "Put1" << endl;
    //std::pair<int, string> token = getToken(infile);

    if (token.second!="put"){
        return 0;
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    // (
    if (token.second!="("){
        outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    // <identifier>
    if (token.first!=1){
        outfile << lineCount <<" |\tSyntax error. Expecting an identifier. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    // ;
    if (token.second!=";"){
		outfile << lineCount << " |\tSyntax error. Expecting ';'. \nToken\t" << token.second << endl;
		exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    if (outputSwitch){
		outfile << "\t<Put> ::=     put ( <identifier> );" << endl;
	}
	return 1;
}


int Get(FILE* infile, ofstream& outfile){

    // get
    // outfile << "Get1" << endl;
    // std::pair<int, string> token = getToken(infile);

    if (token.second!="get"){
        return 0;
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    // (
    if (token.second!="("){
        outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    // <identifier>
    if (token.first!=1){
        outfile << lineCount <<" |\tSyntax error. Expecting an identifier. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    // ;
    if (token.second!=";"){
		outfile << lineCount << " |\tSyntax error. Expecting ';'. \nToken\t" << token.second << endl;
		exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    if (outputSwitch){
		outfile << "\t<Get> ::=    get ( <Identifier> );" << endl;
	}
	return 1;
}

int While(FILE* infile, ofstream& outfile){
    

    //while
    // outfile << "While1" << endl;
    // std::pair<int, string> token = getToken(infile);
	
    if (token.second!="while"){
        return 0;
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    //(
    if (token.second!="("){
        outfile << lineCount <<" |\tSyntax error. Expecting '('. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    //Condition function
    if (!Condition(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting a condition. \nToken\t" << token.second << endl;
        exit(1);
    }

    //)
    if (token.second!=")"){
        outfile << lineCount <<" |\tSyntax error. Expecting ')'. \nToken\t" << token.second << endl;
        exit(1);
    }
    printLexeme(token.first, token.second, outfile);
    token = getToken(infile);

    //Statement function
    if (!Statement(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting a statement start with 'begin|identifier|if|put|get|while'. \nToken\t" << token.second << endl;
        exit(1);
    }

    if (outputSwitch){
		outfile << "\t<While> ::=  while ( <Condition>  )  <Statement>  " << endl;
	}
    return 1;
}

int Condition(FILE* infile, ofstream& outfile){
    // expression function
    if(!Expression(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting a expression. \nToken\t" << token.second << endl;
        exit(1);
    }

    // relop funcion
    if(!Relop(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting  '== | > | < | /=  '. \nToken\t" << token.second << endl;
        exit(1);
    }

    // expression function
    if(!Expression(infile, outfile)){
        outfile << lineCount <<" |\tSyntax error. Expecting a expression. \nToken\t" << token.second << endl;
        exit(1);
    }

    if (outputSwitch){
		outfile << "\t<Condition> ::=     <Expression>  <Relop>   <Expression>" << endl;
	}
    return 1;

}

int Relop(FILE* infile, ofstream& outfile){
    

    // == | > | < | /= 
    if (token.second=="==" || token.second==">" || token.second=="<" || token.second=="/="){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        if (outputSwitch){
		    outfile << "\t<Relop> ::=        ==     |    >     |    <     |    /=" << endl;
	    }
        return 1;
    }
    else{
        outfile << lineCount <<" |\tSyntax error. Expecting  '== | > | < | /=  '. \nToken\t" << token.second << endl;
        exit(1);
    }

}


int Expression(FILE* infile, ofstream& outfile){
    
    // Term function
    if(!Term(infile, outfile)){
        return 0;
    }

    // ExpressionPrime function
    if(!ExpressionPrime(infile, outfile)){
        return 0;
    }

    if (outputSwitch){
		outfile << "\t<Expression> ::= <Term><Expression>’" << endl;
	}
    return 1;

}

int ExpressionPrime(FILE* infile, ofstream& outfile){
    

    // + | -
    if (token.second=="+" || token.second=="-"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        if(Term(infile, outfile)){
            if(ExpressionPrime(infile, outfile)){

                if (outputSwitch){
		            outfile << "\t<Expression>’ ::=   + <Term><Expression>’|   - <Term><Expression>’" << endl;
	            }
                return 1;
            }
        }
		return 0;
	}

    if (outputSwitch){
		outfile << "\t<Expression>’ ::=   + <Term><Expression>’|   - <Term><Expression>’" << endl;
    }
	return 1;
}

int Term(FILE* infile, ofstream& outfile){
    
    // factor function
    if (!Factor(infile, outfile)){
        return 0;
    }

    // termPrime function
    if (!TermPrime(infile, outfile)){
        return 0;
    }
    
    if (outputSwitch){
		outfile << "\t<Term> ::= <Factor><Term>’" << endl;
	}
    return 1;

}

    
int TermPrime(FILE* infile, ofstream& outfile){
    
    
    // * | / | epsilon
    //std::pair<int, string> token = getToken(infile);
	//printLexeme(token.first, token.second, outfile);
    if (token.second=="*" || token.second=="/"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        if(Factor(infile, outfile)){
            if(TermPrime(infile, outfile)){

                if (outputSwitch){
		            outfile << "\t<Term>’ ::= *  <Factor> <Term>’  |   /  <Factor> <Term>’ | <Empty>" << endl;
	            }
                return 1;
            }
        }
		return 0;
	}

    if (outputSwitch){
		outfile << "\t<Term>’ ::= *  <Factor> <Term>’  |   /  <Factor> <Term>’ | <Empty>" << endl;
	}
	return 1;
}

int Factor(FILE* infile, ofstream& outfile){
    // -
    if (token.second=="-"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        if (Primary(infile, outfile)){
            if (outputSwitch){
		        outfile << "\t<Factor> ::=      -  <Primary>    |    <Primary>" << endl;
	        }
            return 1;
        }
    }
    else if (Primary(infile, outfile)){
        if (outputSwitch){
            outfile << "\t<Factor> ::=      -  <Primary>    |    <Primary>" << endl;
        }
        return 1;
    }
    outfile << lineCount <<" |\tSyntax error. Expecting  '- | identifier | integer | ( | true | false'. \nToken\t" << token.second << endl;
    exit(1);
}
    


int Primary(FILE* infile, ofstream& outfile){
    

    // id | int | true | false
    if(token.first == 1 || token.first == 2 || token.second == "true" || token.second == "false"){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        if (outputSwitch){
	    	outfile << "\t<Primary> ::=      <Identifier>  |  <Integer>  |  ( <Expression> )   |  true   |  false  " << endl;
	    }
        return 1;
    }
    else if(token.second == "("){
        printLexeme(token.first, token.second, outfile);
        token = getToken(infile);
        if(Expression(infile, outfile)){
            if(token.second == ")"){
                printLexeme(token.first, token.second, outfile);
                token = getToken(infile);
                if (outputSwitch){
	    	        outfile << "\t<Primary> ::=      <Identifier>  |  <Integer>  |  ( <Expression> )   |  true   |  false  " << endl;
	            }
				return 1;
			}
            else{
                outfile << lineCount <<" |\tSyntax error. Expecting  ')'. \nToken\t" << token.second << endl;
                exit(1);
            }
		}
    }
    outfile << lineCount <<" |\tSyntax error. Expecting  '- | identifier | integer | ( | true | false'. \nToken\t" << token.second << endl;
    exit(1);
}


