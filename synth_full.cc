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
        static const int TOKEN_NOT = 17;        // !
        static const int TOKEN_MAS = 18;        // +
        static const int TOKEN_MENOS = 19;      // -
        static const int TOKEN_MUL = 20;        // *
        static const int TOKEN_DIV = 21;        // /
        static const int TOKEN_MOD = 22;        // %
        static const int TOKEN_POT = 23;        // ^
        static const int TOKEN_ASSIGN = 24;     // =
        static const int TOKEN_DOS_PUNTOS = 25; // :
        static const int TOKEN_COMMA = 26;      // ,
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
        static string get_key_str(int key);

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
        
    private:
        static const string TOKNSTRS[];
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

const string Token::TOKNSTRS [Token::N_TOKENS] = {
    "{", "}", "#", "[", "]", "(", ")", ">", "<", ">=", "<=", "==", ".", "!=",
    "&&", "||", "!", "+", "-", "*", "/", "%", "^", "=", ":", ",", "\n",
    "valor_string", "identificador", "ERROR", "valor_entero", "valor_float",
    // TODO: don't repeat data!
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
    return (key > 0 && key <= Token::N_TOKENS) ? Token::TOKNAMES[--key] : "ERROR_TKN";
}

string Token::get_key_str(int key) {
    return (key > 0 && key <= Token::N_TOKENS) ? Token::TOKNSTRS[--key] : "ERROR_STR";
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
            return Token::TOKEN_POT;
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
                } else {
                    switch(c) {
                    case ' ': case '\t':
                        break;
                    
                    // one-character operators
                    case '{': case '}': case '[': case ']': case '(':
                    case ')': case '+': case '-': case '*': case '/': 
                    case '%': case '^': case ':': case ',': case '\n':
                    {
                        Token op(Token::T_OP, Token::get_op_key(c), global_line_it, ncol);
                        global_col_it = ncol+1;
                        return op;
                    } break;
                    
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
                    } break;
                    
                    case '"':
                    {
                        global_state = ESTADO_STRING;
                        icol = ncol;
                    } break;
                    
                    case '.':
                    {
                        global_state = ESTADO_FLOAT;
                        icol = ncol;
                        buffer = "0.";
                    } break;

                    default:
                        global_col_it = ncol;
                        catch_error_lexico();
                    } break;
                }
            } break;
            
            case ESTADO_STRING:
            {
                switch(c) {
                    case '"':
                    {                        
                        global_col_it = ncol+1;
                        global_state = ESTADO_INICIAL;
                        Token str(Token::T_LEX, Token::TOKEN_STRING, buffer, global_line_it, icol);
                        return str;
                    } break;
                        
                    default:
                    {
                        buffer += c;
                        if(c == '\n') {
                            if(!getline(cin, global_line)) {
                                global_col_it = ncol;
                                catch_error_lexico();
                            }
                        }
                    } break;
                }    
            } break;
            
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
            } break;
            
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
            } break;
            
            case ESTADO_FLOAT:
            {
                if (is_number(c)) {
                    buffer += c;
                } else {
                    global_col_it = ncol;
                    global_state = ESTADO_INICIAL;
                    Token flt(Token::T_LEX, Token::TOKEN_FLOAT, buffer, global_line_it, icol);
                    return flt;
                }
            } break;
            
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

void catch_error_sintactico(int *expectedTokens, int size) {
    cout << '<' << global_line_it << ':' << --global_col_it << "> Error sintactico."
        << " Encontrado: \'" << Token::get_key_str(currentToken.get_key())
        << "'; se esperaba: ";
    
    // order lexicographically
    sort(expectedTokens, expectedTokens + size);    // FIXME: not sorting!
    for(int i=0; i<size; i++) {
        cout << '\'' << Token::get_key_str(expectedTokens[i]) << "'";
        if(i==size-1)
            cout << ".\n";
        else
            cout << ",";
    }
    exit(EXIT_FAILURE);
}

class Grammar {
    private:
        static const int N_MATHOP = 7;
        static int ARR_MATHOP[N_MATHOP];

    public:
        static bool ismathop(int key) {
            for(int i=0; i<N_MATHOP; i++) {
                if(ARR_MATHOP[i] == key)
                    return true;
            }
            return false;
        }

