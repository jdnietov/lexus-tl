#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <string>

#define ESTADO_INICIAL 0
#define ESTADO_STRING 1
#define ESTADO_ID 2
#define ESTADO_INT 3
#define ESTADO_FLOAT 4

using namespace std;

//
// ──────────────────────────────────────────────────────  ──────────
//   :::::: H E L P E R S : :  :   :    :     :        :          :
// ────────────────────────────────────────────────────────────────
//

bool is_number(char c) {
    int ic = int(c);
    return ic >= 48 && ic <= 57;
}

bool is_letter(char c) {
    int ic = int(c);
    if(ic > 90)  ic -= 32;
    return ic >= 65 && ic <= 90;
}

vector<int> iarr2vec(int *array, int length) {
	vector<int> vec(array, array + length);
	return vec;
}

//
// ──────────────────────────────────────────────────────────────────────── I ──────────
//   :::::: L E X I C A L   A N A L Y S I S : :  :   :    :     :        :          :
// ──────────────────────────────────────────────────────────────────────────────────
//


class Token {
    protected:
        string lexeme;
        int t_class;
        int t_key;
        int line;
        int col;
        
    public:
        static const int N_TOKENS = 49;
        static const int N_RWORDS = 33;

        // FIXME: potential inconsistencies with this key model
        static const int TOKEN_LLAVE_IZQ = 1;   // {
        static const int TOKEN_LLAVE_DER = 2;   // }
        static const int TOKEN_COM = 3;         // #
        static const int TOKEN_COR_IZQ = 4;     // [
        static const int TOKEN_COR_DER = 5;     // ]
        static const int TOKEN_PAR_IZQ = 6;     // (
        static const int TOKEN_PAR_DER = 7;     // )
        static const int TOKEN_MAYOR = 8;       // >
        static const int TOKEN_MENOR = 9;       // <
        static const int TOKEN_MAYOR_IG = 10;   // >=
        static const int TOKEN_MENOR_IG = 11;   // <=
        static const int TOKEN_IGUAL_NUM = 12;  // ==
        static const int TOKEN_POINT = 13;      // .
        static const int TOKEN_DIFF_NUM = 14;   // !=
        static const int TOKEN_AND = 15;        // &&
        static const int TOKEN_OR = 16;         // 
        static const int TOKEN_NOT = 17;
        static const int TOKEN_MAS = 18;
        static const int TOKEN_MENOS = 19;
        static const int TOKEN_MUL = 20;
        static const int TOKEN_DIV = 21;
        static const int TOKEN_MOD = 22;
        static const int TOKEN_POD = 23;
        static const int TOKEN_ASSIGN = 24;
        static const int TOKEN_DOS_PUNTOS = 25;
        static const int TOKEN_COMMA = 26;
        static const int TOKEN_NEW_LINE = 27;
        static const int TOKEN_STRING = 28;
        static const int TOKEN_ID = 29;
        static const int TOKEN_RESWORD = 30;
        static const int TOKEN_INT = 31;
        static const int TOKEN_FLOAT = 32;
        static const int RWORD_LOG = 33;
        static const int RWORD_FALSE = 34;
        static const int RWORD_TRUE = 35;
        static const int RWORD_IMPORTAR = 36;
        static const int RWORD_FOR = 37;
        static const int RWORD_IF = 38;
        static const int RWORD_FUNCION = 39;
        static const int RWORD_RETORNO = 40;
        static const int RWORD_END = 41;
        static const int RWORD_WHILE = 42;
        static const int RWORD_ELIF = 43;
        static const int RWORD_ELSE = 44;
        static const int RWORD_IN = 45;
        static const int RWORD_DESDE = 46;
        static const int RWORD_TODO = 47;
        static const int RWORD_NIL = 48;
        static const int RWORD_LEER = 49;

        static const int T_OP = 1;
        static const int T_LEX = 2;
        static const int T_RES = 3;
        
        Token();
        Token(int cla);
        Token(int cla, int key, int lin, int co);
        Token(int cla, int key, string lex, int lin, int co);
        
        void print();
        static int get_op_key(char c);
        static int get_op_comp_key(string c);
        static int get_res_word_key(string word);
        static string get_key_name(int key);

        void set_key(int t) {
            t_key = t;
        }
        void set_line(int l) {
            line = l;
        }
        void set_col(int c) {
            col = c;
        }
        int get_class() {
            return t_class;
        }
        int get_key() {
            return t_key;
        }
        int get_line() {
            return line;
        }
        int get_col() {
            return col;
        }
        
