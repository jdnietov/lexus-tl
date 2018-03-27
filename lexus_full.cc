#include <iostream>
#include <fstream>
#include <queue>
#include <string>

#define ESTADO_INICIAL 0
#define ESTADO_STRING 1
#define ESTADO_ID 2
#define ESTADO_INT 3
#define ESTADO_FLOAT 4

using namespace std;

class Token {
    protected:
        int type;
        int line;
        int col;
        
    private:    // TODO
        int TYPES[];
        
    public:
        static const int TOKENS;

        static const int TOKEN_LLAVE_IZQ;
        static const int TOKEN_LLAVE_DER;
        static const int TOKEN_COM;
        static const int TOKEN_COR_IZQ;
        static const int TOKEN_COR_DER;
        static const int TOKEN_PAR_IZQ;
        static const int TOKEN_PAR_DER;
        static const int TOKEN_MAYOR;
        static const int TOKEN_MENOR;
        static const int TOKEN_MAYOR_IG;
        static const int TOKEN_MENOR_IG;
        static const int TOKEN_IN;
        static const int TOKEN_IGUAL_NUM;
        static const int TOKEN_POINT;
        static const int TOKEN_DIFF_NUM;
        static const int TOKEN_AND;
        static const int TOKEN_OR;
        static const int TOKEN_NOT;
        static const int TOKEN_MAS;
        static const int TOKEN_MENOS;
        static const int TOKEN_MUL;
        static const int TOKEN_DIV;
        static const int TOKEN_MOD;
        static const int TOKEN_POD;
        static const int TOKEN_ASSIGN;
        static const int TOKEN_STRING;
        static const int TOKEN_ID;
        static const int TOKEN_RESWORD;
        static const int TOKEN_INT;
        static const int TOKEN_FLOAT;
        static const int TOKEN_DOS_PUNTOS;
        static const int TOKEN_COMMA;
        
        Token();
        Token(int t, int l, int c);
        
        void set_type(int t) {
            type = t;
        }
        void set_line(int l) {
            line = l;
        }
        void set_col(int c) {
            col = c;
        }
        
        int get_type() {
            return type;
        }
        int get_line() {
            return line;
        }
        int get_col() {
            return col;
        }
        
        static bool is_operator(char c);
        static int get_op_key(char c);
        static int get_comp_op_key(std::string c);
        
        static std::string type2str(int type);
};

const int Token::TOKENS = 31;

const int Token::TOKEN_LLAVE_IZQ = 1;   // {
const int Token::TOKEN_LLAVE_DER = 2;   // }
const int Token::TOKEN_COM = 3;         // #
const int Token::TOKEN_COR_IZQ = 4;     // [
const int Token::TOKEN_COR_DER = 5;     // ]
const int Token::TOKEN_PAR_IZQ = 6;     // (
const int Token::TOKEN_PAR_DER = 7;     // )
const int Token::TOKEN_MAYOR = 8;       // >
const int Token::TOKEN_MENOR = 9;       // <
const int Token::TOKEN_MAYOR_IG = 10;   // >=
const int Token::TOKEN_MENOR_IG = 11;   // <=
const int Token::TOKEN_IGUAL_NUM = 12;  // ==
const int Token::TOKEN_POINT = 13;      // .
const int Token::TOKEN_DIFF_NUM = 14;   // !=
const int Token::TOKEN_AND = 15;        // &&
const int Token::TOKEN_OR = 16;         // 
const int Token::TOKEN_NOT = 17;
const int Token::TOKEN_MAS = 18;
const int Token::TOKEN_MENOS = 19;
const int Token::TOKEN_MUL = 20;
const int Token::TOKEN_DIV = 21;
const int Token::TOKEN_MOD = 22;
const int Token::TOKEN_POD = 23;
const int Token::TOKEN_ASSIGN = 24;
const int Token::TOKEN_STRING = 25;
const int Token::TOKEN_ID = 26;
const int Token::TOKEN_RESWORD = 27;
const int Token::TOKEN_INT = 28;
const int Token::TOKEN_FLOAT = 29;
const int Token::TOKEN_DOS_PUNTOS = 30;
const int Token::TOKEN_COMMA = 31;