        static bool isparamval(int key) {
            return key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT || key == Token::TOKEN_STRING
                || key == Token::RWORD_TRUE || key == Token::RWORD_FALSE || key == Token::TOKEN_ID;
        }

        static void CALL() {
            cout << "deriving from CALL\n";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_PAR_IZQ) {
                followup(Token::TOKEN_PAR_IZQ);
                PARAMS();
                followup(Token::TOKEN_PAR_DER);
            } else {
                int exptoks[] = {Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(exptoks, 1);
            }
        }

        static void ID() {  
            cout << "deriving from ID: \n";
            int tokenKey = currentToken.get_key();
            if(tokenKey == Token::TOKEN_ID) {
                followup(tokenKey);
                IDNEXT();
            } else {
                int expToks[] = {Token::TOKEN_ID};
                catch_error_sintactico(expToks, 1);
            }
        }

        static void IDCALL() {
            int key = currentToken.get_key();

            if(key == Token::TOKEN_ID) {
                ID();
                IDCALLNEXT();
            } else {
                // TODO:
            }
        }

        static void IDCALLNEXT() {
            int key = currentToken.get_key();

            if(key == Token::TOKEN_PAR_IZQ) {
                CALL();
            } else return;  // TODO:
        }

        static void IDNEXT() {
            int key = currentToken.get_key();
            if(key == Token::TOKEN_POINT) {
                followup(Token::TOKEN_POINT);
                followup(Token::TOKEN_ID);
                IDNEXT();
            } else if(key == Token::TOKEN_COR_IZQ) {
                followup(Token::TOKEN_COR_IZQ);
                IDX();
                followup(Token::TOKEN_COR_DER);
            }
        }

        static void IDNUMBER() {
            int key = currentToken.get_key();

            if(key == Token::TOKEN_ID) {
                IDCALL();
            } else if(key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT) {
                NUMBER();
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_INT, Token::TOKEN_FLOAT};
                catch_error_sintactico(exptoks,3);
            }
        }

        static void IDX() {
            int key = currentToken.get_key();

            if(key == Token::TOKEN_ID) {
                IDCALL();
            } else if(key == Token::TOKEN_INT) {
                followup(Token::TOKEN_INT);
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_INT};
                catch_error_sintactico(exptoks, 2);
            }
        }

        static void INITMATHNEXT() {
            int key = currentToken.get_key();

            if(ismathop(key)) {
                MATHOP();
                INITMATH();
            } else if(key == Token::TOKEN_COMMA || key == Token::TOKEN_PAR_DER) {
                return;
            } else {
                catch_error_sintactico(ARR_MATHOP, N_MATHOP);
            }
        }

        static void INITMATH() {
            int key = currentToken.get_key();

            if(key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT || key == Token::TOKEN_ID) {
                IDNUMBER();
                INITMATHNEXT();
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_INT, Token::TOKEN_FLOAT};
                catch_error_sintactico(exptoks,3);
            }
        }

        static void MATHOP() {
            cout << "deriving from MATHOP\n";
            int key = currentToken.get_key();
            
            if(ismathop(key)) {
                Grammar::followup(key);
            } else {
                catch_error_sintactico(ARR_MATHOP, N_MATHOP);
            }
        }
        
        static void NUMBER() {
            int tokenType = currentToken.get_key();

            if(tokenType == Token::TOKEN_INT || tokenType == Token::TOKEN_FLOAT)
                followup(tokenType);
            else {
                int expToks[] = {Token::TOKEN_INT, Token::TOKEN_FLOAT};
                catch_error_sintactico(expToks, 2);
            }
        }

        static void PARAMS() {
            cout << "deriving from PARAMS\n";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_PAR_DER) {   // FIXME: just, ugh.
                return;
            } else if(key == Token::TOKEN_ID || key == Token::TOKEN_FLOAT
                || key == Token::TOKEN_STRING || key == Token::TOKEN_INT
                || key == Token::RWORD_TRUE || key == Token::RWORD_FALSE) {
                Grammar::PARAMVAL();
                PARAMSNEXT();
            }
        }

        static void PARAMSNEXT() {
            cout << "deriving from PARAMSNEXT\n";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_COMMA) {
                Grammar::followup(Token::TOKEN_COMMA);
                PARAMS();
            } else if(key == Token::TOKEN_PAR_DER) {
                return;
            }
        }

        static void PARAMVAL() {
            int key = currentToken.get_key();
            
            if(key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT) {
                INITMATH(); // TODO: ids shared between conditionals and math!
            } else if(key == Token::TOKEN_STRING
                || key == Token::RWORD_TRUE || key == Token::RWORD_FALSE) {
                Grammar::followup(key);
            } else if(key == Token::TOKEN_ID) {
                IDCALL();
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_INT, Token::TOKEN_FLOAT,
                    Token::TOKEN_STRING, Token::RWORD_TRUE, Token::RWORD_FALSE};
                catch_error_sintactico(exptoks, 6);
            }
        }

        static void followup(int expectedKey) {
            if(currentToken.get_key() == expectedKey) {
                //while((currentToken = get_next_token()).get_key() == Token::TOKEN_NEW_LINE);
                currentToken = get_next_token();
                cout << "OK\n\n";
                currentToken.print();
            } else {
                int expToks[] = {expectedKey};
                catch_error_sintactico(expToks, 1);
            }
        }
};

