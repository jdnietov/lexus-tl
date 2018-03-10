#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include "lextoken.h"

#define ESTADO_INICIAL 0
#define ESTADO_STRING 1
#define ESTADO_ID 2
#define ESTADO_INT 3
#define ESTADO_FLOAT 4

using namespace std;

bool is_number(char c) {
    int ic = int(c);
    return ic >= 48 && ic <= 57;
}

bool is_letter(char c) {
    int ic = int(c);
    if(ic > 90)  ic -= 32;
    return ic >= 65 && ic <= 90;
}

int main (int argc, char *argv[]) { // TODO: get file by command line parameters
    ifstream code ("in01.txt");
    
    if (code.is_open()) {
        int state = ESTADO_INICIAL, nline = 1;
        queue<Token> tokens;
        string line, buffer = "";
        
        while ( getline (code,line) ) {
            line += '\n';
            
            int icol = 1;
            for(int ncol = 1; ncol <= line.length(); ncol++) {
                char c = line[ncol-1];
                // cout << "line " << nline << ", reading " << c << '\n';
                
                // commentary - stop reading line, go to next
                if(c == '#') {
                    break;
                }
                
                string str_buffer = "";
                switch(state) {
                    case ESTADO_INICIAL:
                    {
                        if(is_letter(c)) 
                        {
                            state = ESTADO_ID;
                            buffer += c;
                            icol = ncol;
                        }
                        
                        else if(is_number(c)) 
                        {
                            state = ESTADO_INT;
                            buffer += c;
                            icol = ncol;
                        }
                        
                        else 
                        {
                            switch(c) {
                            case ' ':
                            case '\t':
                            case '\n':
                                break;
                            
                            // one-character operators
                            case '{':
                            case '}':
                            case '[':
                            case ']':
                            case '(':
                            case ')':
                            case '.':
                            case '+':
                            case '-':
                            case '*':
                            case '/':
                            case '%':
                            case '^':
                            case ':':
                            case ',':
                            {
                                Operator op(Token::get_op_key(c), nline, ncol);
                                op.print();
                            }
                            break;
                            
                            case '<':
                            case '>':
                            case '=':
                            case '&':
                            case '!':
                            case '|':
                            {
                                int key;
                                Operator op;
                                op.set_line(nline);
                                op.set_col(ncol);
                                
                                string s(line, ncol-1, 2);
                                key = Token::get_comp_op_key(s);
                                
                                if(key != -1) {
                                    op.set_type(key);
                                    ncol++;
                                } else if(Token::get_op_key(c) != -1) {
                                    op.set_type(Token::get_op_key(c));
                                }
                                
                                if(op.get_type() != -1)
                                    op.print();
                            }
                            break;
                            
                            case 'i':
                            {
                                if(line[ncol] == 'n') {
                                    Operator op(Token::TOKEN_IN, nline, ncol);
                                    op.print();
                                }
                            }
                            break;
                            
                            case '"':
                            {
                                state = ESTADO_STRING;
                                icol = ncol;
                            }
                            break;
                            
                            default:
                                cout << "Error lexico(linea:" << nline << ",posicion:" << ncol << ")\n";
                                return -1;
                                break;
                            }
                        }
                    }
                    break;
                    
                    case ESTADO_STRING:
                    {
                        switch(c) {
                            case '"':
                            {
                                Lexeme str(Token::TOKEN_STRING, buffer, nline, icol);
                                str.print();
                               
                                state = ESTADO_INICIAL;
                                buffer = "";
                            }
                            break;
                                
                            default:
                            {
                                buffer += c;
                            }
                                break;
                        }    
                    }
                    break;
                    
                    case ESTADO_ID:
                    {
                        if(is_letter(c) || is_number(c)) {  // still an id
                            buffer += c;
                        } else {
                            int key = ResWord::get_word_key(buffer);
                            
                            if(key == -1) {
                                Lexeme word(Token::TOKEN_ID, buffer, nline, icol);
                                word.print();
                            } else {
                                ResWord word(key, nline, icol);
                                word.print();
                            }
                            
                            --ncol;
                            state = ESTADO_INICIAL;
                            buffer = "";
                        }
                    }
                    break;
                    
                    case ESTADO_INT:
                    {
                        if (is_number(c)) {
                            buffer += c;
                        } else if(c == '.') {
                            buffer += c;
                            state = ESTADO_FLOAT;
                        } else {
                            Lexeme number(Token::TOKEN_INT, buffer, nline, icol);
                            number.print();
                            buffer = "";
                            state = ESTADO_INICIAL;
                            --ncol;
                        }
                    }
                    break;
                    
                    case ESTADO_FLOAT:  // TODO - read if next char is number too!
                    {
                        if (is_number(c)) {
                            buffer += c;
                        } else {
                            Lexeme flt(Token::TOKEN_FLOAT, buffer, nline, ncol);
                            flt.print();
                            
                            buffer = "";
                            state = ESTADO_INICIAL;
                        }
                    }
                    break;
                    
                    default:    // error - automaton experienced an error
                        cout << "Error - we fucked up. Sorry!\n";
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