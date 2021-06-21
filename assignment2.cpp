#include "lexer.cpp"

const int outputSwitch = 0;

int main(){
    

    if (outputSwitch){
        cout << "" << endl;
    }

    return 0;

}


void Rat21SU (FILE* infile, ofstream& outfile){
    std::pair<int, string> result = lexer(infile);

    if (outputSwitch){
        outfile << "<Rat21SU>  ::=   %%  <Opt Declaration List>    <Statement List>  %%" << endl;
    }
    
    if (result.second!="%%"){
        cout << "Syntax error. This program doesn't start with %%." << endl;
        exit(1);
    }

    OptDeclarationList(infile, outfile);
    StatementList(infile, outfile);
    
    if (result.second!="%%"){
        cout << "Syntax error. Missing %% at the end of the file" << endl;
        exit(1);
    }

    if (feof(infile)==false){
        cout << "Syntax error. Out of scope." << endl;
        exit(1);
    }
}



void OptDeclarationList (FILE* infile, ofstream& outfile){

    if (outputSwitch){
        outfile << "<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;
    }
    
    DeclarationList(infile, outfile);
    
}

void DeclarationList (FILE* infile, ofstream& outfile){
    
    if (outputSwitch){
        outfile << "<Declaration List>  ::= <Declaration> ;     |      <Declaration> ; <Declaration List>" << endl;
    }
    
    Declaration(infile, outfile);
    
    //for ;
    std::pair<int, string> result = lexer(infile);
    if (result.second!=";"){
        cout << "Syntax Error" << endl;
        exit(1);
    }

    
    
}




void Declaration(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << " <Declaration> ::=   <Qualifier >  <identifier>" << endl;
    }

	Qualifier(infile, outfile);

	Identifier(infile, outfile);

}


void Qualifier(FILE* infile, ofstream& outfile){
    if (outputSwitch){
        outfile << " <Qualifier> ::=  integer |  boolean>" << endl;
    }
    std::pair<int, string> result = lexer(infile);
    if(result.first != 2 | result.second != "true" |result.second != "true"){
        cout << "Syntax Error" << endl;
        exit(1);
    }
}