int Grammar::ARR_MATHOP[] = {
    Token::TOKEN_MAS, Token::TOKEN_MENOS, Token::TOKEN_MUL, Token::TOKEN_DIV,
    Token::TOKEN_POT, Token::TOKEN_MOD, Token::TOKEN_ASSIGN
};

//
// ─── CONDITIONALS ───────────────────────────────────────────────────────────────
//

// TODO: strings!
class GrammarCond {
    private:
        static const int N_OPBIN = 4;
        static const int N_OPNUM = 5;
    public:
        static int ARR_OPBIN[N_OPBIN];
        static int ARR_OPNUM[N_OPNUM];

        static bool isopbin(int tokenKey) {
            for(int i=0; i<N_OPBIN; i++) {
                if(ARR_OPBIN[i] == tokenKey)
                    return true;
            }
            return false;
        }

        static bool isopnum(int tokenKey) {
            for(int i=0; i<N_OPNUM; i++) {
                if(ARR_OPNUM[i] == tokenKey)
                    return true;
            }
            return false;
        }

        static void TFVAL() {
            int tokenKey = currentToken.get_key();

            if(tokenKey == Token::RWORD_TRUE || tokenKey == Token::RWORD_FALSE) {
                Grammar::followup(tokenKey);
                TFNEXT();
            } else {
                int expToks[] = {Token::RWORD_TRUE, Token::RWORD_FALSE};
                catch_error_sintactico(expToks, 2);
            }
        }

        static void TFNEXT() {
            int tokenKey = currentToken.get_key();

            if(tokenKey == Token::TOKEN_PAR_DER) {  // FIXME: this feels wrong
                return;
            } else if(isopbin(tokenKey)) {
                OPBIN();
                COND();
            } else {
                int expToks[N_OPBIN+1];
                for(int i=0; i<N_OPBIN; i++)
                    expToks[i] = ARR_OPBIN[i];
                expToks[N_OPBIN] = Token::TOKEN_PAR_DER;
                catch_error_sintactico(expToks, N_OPBIN+1);
            }
        }

        static void OPBIN() {
            cout << "deriving from OPBIN: \n";
            int tokenKey = currentToken.get_key();
            if(isopbin(tokenKey)) {
                Grammar::followup(tokenKey);
            } else {
                catch_error_sintactico(ARR_OPBIN, N_OPBIN);
            }
        }

        static void OPNUM() {
            cout << "deriving from OPNUM: \n";
            int tokenKey = currentToken.get_key();
            if(isopnum(tokenKey)) {
                Grammar::followup(tokenKey);
            } else {
                catch_error_sintactico(ARR_OPNUM, N_OPNUM);
            }
        }