    private:    // TODO
        static const string T[];
        static const string TOKNAMES[];
};

const string Token::TOKNAMES [Token::N_TOKENS] = { 
    // tokens
    "token_llave_izq", "token_llave_der", "token_comentario",
    "token_cor_izq", "token_cor_der", "token_par_izq", "token_par_der",
    "token_mayor", "token_menor", "token_mayor_igual", "token_menor_igual",
    "token_igual_num", "token_point", "token_diff_num",
    "token_and", "token_or", "token_not", "token_mas", "token_menos", 
    "token_mul", "token_div", "token_mod", "token_pot", "token_assign",
    "token_dosp", "token_coma", "token_new_line", "token_string", "id",
    "token_reserved_word", "token_integer", "token_float",
    // reserved words
    "log", "false", "true", "importar", "for", "if", "funcion", "retorno",
    "end", "while", "elif", "else", "in", "desde", "todo", "nil", "leer"
};

Token::Token() {
    t_class = 0;
    t_key = 0;
    line = 0;
    col = 0;
    lexeme = "";
}

Token::Token(int cla) {
    t_class = cla;
    t_key = -1;
    line = -1;
    col = -1;
    lexeme = "";
}

Token::Token(int cla, int key, int lin, int co) {
    t_class = cla;
    t_key = key;
    lexeme = Token::get_key_name(key);
    line = lin;
    col = co;
}

Token::Token(int cla, int key, std::string lex, int lin, int co) {
    t_class = cla;
    t_key = key;
    lexeme = lex;
    line = lin;
    col = co;
}

string Token::get_key_name(int key) {
    key--;
    return (key >= 0 && key < Token::N_TOKENS) ? Token::TOKNAMES[key] : "ERROR_TKN";
}

int Token::get_op_comp_key(string s) {
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
        case '\n':
            return Token::TOKEN_NEW_LINE;
        default:
            return -1;
    }
}

int Token::get_res_word_key(string word) {
    for(int i = Token::N_RWORDS-1; i < Token::N_TOKENS; i++) {
        if(word == TOKNAMES[i]) return i+1;
    }    
    return -1;
}

void Token::print() {
    cout << "<" << Token::get_key_name(t_key) << ",";
    if(t_class == T_LEX)
        cout << lexeme << ",";
    cout << line << "," << col << ">\n";
}

//
// ─── FETCH TOKEN ────────────────────────────────────────────────────────────────
//

Token currentToken;
int global_state, global_line_it, global_col_it;
string global_line;

void catch_error_lexico() {
    cout << ">>> Error lexico(linea:" << global_line_it << ",posicion:" << global_col_it << ")\n";
    exit(EXIT_FAILURE);
}