const std::string TYPE2STR [Token::TOKENS] =
    { "token_llave_izq", "token_llave_der", "token_comentario",
    "token_cor_izq", "token_cor_der", "token_par_izq", "token_par_der",
    "token_mayor", "token_menor", "token_mayor_igual", "token_menor_igual",
    "token_igual_num", "token_point", "token_diff_num",
    "token_and", "token_or", "token_not", "token_mas", "token_menos", 
    "token_mul", "token_div", "token_mod", "token_pot", "token_assign",
    "token_string", "id", "token_reserved_word", "token_integer", "token_float", 
    "token_dosp", "token_coma" };

Token::Token(int t, int l, int c) {
    type = t;
    line = l;
    col = c;
}

Token::Token() {
    type = -1;
    line = -1;
    col = -1;
}

class Operator: public Token {
    public:
        Operator(int t, int l, int c) : Token(t, l, c) {
            
        }
        
        Operator() : Token() {
            
        }
        void print();
};

class Lexeme: public Token {
    std::string lexeme;
    public:
        Lexeme(int t, std::string s, int l, int c) : Token(t, l, c) {
            type = t;
            lexeme = s;
            line = l;
            col = c;
        }
        void print();
};


class ResWord: public Token {
    public:
        static std::string RESWORDS[];
        
        static const int N_RWORDS;
        
        static const int RWORD_LOG;
        static const int RWORD_FALSE;
        static const int RWORD_TRUE;
        static const int RWORD_IMPORTAR;
        static const int RWORD_FOR;
        static const int RWORD_IF;
        static const int RWORD_FUNCION;
        static const int RWORD_RETORNO;
        static const int RWORD_END;
        static const int RWORD_WHILE;
        static const int RWORD_ELIF;
        static const int RWORD_ELSE;
        static const int RWORD_IN;
        static const int RWORD_DESDE;
        static const int RWORD_TODO;
        static const int RWORD_NIL;
        
        ResWord(int t, int l, int c) : Token(t, l, c) {
            type = t;
            line = l;
            col = c;
        }
        
        static int get_word_key(std::string word);
        static std::string get_word(int idx);
        void print();
};



    
std::string Token::type2str(int type) {
    type--;
    return (type >= 0 && type < Token::TOKENS) ? TYPE2STR[type] : "ERROR_TKN";
}

int Token::get_comp_op_key(std::string s) {
    if(s == ">=") {
        return Token::TOKEN_MAYOR_IG;
    } if(s == "<=") {
        return Token::TOKEN_MENOR_IG;
    } if(s == "==") {
        return Token::TOKEN_IGUAL_NUM;
    } if(s == "!=") {
        return Token::TOKEN_DIFF_NUM;
    } if(s == "&&") {
        return Token::TOKEN_AND;
    } if(s == "||") {
        return Token::TOKEN_OR;
    }
        
    return -1;
}

int Token::get_op_key(char c) {
    switch(c) {  // check first character
        case '{':
            return Token::TOKEN_LLAVE_IZQ;
        case '}':
            return Token::TOKEN_LLAVE_DER;
        case '[':
            return Token::TOKEN_COR_IZQ;
        case ']':
            return Token::TOKEN_COR_DER;
        case '#':
            return Token::TOKEN_COM;
        case '(':
            return Token::TOKEN_PAR_IZQ;
         case ')':
            return Token::TOKEN_PAR_DER;
         case '>':
            return Token::TOKEN_MAYOR;
         case '<':
            return Token::TOKEN_MENOR;
        case '.':
            return Token::TOKEN_POINT;
        case '!':
            return Token::TOKEN_NOT;
        case '+':
            return Token::TOKEN_MAS;
        case '-':
            return Token::TOKEN_MENOS;
        case '*':
            return Token::TOKEN_MUL;
        case '/':
            return Token::TOKEN_DIV;
        case '%':
            return Token::TOKEN_MOD;
        case '^':
            return Token::TOKEN_POD;
        case '=':
            return Token::TOKEN_ASSIGN;
        case ':':
            return Token::TOKEN_DOS_PUNTOS;
        case ',':
            return Token::TOKEN_COMMA;
        default:
            return -1;
    }
    
}