        static void CONDNOT() {
            cout << "deriving from CONDNOT\n";
            int tokenKey = currentToken.get_key();

            if(tokenKey == Token::TOKEN_ID) {
                Grammar::ID();
            } else if(tokenKey == Token::TOKEN_PAR_IZQ) {
                COND();
            } else {
                int expToks[] = {Token::TOKEN_ID, Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(expToks, 1);
            }
        }

        static void CONDNEXT() {
            cout << "deriving from CONDNEXT\n";
            int tokenKey = currentToken.get_key();

            if(isopbin(tokenKey)) {
                OPBIN();
                COND();
            } else if(isopnum(tokenKey)) {
                OPNUM();
                Grammar::PARAMVAL();
            } else if(tokenKey == Token::TOKEN_NEW_LINE) {
                Grammar::followup(Token::TOKEN_NEW_LINE);
            } else if(tokenKey == Token::TOKEN_PAR_DER) {
                return;
            } else {
                // TODO: incomplete report
                int expToks[] = {Token::TOKEN_AND};
                catch_error_sintactico(expToks, 1);
            }
        }

        static void COND() {
            cout << "deriving from COND\n";
            int tokenKey = currentToken.get_key();

            switch(tokenKey) {
                case Token::RWORD_TRUE: case Token::RWORD_FALSE:
                {
                    TFVAL();
                    TFNEXT();
                } break;

                case Token::TOKEN_ID:
                {
                    Grammar::IDCALL();
                    CONDNEXT();
                } break;

                case Token::TOKEN_INT:
                {
                    Grammar::followup(Token::TOKEN_INT);
                    OPNUM();
                    Grammar::PARAMVAL();
                } break;

                case Token::TOKEN_NOT:
                {
                    Grammar::followup(Token::TOKEN_NOT);
                    CONDNOT();
                } break;

                case Token::TOKEN_PAR_IZQ:
                {
                    Grammar::followup(Token::TOKEN_PAR_IZQ);
                    COND();
                    Grammar::followup(Token::TOKEN_PAR_DER);
                } break;

                default:
                {
                    int expToks[] = {Token::RWORD_TRUE, Token::RWORD_FALSE, Token::TOKEN_ID, 
                    Token::TOKEN_INT, Token::TOKEN_PAR_IZQ, Token::TOKEN_NOT};
                    catch_error_sintactico(expToks, 6);
                } break;
            }
        }
};

int GrammarCond::ARR_OPBIN[GrammarCond::N_OPBIN] = {
    Token::TOKEN_AND, Token::TOKEN_IGUAL_NUM, Token::TOKEN_OR,
    Token::TOKEN_DIFF_NUM
};

int GrammarCond::ARR_OPNUM[GrammarCond::N_OPNUM] = {
    Token::TOKEN_MAYOR_IG, Token::TOKEN_MENOR, Token::TOKEN_MENOR_IG,
    Token::TOKEN_IGUAL_NUM, Token::TOKEN_DIFF_NUM
};

//
// ─── IF STATEMENTS ──────────────────────────────────────────────────────────────
//

    
class GrammarIf {
    public:
        static void IF() {
            cout << "deriving from IF\n";
            int tokenKey = currentToken.get_key();

            if(tokenKey == Token::RWORD_IF) {
                Grammar::followup(Token::RWORD_IF);
                Grammar::followup(Token::TOKEN_PAR_IZQ);
                GrammarCond::COND();
                Grammar::followup(Token::TOKEN_PAR_DER);
            } else {
                int expToks[] = {Token::RWORD_IF};
                catch_error_sintactico(expToks, 1);
            }
        }
};

//
// ─── IMPORTING ──────────────────────────────────────────────────────────────────
//

    
class GrammarImport {
    public:
        static void IMPORT() {
            int key = currentToken.get_key();

            if(key == Token::RWORD_IMPORTAR) {
                Grammar::followup(Token::RWORD_IMPORTAR);
                Grammar::ID();
            } else if(key == Token::RWORD_DESDE) {
                Grammar::followup(Token::RWORD_DESDE);
                Grammar::ID();
                Grammar::followup(Token::RWORD_IMPORTAR);
                Grammar::ID();
            } else {
                int exptoks[] = {Token::RWORD_IMPORTAR};
                catch_error_sintactico(exptoks, 1);
            }
        }
};

//
// ─── INSTRUCTIONS ───────────────────────────────────────────────────────────────
//

    
class GrammarInst {
    private:
        static const int N_RWFUNC = 2;
        
        static bool isrwfunc(int key) {
            for(int i=0; i<N_RWFUNC; i++) {
                if(ARR_RWFUNC[i] == key)
                    return true;
            }
            return false;
        }
    public:
        static int ARR_MATHOP[];
        static int ARR_RWFUNC[];