Token get_next_token() {
    Token token;
    string buffer = "";

    int icol = 1;
    for(int ncol = global_col_it; ncol <= global_line.length(); ncol++) {
        char c = global_line[ncol-1];
        //cout << "char: " << c << ", estado: " << global_state << ", buffer: " << buffer << ", ncol: " << ncol << "\n";

        if(c == '#')
            break;
        
        string str_buffer = "";
        switch(global_state) {
            case ESTADO_INICIAL:
            {
                if(is_letter(c) || is_number(c)) {
                    global_state = is_letter(c) ? ESTADO_ID : ESTADO_INT;
                    buffer += c;
                    icol = ncol;
                }
                
                else 
                {
                    switch(c) {
                    case ' ': case '\t':
                        break;
                    
                    // one-character operators
                    case '{': case '}': case '[': case ']': case '(':
                    case ')': case '.': case '+': case '-': case '*':
                    case '/': case '%': case '^': case ':': case ',':
                    case '\n':
                    {
                        Token op(Token::T_OP, Token::get_op_key(c), global_line_it, ncol);
                        global_col_it = ncol+1;
                        return op;
                    }
                    break;
                    
                    case '<': case '>': case '=': case '&': case '!':
                    case '|':
                    {
                        int key = -1;

                        Token op(Token::T_OP);
                        op.set_line(global_line_it);
                        op.set_col(ncol);

                        string s;
                        if(ncol+1 < global_line.length()) {
                            s = global_line.substr(ncol-1, 2);
                            key = Token::get_op_comp_key(s);
                        }
                        
                        if(key != -1 && ncol < global_line.length()) {
                            op.set_key(key);
                            ncol++;
                        } else if(Token::get_op_key(c) != -1) {
                            op.set_key(Token::get_op_key(c));
                        } else {
                            catch_error_lexico();
                        }
                        
                        if(op.get_key() != -1) {
                            global_state = ESTADO_INICIAL;
                            global_col_it = ncol+1;
                            return op;
                        } else {
                            cout << "*** ERROR: operand t_key is not defined, somehow? like, how the fuck did this happen?\n";
                        }
                    }
                    break;
                    
                    case '"':
                    {
                        global_state = ESTADO_STRING;
                        icol = ncol;
                    }
                    break;
                    
                    default:
                        global_col_it = ncol;
                        catch_error_lexico();
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
                        global_col_it = ncol+1;
                        global_state = ESTADO_INICIAL;
                        Token str(Token::T_LEX, Token::TOKEN_STRING, buffer, global_line_it, icol);
                        return str;
                    }
                    break;
                        
                    default:
                    {
                        buffer += c;
                        if(c == '\n') {
                            if(!getline(cin, global_line)) {
                                global_col_it = ncol;
                                catch_error_lexico();
                            }
                        }
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
                    int key = Token::get_res_word_key(buffer);
                    
                    global_col_it = ncol;
                    global_state = ESTADO_INICIAL;

                    if(key == -1) {
                        Token word(Token::T_LEX, Token::TOKEN_ID, buffer, global_line_it, icol);
                        return word;
                    } else {
                        Token word(Token::T_RES, key, global_line_it, icol);
                        return word;
                    }
                    
                }
            }
            break;
            
            case ESTADO_INT:
            {
                if (is_number(c)) {
                    buffer += c;
                } else if(c == '.') {
                    buffer += c;
                    global_state = ESTADO_FLOAT;
                } else {
                    global_col_it = ncol;
                    global_state = ESTADO_INICIAL;
                    Token number(Token::T_LEX, Token::TOKEN_INT, buffer, global_line_it, icol);
                    return number;
                }
            }
            break;
            
            case ESTADO_FLOAT:  // TODO - read if next char is number too!
            {
                if (is_number(c)) {
                    buffer += c;
                } else {
                    global_col_it = ncol;
                    global_state = ESTADO_INICIAL;
                    Token flt(Token::T_LEX, Token::TOKEN_FLOAT, buffer, global_line_it, icol);
                    return flt;
                }
            }
            break;
            
            default:    // error - automaton experienced an error
                cout << "*** ERROR: Automaton error (reached undefined global_state)\n";
                exit(EXIT_FAILURE);
                break;
        }
    }

    global_col_it = 0;
    return token;
}

//
// ────────────────────────────────────────────────────────────────────── II ──────────
//   :::::: S Y N T A X   A N A L Y S I S : :  :   :    :     :        :          :
// ────────────────────────────────────────────────────────────────────────────────
//

// TODO: don't print key names, but actual characters
void catch_error_sintactico(vector<int> expectedTokens) {
    cout << '<' << global_line_it << ':' << global_col_it << "> Error sintactico."
        << " Encontrado: \'" << Token::get_key_name(currentToken.get_key())
        << "', se esperaba ";
    
    // order lexicographically
    sort(expectedTokens.begin(), expectedTokens.end());
    for(int i=0; i<expectedTokens.size(); i++) {
        cout << '\'' << Token::get_key_name(expectedTokens[i]) << "'";
        if(i==expectedTokens.size()-1)
            cout << '\n';
        else
            cout << ",";
    }
    exit(EXIT_FAILURE);
}

class Grammar {
    public:
        static void followup(int expectedKey) {
            if(currentToken.get_key() == expectedKey) {
                //while((currentToken = get_next_token()).get_key() == Token::TOKEN_NEW_LINE);
                currentToken = get_next_token();
                cout << "OK\n\n";
                currentToken.print();
            } else {
                catch_error_sintactico((vector<int> (1, expectedKey)));
            }
        }
};

class GrammarCond {
    private:
        static const int N_OPCONDS = 7;
        static bool isOpCond(int tokenKey) {
            for(int i=0; i<N_OPCONDS; i++) {
                if(OPCONDS[i] == tokenKey)
                    return true;
            }
            return false;
        }
    public:
        static int OPCONDS[N_OPCONDS];
        static void ID() {  // TODO: object fields, arrays...
            cout << "deriving from ID: \n";
            int tokenKey = currentToken.get_key();
            if(tokenKey == Token::TOKEN_ID) {
                Grammar::followup(tokenKey);
            } else {
                int expToks[] = {Token::TOKEN_ID};
                catch_error_sintactico(iarr2vec(expToks, 1));
            }
        }

        static void XOP() {
            cout << "deriving from XOP: \n";
            int tokenKey = currentToken.get_key();
            if(tokenKey == Token::TOKEN_ID || tokenKey == Token::TOKEN_INT) {
                Grammar::followup(tokenKey);
            } else {
                int expToks[] = {Token::TOKEN_ID, Token::TOKEN_INT};
                catch_error_sintactico(iarr2vec(expToks, 2));
            }
        }

        static void OP_BIN_COND() {
            cout << "deriving from OP_BIN_COND: \n";
            int tokenKey = currentToken.get_key();
            if(isOpCond(tokenKey)) {
                Grammar::followup(tokenKey);
            } else {
                catch_error_sintactico(iarr2vec(OPCONDS, N_OPCONDS));
            }
        }

        static void CONDVAL() {
            cout << "deriving from CONDVAL\n";
            int tokenClass = currentToken.get_class();
            int tokenKey = currentToken.get_key();
            
            if(tokenKey == Token::RWORD_TRUE || tokenKey == Token::RWORD_FALSE
                || tokenKey == Token::TOKEN_ID || tokenKey == Token::TOKEN_INT) {
                Grammar::followup(tokenKey);
            } else if(tokenKey == Token::TOKEN_NEW_LINE) {
                Grammar::followup(Token::TOKEN_NEW_LINE);
            } else {
                int expToks[] = {Token::RWORD_TRUE, Token::RWORD_FALSE};    // TODO: 
                catch_error_sintactico(iarr2vec(expToks, 2));
            }
        }

        static void CONDAPP() {
            cout << "deriving from CONDAPP\n";
            int tokenKey = currentToken.get_key();

            if(isOpCond(tokenKey)) {
                OP_BIN_COND();
                COND();
            } else if(tokenKey == Token::TOKEN_NEW_LINE) {
                Grammar::followup(Token::TOKEN_NEW_LINE);
            } else {
                int expToks[] = {Token::TOKEN_AND};
                catch_error_sintactico(iarr2vec(expToks, 1));
            }
        }

        static void CONDNOT() {
            int tokenKey = currentToken.get_key();

            if(tokenKey == Token::TOKEN_ID) {
                ID();
            } else if(tokenKey == Token::TOKEN_PAR_IZQ) {
                COND();
            } else {
                int expToks[] = {Token::TOKEN_ID, Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(iarr2vec(expToks, 1));
            }
        }

        static void COND() {
            cout << "deriving from COND\n";
            int tokenKey = currentToken.get_key();            

            if(tokenKey == Token::RWORD_TRUE || tokenKey == Token::RWORD_FALSE
                || tokenKey == Token::TOKEN_ID || tokenKey == Token::TOKEN_INT) {
                CONDVAL();
                CONDAPP();
            } else if(tokenKey == Token::TOKEN_NOT) {
                Grammar::followup(Token::TOKEN_NOT);
                CONDNOT();
            } else if(tokenKey == Token::TOKEN_PAR_IZQ) {
                Grammar::followup(Token::TOKEN_PAR_IZQ);
                COND();
                Grammar::followup(Token::TOKEN_PAR_DER);
            } else {
                int expToks[] = {Token::RWORD_TRUE, Token::RWORD_FALSE, Token::TOKEN_ID, 
                Token::TOKEN_INT, Token::TOKEN_PAR_IZQ, Token::TOKEN_NOT};
                catch_error_sintactico(iarr2vec(expToks, 6));
            } 
        }
};

int GrammarCond::OPCONDS[GrammarCond::N_OPCONDS] = {
    Token::TOKEN_AND, Token::TOKEN_IGUAL_NUM, Token::TOKEN_MAYOR,
    Token::TOKEN_MAYOR_IG, Token::TOKEN_MENOR, Token::TOKEN_MENOR_IG,
    Token::TOKEN_OR
};

class GrammarIf {
    public:
        static void IF() {
            
        }
};

int main (int argc, char *argv[]) {
    global_state = ESTADO_INICIAL;
    global_line_it = 1;
    
    while ( getline (cin,global_line) ) {
        global_line+='\n';
        global_col_it = 1;
        // cout << global_line << '\n';

        while(global_col_it > 0 && global_col_it <= global_line.length()) {
            currentToken = get_next_token();
            if(global_state == ESTADO_INICIAL) {
                cout << '\n';
                currentToken.print();
                GrammarCond::COND();
                cout << "done.\n************\n";
            }
        }
        global_line_it++;
    }

    if(global_state != ESTADO_INICIAL) {
        --global_line_it;
        catch_error_lexico();
    }

    return 0;
}