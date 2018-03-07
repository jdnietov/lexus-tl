#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include "lextoken.h"

using namespace std;

/** KEYWORDS
 * log
 * false
 * true
 * importar
 * for
 * in
 * if
 * funcion
 * retorno
 * end
 * while
*/

int main (int argc, char *argv[]) { // TODO: get file by command line parameters
    string line;
    ifstream code ("in01.txt");
    
    if (code.is_open()) {
        string buffer = "";
        
        queue<Token> tokens;
        
        int nline = 1;
        while ( getline (code,line) ) {
            for(int col = 0; col < line.length(); col++) {
                char c = line[col];
                bool found = false;
                switch(c) {
                    case ' ':
                    case '\t':
                    case '\n':
                        break;
                        
                    case '{': 
                    {
                        Operator llaveizq (Token::TOKEN_LLAVE_IZQ, nline, col+1);
                        llaveizq.print();
                    }
                        break;
                        
                    case '}':
                    {
                        Operator llaveder (Token::TOKEN_LLAVE_DER, nline, col+1);
                        llaveder.print();
                    }
                        break;
                    
                    case '#':   // TODO: caso del comentario!
                    {
                        Operator com (Token::TOKEN_COM, nline, col+1);
                        com.print();
                    }
                        break;
                        
                    case '[':
                    {
                        Operator corizq (Token::TOKEN_COR_IZQ, nline, col+1);
                        corizq.print();
                    }
                        break;
                        
                    case ']':
                    {
                        Operator corder (Token::TOKEN_COR_DER, nline, col+1);
                        corder.print();
                    }
                        break;
                    
                    case '(':
                    {
                        Operator parizq (Token::TOKEN_PAR_IZQ, nline, col+1);
                        parizq.print();
                    }
                        break;
                        
                    case ')':
                    {
                        Operator parizq (Token::TOKEN_PAR_IZQ, nline, col+1);
                        parizq.print();
                    }
                        break;
                    
                    case '>':
                    {
                        if(col+1 < line.length() && line[col+1] == '=') {
                            Operator mayor_eq (Token::TOKEN_MAYOR_IG, nline, col+1);
                            mayor_eq.print();
                        } else {
                            Operator mayor (Token::TOKEN_MAYOR, nline, col+1);
                            mayor.print();
                        }
                    }
                        break;
                        
                    case '<':
                    {
                        if(col+1 < line.length() && line[col+1] == '=') {
                            Operator menor_eq (Token::TOKEN_MENOR_IG, nline, col+1);
                            menor_eq.print();
                        } else {
                            Operator menor (Token::TOKEN_MENOR_IG, nline, col+1);
                            menor.print();
                        };
                    }
                        break;
                        
                    // TODO el resto!
                        
                    case '+':
                    {
                        Operator addop (Token::TOKEN_MAS, nline, col+1);
                        addop.print();
                    }
                        break;
                }
            }
            nline++;
        }
        code.close();
    }

    else cout << "No fue posible abrir el archivo." << '\n'; 

    return 0;
}