        static void RWFUNC() {
            cout << "deriving from RWFUNC\n";
            int key = currentToken.get_key();

            if(isrwfunc(key)) {
                Grammar::followup(key);
            } else {
                catch_error_sintactico(ARR_RWFUNC, N_RWFUNC);
            }
        }

        static void INSTNEXT() {
            cout << "deriving from INSTNEXT\n";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_PAR_IZQ) {
                Grammar::CALL();
            } else if(Grammar::ismathop(key)) {
                Grammar::MATHOP();
                Grammar::PARAMVAL();
            } else if(key == Token::TOKEN_NEW_LINE || key == Token::TOKEN_PAR_DER) {   // FIXME: still wrong
                return;
            } else {
                int exptoks[] = {Token::TOKEN_PAR_IZQ, Token::TOKEN_POINT, Token::TOKEN_NEW_LINE};
                catch_error_sintactico(exptoks, 3);
            }
        }

        static void INST() {
            cout << "deriving from INST\n";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_ID) {
                Grammar::ID();
                INSTNEXT();
            } else if(key == Token::RWORD_IF) {
                GrammarIf::IF();
            } else if(key == Token::RWORD_WHILE) {
                Grammar::followup(Token::RWORD_WHILE);
                Grammar::followup(Token::TOKEN_PAR_IZQ);
                GrammarCond::COND();
                Grammar::followup(Token::TOKEN_PAR_DER);
            } else if(isrwfunc(key)) {
                RWFUNC();
                Grammar::CALL();
            } else if(key == Token::RWORD_IMPORTAR || key == Token::RWORD_DESDE) {
                GrammarImport::IMPORT();
            } else {
                int exptoks[] = {-1};   // TODO: actual values
                catch_error_sintactico(exptoks, 1);
            } 
        }
        

        static void INSTS() {
            cout << "deriving from INSTS\n";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_ID || key == Token::RWORD_IF || isrwfunc(key)
                || key == Token::RWORD_IMPORTAR || key == Token::RWORD_DESDE
                || key == Token::RWORD_WHILE) {
                INST();
                INSTS();
            } else if(key == Token::TOKEN_NEW_LINE) {
                return;
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::RWORD_IF, Token::RWORD_WHILE}; // TODO: complete
                catch_error_sintactico(exptoks, 3);
            }
        }
};

class GrammarFunc {
    public:
        static void XPARAMVAL() {
            int key = currentToken.get_key();

            if(key == Token::TOKEN_NEW_LINE) {
                return;
            } else if(key == Token::TOKEN_ID) {
                Grammar::PARAMVAL();
            }
        }

        static void FUNC() {
            cout << "deriving from FUNC\n";
            int key = currentToken.get_key();

            if(key == Token::RWORD_FUNCION) {
                Grammar::followup(Token::RWORD_FUNCION);
                Grammar::followup(Token::TOKEN_ID);
                Grammar::CALL();
                Grammar::followup(Token::TOKEN_LLAVE_IZQ);
                Grammar::followup(Token::TOKEN_NEW_LINE);
                GrammarInst::INSTS();
                Grammar::followup(Token::RWORD_RETORNO);
                XPARAMVAL();
                Grammar::followup(Token::TOKEN_LLAVE_DER);
            }
        }

        static void FUNCS() {
            cout << "deriving from FUNCS\n";
            int key = currentToken.get_key();

            if(key == Token::RWORD_FUNCION) {
                FUNC();
                Grammar::followup(Token::TOKEN_NEW_LINE);
                FUNCS();
            } else return;  // TODO: this doesn't look right
        }
};

class GrammarProgram {
    public:
        static void PROGRAM() {
            int key = currentToken.get_key();

            if(key == Token::RWORD_FUNCION) {
                GrammarFunc::FUNCS();
                GrammarInst::INSTS();
            } else {
                GrammarInst::INSTS();
            }
        }
};

int GrammarInst::ARR_RWFUNC[] = {
    Token::RWORD_LEER, Token::RWORD_LOG
};

/**
 * TODO: COUNTEREXAMPLES NOT COVERED YET:
 * - "lala" (solitary identifiers)
 * */
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
                GrammarProgram::PROGRAM();
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