void Operator::print() {
    std::cout << "<" << Token::type2str(type) << "," << line << "," << col << ">\n";
}

void Lexeme::print() {
    std::cout << "<" << Token::type2str(type) << "," << lexeme << "," << line << "," << col << ">\n";
}

const int ResWord::N_RWORDS = 16;

const int ResWord::RWORD_LOG = 1;
const int ResWord::RWORD_FALSE = 2;
const int ResWord::RWORD_TRUE = 3;
const int ResWord::RWORD_IMPORTAR = 4;
const int ResWord::RWORD_FOR = 5;
const int ResWord::RWORD_IF = 6;
const int ResWord::RWORD_FUNCION = 7;
const int ResWord::RWORD_RETORNO = 8;
const int ResWord::RWORD_END = 9;
const int ResWord::RWORD_WHILE = 10;
const int ResWord::RWORD_ELIF = 11;
const int ResWord::RWORD_ELSE = 12;
const int ResWord::RWORD_IN = 13;
const int ResWord::RWORD_DESDE = 14;
const int ResWord::RWORD_TODO = 15;
const int ResWord::RWORD_NIL = 16;

std::string ResWord::RESWORDS [ResWord::N_RWORDS] = {
    "log", "false", "true", "importar", "for", "if", "funcion", "retorno",
    "end", "while", "elif", "else", "in", "desde", "todo", "nil"
};

int ResWord::get_word_key(std::string word) {    // TODO: optimizar
    for(int i = 0; i < ResWord::N_RWORDS; i++) {
        if(word == RESWORDS[i]) return i+1;
    }
    
    return -1;
}

std::string ResWord::get_word(int idx) {
    idx--;
    return (idx >= 0 && idx < ResWord::N_RWORDS) ? ResWord::RESWORDS[idx] : "ERROR_RWD";
}

void ResWord::print() {
    std::cout << "<" << ResWord::get_word(type) << "," << line << "," << col << ">\n";
}


bool is_number(char c) {
    int ic = int(c);
    return ic >= 48 && ic <= 57;
}

bool is_letter(char c) {
    int ic = int(c);
    if(ic > 90)  ic -= 32;
    return ic >= 65 && ic <= 90;
}

void catch_error_lexico(int line, int col) {
    cout << ">>> Error lexico(linea:" << line << ",posicion:" << col << ")\n";
}

int main (int argc, char *argv[]) { // TODO: get file by command line parameters
    // ifstream code ("in01.txt");
    
    // if (code.is_open()) {
        int state = ESTADO_INICIAL, nline = 1;
        queue<Token> tokens;
        string line, buffer = "";
        
        while ( getline (cin,line) ) {
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
                                
                                if(key != -1 && ncol < line.length()) {
                                    op.set_type(key);
                                    ncol++;
                                } else if(Token::get_op_key(c) != -1) {
                                    op.set_type(Token::get_op_key(c));
                                } else {
                                    catch_error_lexico(nline, ncol);
                                    return 0;
                                }
                                
                                if(op.get_type() != -1)
                                    op.print();
                            }
                            break;
                            
                            case '"':
                            {
                                state = ESTADO_STRING;
                                icol = ncol;
                            }
                            break;
                            
                            default:
                                catch_error_lexico(nline, ncol);
                                return 0;
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
                            Lexeme flt(Token::TOKEN_FLOAT, buffer, nline, icol);
                            flt.print();
                            
                            buffer = "";
                            state = ESTADO_INICIAL;
                            ncol--;
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
        
        if(state != ESTADO_INICIAL)
            catch_error_lexico(nline-1, line.length());
            
        //code.close();
    //}

    //else cout << "No fue posible abrir el archivo." << '\n'; 

    return 0;
}