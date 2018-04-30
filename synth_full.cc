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
// ─── HELPERS ────────────────────────────────────────────────────────────────────
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
// ─── LEXICAL ANALYSIS ───────────────────────────────────────────────────────────
//



class Token {
    protected:
        string lexeme;
        int t_class;
        int t_key;
        int line;
        int col;
        
    public:
        static const int N_TOKENS = 50;
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
        static const int TOKEN_EOF = 50;

        static const int T_OP = 1;
        static const int T_LEX = 2;
        static const int T_RES = 3;
        
        Token();
        Token(int cla);
        Token(int cla, int key, int lin, int co);
        Token(int cla, int key, string lex, int lin, int co);
        
        void print();
        string str();
        string trad();
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
        int get_key() {
            return t_key;
        }
        int get_line() {
            return line;
        }
        int get_col() {
            return col;
        }
        string get_lex() {
            return lexeme;
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
    "end", "while", "elif", "else", "in", "desde", "todo", "nil", "leer", "EOF"
};

const string Token::TOKNSTRS [Token::N_TOKENS] = {
    "{", "}", "#", "[", "]", "(", ")", ">", "<", ">=", "<=", "==", ".", "!=",
    "&&", "||", "!", "+", "-", "*", "/", "%", "^", "=", ":", ",", "\n",
    "token_string", "identificador", "ERROR", "token_entero", "token_float",
    // TODO: don't repeat data!
    "log", "false", "true", "importar", "for", "if", "funcion", "retorno",
    "end", "while", "elif", "else", "in", "desde", "todo", "nil", "leer", "EOF"
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

string Token::str() {
    if(t_key == Token::TOKEN_ID || t_key == Token::TOKEN_INT || t_key == Token::TOKEN_FLOAT) {
        return lexeme;
    } else if(t_key == Token::TOKEN_STRING) {
        return "\"" + lexeme + "\"";
    } else return (t_key > 0 && t_key <= Token::N_TOKENS) ? 
        Token::TOKNSTRS[--t_key] : "ERROR_STR";
}

string Token::trad() {
    switch(t_key) {
        case Token::TOKEN_ID:
        case Token::TOKEN_FLOAT:
        case Token::TOKEN_INT:
            return lexeme;
        case Token::TOKEN_NOT:
            return "not";
        case Token::TOKEN_OR:
            return "or";
        case Token::TOKEN_AND:
            return "and";
        case Token::TOKEN_POT:
            return "**";
        case Token::RWORD_FUNCION:
            return "def";
        case Token::RWORD_TRUE:
            return "True";
        case Token::RWORD_FALSE:
            return "False";
        case Token::RWORD_IMPORTAR:
            return "import";
        case Token::RWORD_DESDE:
            return "from";
        case Token::RWORD_LOG:
            return "print";
        case Token::RWORD_RETORNO:
            return "return";
        case Token::RWORD_LEER:
            return "input";
        default:
            return this->str();
    }
}

string Token::get_key_str(int key) {
    return (key > 0 && key <= Token::N_TOKENS) ? Token::TOKNSTRS[--key] : "ERROR_TKN";
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
    cout  << "<" << Token::get_key_name(t_key) << ",";
    if(t_class == T_LEX)
        cout  << lexeme << ",";
    cout  << line << "," << col << ">\n";
}

//
// ??? FETCH TOKEN ????????????????????????????????????????????????????????????????
//

Token currentToken;
int global_state, global_line_it, global_col_it;
string global_line;

void catch_error_lexico() {
    cout  << ">>> Error lexico(linea:" << global_line_it << ",posicion:" << global_col_it << ")\n";
    exit(EXIT_FAILURE);
}

bool tl_getline() {
    if(getline(cin, global_line)) {
        global_line+='\n';
        global_col_it = 1;
        global_line_it++;
        return true;
    }
    return false;
}

Token get_next_token() {
    Token token;
    string buffer = "";

    int icol = 1;
    for(int ncol = global_col_it; ncol <= global_line.length(); ncol++) {
        char c = global_line[ncol-1];
        //cout  << "char: " << c << ", estado: " << global_state << ", buffer: " << buffer << ", ncol: " << ncol << "\n";

        if(c == '#') {
            Token tok(Token::T_OP, Token::TOKEN_NEW_LINE, global_line_it, ncol);
            tl_getline();
            return tok;
        }
        //cout  << int(c) << '\n';
        if(int(c) == 4) {
            Token end(Token::T_OP, Token::TOKEN_EOF, global_line_it, ncol);
            return end;
        }
        
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
                    case ')': case '+': case '*': case '/': case '%':
                    case '^': case ':': case ',': case '\n':
                    {
                        Token op(Token::T_OP, Token::get_op_key(c), global_line_it, ncol);
                        global_col_it = ncol+1;
                        if(c == '\n')   tl_getline();
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
                            cout  << "*** ERROR: operand t_key is not defined, somehow? like, how the fuck did this happen?\n";
                        }
                    } break;
                    
                    case '"':
                    {
                        global_state = ESTADO_STRING;
                        icol = ncol;
                    } break;
                    
                    case '.':
                    {
                        if(is_number(global_line[ncol])) {
                            global_state = ESTADO_FLOAT;
                            icol = ncol;
                            buffer = "0.";
                        } else {                        
                            global_col_it = ncol+1;
                            Token pnt(Token::T_OP, Token::TOKEN_POINT, global_line_it, ncol);
                            return pnt;
                        }
                    } break;

                    case '-':
                    {
                        char cx;
                        int auxcol = ncol-2;
                        while(auxcol >= 0) {
                            cx = global_line[auxcol];
                            if(is_number(cx) || is_letter(cx)) {
                                global_col_it = ncol+1;
                                //cout  << "new char: " << global_line[global_col_it-1] << '\n';
                                Token menos(Token::T_OP, Token::TOKEN_MENOS, global_line_it, ncol);
                                global_state = ESTADO_INICIAL;
                                return menos;
                            } else if(cx != ' ')    break;
                            auxcol--;
                        }
                        global_state = ESTADO_INT;
                        icol = ncol;
                        buffer = "-";
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
                if(is_letter(c) || is_number(c) || c == '_') {  // still an id
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
                cout  << "*** ERROR: Automaton error (reached undefined global_state)\n";
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
    cout  << '<' << currentToken.get_line() << ':' << currentToken.get_col()
        << "> Error sintactico. Encontrado: \'"
        << currentToken.str() << "'; se esperaba: ";

    // order lexicographically
    vector<string> names;
    for(int i=0; i<size; i++) {
        names.push_back(Token::get_key_str(expectedTokens[i]));
    }
    sort(names.begin(), names.end());
    for(int i=0; i<size; i++) {
        cout  << '\'' << names[i] << "'";
        if(i==size-1)
            cout  << ".\n";
        else
            cout  << ", ";
    }
    exit(EXIT_SUCCESS);
}

string getIndent(int indent) {
    string tabs = "";
    for(int i=0; i<indent; i++)    tabs+='\t';
    return tabs;
}

class Grammar {
    private:
        static const int N_OPMATH = 6;
        static int ARR_OPMATH[N_OPMATH];

        static const int N_OPSTRCMP = 2;
        static int ARR_OPSTRCMP[N_OPSTRCMP];

        static const int N_OPBIN = 4;
        static int ARR_OPBIN[N_OPBIN];

        static const int N_OPNUM = 6;
        static int ARR_OPNUM[N_OPNUM];

        static const int N_RWFUNC = 2;
        static int ARR_RWFUNC[N_RWFUNC];
    public:
        static bool ismathop(int key) {
            for(int i=0; i<N_OPMATH; i++) {
                if(ARR_OPMATH[i] == key)
                    return true;
            }
            return false;
        }

        static bool isopbin(int key) {
            for(int i=0; i<N_OPBIN; i++) {
                if(ARR_OPBIN[i] == key)
                    return true;
            }
            return false;
        }

        static bool isopnum(int key) {
            for(int i=0; i<N_OPNUM; i++) {
                if(ARR_OPNUM[i] == key)
                    return true;
            }
            return false;
        }

        static bool isrwfunc(int key) {
            for(int i=0; i<N_RWFUNC; i++) {
                if(ARR_RWFUNC[i] == key)
                    return true;
            }
            return false;
        }

        static bool isparamval(int key) {
            return key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT || key == Token::TOKEN_STRING
                || key == Token::RWORD_TRUE || key == Token::RWORD_FALSE || key == Token::TOKEN_ID;
        }

        static string ARRAY() {
            // cout  << "ARRAY -> ";
            int key = currentToken.get_key();
            string tradParams = "";

            if(key == Token::TOKEN_COR_IZQ) {
                followup(Token::TOKEN_COR_IZQ);
                tradParams = ARRAYPARAMS();
                followup(Token::TOKEN_COR_DER);
            } else {
                int exptoks[] = {Token::TOKEN_COR_IZQ};
                catch_error_sintactico(exptoks, 1);
            }

            return "[" + tradParams + "]";
        }

        static string ARRAYPARAMS() {
            // cout << "ARRAYPARAMS -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_ID || key == Token::TOKEN_FLOAT
                || key == Token::TOKEN_STRING || key == Token::TOKEN_INT
                || key == Token::TOKEN_PAR_IZQ) {
                    trad += PARAMVAL();
                    trad += PARAMSNEXT();
            } else {
                int expToks[] = {Token::TOKEN_ID, Token::TOKEN_FLOAT, Token::TOKEN_STRING,
                Token::TOKEN_INT, Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(expToks, 5);
            }

            return trad;
        }

        static string ASSIGNVAL() {
            // cout  << "ASSIGNVAL -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_ID || key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT
                || key == Token::TOKEN_STRING || key == Token::RWORD_TRUE || key == Token::RWORD_FALSE
                || key == Token::RWORD_NIL) {
                trad = PARAMVAL();
            } else if(key == Token::TOKEN_LLAVE_IZQ) {
                trad = STRUCT();
            } else if(key == Token::TOKEN_COR_IZQ) {
                trad = ARRAY();
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_INT, Token::TOKEN_FLOAT,
                    Token::TOKEN_STRING, Token::RWORD_TRUE, Token::RWORD_FALSE,
                    Token::RWORD_NIL, Token::TOKEN_LLAVE_IZQ, Token::TOKEN_COR_IZQ, Token::TOKEN_MENOS, Token::TOKEN_NOT};
                sort(exptoks, exptoks + 10);
                catch_error_sintactico(exptoks, 11);
            }

            return trad;
        }

        static string BLOCK(int indent){
            // cout  << "BLOCK(" << indent << ") -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_LLAVE_IZQ) {
                followup(Token::TOKEN_LLAVE_IZQ);
                trad += BLOCKINSTS(indent);
                followup(Token::TOKEN_LLAVE_DER);
            } else if(key == Token::TOKEN_ID || key == Token::TOKEN_INT 
                || key == Token::TOKEN_FLOAT || key == Token::TOKEN_STRING
                || isrwfunc(key) || key == Token::RWORD_RETORNO
                || key == Token::RWORD_FOR || key == Token::RWORD_WHILE
                || key == Token::RWORD_IF) {
                trad = INST(indent);
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                return BLOCK(indent);
            }

            return trad;
        }
        
        static string BLOCKINSTS(int indent) {
            // cout  << "BLOCKINSTS(" << indent << ") -> ";
            int key = currentToken.get_key();
            string trad = "";
            
            if(key == Token::TOKEN_LLAVE_DER || key == Token::RWORD_ELSE) {
                return ""; // FIXME: program could end with } !
            } else if(key != Token::TOKEN_EOF && key != Token::TOKEN_NEW_LINE) {
                trad += INST(indent);
                trad += BLOCKINSTS(indent);
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                trad += BLOCKINSTS(indent);
            } else {
                int exptoks[] = {Token::TOKEN_LLAVE_IZQ};
                catch_error_sintactico(exptoks, 1);
            };
            return trad;
        }

        static string CALL() {
            // cout  << "CALL -> ";
            int key = currentToken.get_key();
            string tradParams = "";

            if(key == Token::TOKEN_PAR_IZQ) {
                followup(Token::TOKEN_PAR_IZQ);
                tradParams = PARAMS();
                followup(Token::TOKEN_PAR_DER);
            } else {
                int exptoks[] = {Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(exptoks, 1);
            }

            return "(" + tradParams + ")";
        }

        static string CONDNOT() {
            // cout  << "CONDNOT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_ID) {
                trad = ID();
            } else if(key == Token::TOKEN_PAR_IZQ) {
                trad = INITCOND();
            } else {
                int expToks[] = {Token::TOKEN_ID, Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(expToks, 1);
            }

            return trad;
        }

        static void ELSENEXT(int indent) {
            // cout  << "ELSENEXT -> ";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_ID || key == Token::TOKEN_INT 
                || key == Token::TOKEN_FLOAT || key == Token::TOKEN_STRING
                || key == Token::TOKEN_LLAVE_IZQ) {
                BLOCK(indent);
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                ELSENEXT(indent);
            } else if(key == Token::RWORD_IF) {
                IF(indent);
            } else if(key == Token::TOKEN_LLAVE_IZQ) {
                BLOCK(indent);
            } else {
                int exptoks[] = {Token::RWORD_IF, Token::TOKEN_LLAVE_IZQ};
                catch_error_sintactico(exptoks, 2);
            }
        }

        static string FIELD() {
            // cout  << "FIELD -> ";
            int key = currentToken.get_key();
            string trad = "";
            
            if(key == Token::TOKEN_ID) {
                trad += '"';
                trad += followup(Token::TOKEN_ID);
                followup(Token::TOKEN_DOS_PUNTOS);
                trad += "\":";
                trad += PARAMVAL();
            } else {
                int exptoks[] = {Token::TOKEN_ID};
                catch_error_sintactico(exptoks, 1);
            }

            return trad;
        }

        static string FIELDS() {
            // cout  << "FIELDS -> ";
            int key = currentToken.get_key();
            string trad = "";
            
            if(key == Token::TOKEN_ID) {
                trad += FIELD();
                trad += FIELDNEXT();
            } else if(key == Token::TOKEN_LLAVE_DER) {
                return "";
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_LLAVE_DER};
                catch_error_sintactico(exptoks,2);
            }

            return trad;
        }

        static string FIELDNEXT() {
            // cout  << "FIELDNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_COMMA) {
                trad += followup(Token::TOKEN_COMMA);
                trad += FIELDS();
            } else if(key == Token::TOKEN_LLAVE_DER) {
                return "";
            } else {
                int exptoks[] = {Token::TOKEN_COMMA, Token::TOKEN_LLAVE_DER};
                catch_error_sintactico(exptoks,2);
            }

            return trad;
        }

        static string FUNC() {
            // cout  << "FUNC -> ";
            int key = currentToken.get_key();
            string tradId = "", tradParams = "", tradInsts = "";
            if(key == Token::RWORD_FUNCION) {
                followup(Token::RWORD_FUNCION);
                tradId = followup(Token::TOKEN_ID);
                followup(Token::TOKEN_PAR_IZQ);
                tradParams = FUNCPARAMS();
                followup(Token::TOKEN_PAR_DER);
                followup(Token::TOKEN_NEW_LINE);
                tradInsts = FUNCINSTS();
                followup(Token::RWORD_END);
                followup(Token::RWORD_FUNCION);
            } else {
                int exptoks[] = {Token::RWORD_FUNCION};
                catch_error_sintactico(exptoks,1);
            }

            return "def " + tradId + "(" + tradParams + ")" + ":\n" + tradInsts;
        }

        static string FUNCS() {
            // cout  << "FUNCS -> ";
            int key = currentToken.get_key();
            string tradFunc = "", tradNext = "";
            if(key == Token::RWORD_FUNCION) {
                tradFunc = FUNC();
                followup(Token::TOKEN_NEW_LINE);
                tradNext += FUNCS();
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                tradNext += FUNCS();
            } else return "";

            return tradFunc + tradNext;
        }

        static void FUNCBLOCK() {
            // cout  << "FUNCBLOCK -> ";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_LLAVE_IZQ) {
                followup(Token::TOKEN_LLAVE_IZQ);
                FUNCINSTS();
                followup(Token::TOKEN_LLAVE_DER);
            } else {
                int exptoks[] = {Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(exptoks, 1);
            }
        }
        
        static string FUNCINST() {  // TODO: indent
            // cout  << "FUNCINST -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_ID) {
                trad += ID();
                trad += INSTIDNEXT();
            } else if(key == Token::RWORD_IF) {
                trad += IF(1);
            } else if(key == Token::RWORD_WHILE) {
                trad += WHILE(1);
            } else if(key == Token::RWORD_FOR) {
                trad = INITFOR(1);
            } else if(key == Token::RWORD_IMPORTAR) {
                trad += IMPORT();
            } else if(key == Token::RWORD_RETORNO) {
                trad += RETORNO();
            } else if(isrwfunc(key)) {
                trad += RWFUNC();
            } else if(key == Token::TOKEN_NEW_LINE || key == Token::RWORD_END) {
                return "";
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::RWORD_IF, Token::RWORD_WHILE,
                    Token::RWORD_RETORNO, Token::RWORD_FOR, Token::RWORD_IMPORTAR, Token::RWORD_END};
                catch_error_sintactico(exptoks, 7);            
            }

            return '\t' + trad + '\n';
        }

        static string FUNCINSTS() {
            // cout  << "FUNCINSTS -> ";
            int key = currentToken.get_key();
            string tradInst = "", tradNext = "";
            if(key == Token::TOKEN_ID || key == Token::RWORD_IF || key == Token::RWORD_WHILE 
                || key == Token::RWORD_RETORNO || key == Token::RWORD_FOR || key == Token::RWORD_IMPORTAR
                || isrwfunc(key)) {
                tradInst = FUNCINST();
                tradNext = FUNCINSTS();
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                tradNext = FUNCINSTS();
            } else if(key == Token::RWORD_END) {
                return "";
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::RWORD_IF, Token::RWORD_WHILE,
                    Token::RWORD_RETORNO, Token::RWORD_FOR, Token::RWORD_IMPORTAR, Token::RWORD_END};
                catch_error_sintactico(exptoks, 7);
            }

            return tradInst + tradNext;
        }

        static string FUNCPARAMS() {
            // cout << "FUNCPARAMS -> ";
            int key = currentToken.get_key();
            string tradId = "", tradNext = "";
            if(key == Token::TOKEN_PAR_DER) {
                return "";
            } else if(key == Token::TOKEN_ID) {
                tradId = followup(Token::TOKEN_ID);
                tradNext = FUNCPARAMSNEXT();
            } else {
                int expToks[] = {Token::TOKEN_ID, Token::TOKEN_PAR_DER};
                catch_error_sintactico(expToks, 2);
            }

            return tradId + tradNext;
        }

        static string FUNCPARAMSNEXT() {
            // cout << "FUNCPARAMSNEXT -> ";
            int key = currentToken.get_key();
            string tradId = "", tradNext = "";
            if(key == Token::TOKEN_COMMA) {
                followup(key);
                tradId = followup(Token::TOKEN_ID);
                tradNext = FUNCPARAMSNEXT();
            } else if(key == Token::TOKEN_PAR_DER)
                return "";
            else {
                int expToks[] = {Token::TOKEN_COMMA, Token::TOKEN_PAR_DER};
                catch_error_sintactico(expToks, 2);
            }

            return ", " + tradId + tradNext;
        }

        static string ID() {  
            // cout  << "ID -> ";
            int key = currentToken.get_key();
            string tradId = "", tradNext = "";
            if(key == Token::TOKEN_ID) {
                tradId = followup(Token::TOKEN_ID);
                tradNext = IDNEXT();
            } else {
                int expToks[] = {Token::TOKEN_ID};
                catch_error_sintactico(expToks, 1);
            }

            return tradId + tradNext;
        }

        static string IDARRAY() {
            // cout  << "IDARRAY -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_ID) {
                trad = ID();
            } else if(key == Token::Token::TOKEN_COR_IZQ) {
                trad = ARRAY();
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_COR_IZQ};
                catch_error_sintactico(exptoks, 2);
            }

            return trad;
        }

        static string IDCALL() {
            // cout  << "IDCALL -> ";
            int key = currentToken.get_key();
            string tradId = "", tradNext = "";

            if(key == Token::TOKEN_ID) {
                tradId = ID();
                tradNext = IDCALLNEXT();
            } else {
                int exptoks[] = {Token::TOKEN_ID};
                catch_error_sintactico(exptoks, 1);
            }

            return tradId + tradNext;
        }

        static string IDCALLNEXT() {
            // cout  << "IDCALLNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_PAR_IZQ) {
                trad = CALL();
            } else return "";

            return trad;
        }

        static string IDNEXT() {
            // cout  << "IDNEXT -> ";
            string tradId = "", tradNext = "";
            int key = currentToken.get_key();
            if(key == Token::TOKEN_POINT) {
                followup(Token::TOKEN_POINT);
                tradId += "[\"";
                tradId += followup(Token::TOKEN_ID);
                tradId += "\"]";
                tradNext = IDNEXT();
            } else if(key == Token::TOKEN_COR_IZQ) {
                tradNext = ARRAY();
            }

            return tradId + tradNext;
        }

        static string IF(int indent) {
            // cout  << "IF -> \n";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::RWORD_IF) {
                trad += followup(Token::RWORD_IF);
                followup(Token::TOKEN_PAR_IZQ);
                trad += ' ';
                trad += INITCOND();
                followup(Token::TOKEN_PAR_DER);
                trad += ":\n";
                trad += BLOCK(indent+1);
                IFNEXT();
            } else {
                int exptoks[] = {Token::RWORD_IF};
                catch_error_sintactico(exptoks, 1);
            }

            return trad;
        }

        static void IFNEXT() {
            // cout  << "IFNEXT -> ";
            int key = currentToken.get_key();

            if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                IFNEXT();
            } else if(key == Token::RWORD_ELSE) {
                followup(Token::RWORD_ELSE);
                ELSENEXT(1);
            } else return;  // TODO: hmm
        }

        static string IMPORT() {
            // cout  << "IMPORT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::RWORD_IMPORTAR) {
                trad += followup(Token::RWORD_IMPORTAR);
                trad += ' ';
                trad += ID();
            } else if(key == Token::RWORD_DESDE) {
                trad += followup(Token::RWORD_DESDE);
                trad += ' ';
                trad += ID();
                trad += ' ';
                trad += followup(Token::RWORD_IMPORTAR);
                trad += ' ';
                trad += ID();
            } else {
                int exptoks[] = {Token::RWORD_IMPORTAR};
                catch_error_sintactico(exptoks, 1);
            }

            return trad;
        }

        static string INITCOND() {
            // cout  << "COND -> ";
            int key = currentToken.get_key();
            string trad = "";

            switch(key) {
                case Token::RWORD_TRUE: case Token::RWORD_FALSE: {
                    trad += TFVAL();
                    trad += TFNEXT();
                } break;

                case Token::TOKEN_ID: case Token::TOKEN_STRING: {
                    trad += PARAMVAL();
                    trad += INITCONDNEXT();
                } break;

                case Token::TOKEN_INT: case Token::TOKEN_FLOAT: {
                    trad += INITMATH();
                    trad += INITCONDNEXT();
                } break;

                case Token::TOKEN_NOT: {
                    trad += followup(Token::TOKEN_NOT);
                    trad += CONDNOT();
                } break;

                case Token::TOKEN_PAR_IZQ: {
                    followup(Token::TOKEN_PAR_IZQ);
                    trad += INITCOND();
                    followup(Token::TOKEN_PAR_DER);
                } break;

                default: {
                    int expToks[] = {Token::RWORD_TRUE, Token::RWORD_FALSE, Token::TOKEN_ID, 
                    Token::TOKEN_INT, Token::TOKEN_PAR_IZQ, Token::TOKEN_NOT};
                    catch_error_sintactico(expToks, 6);
                } break;
            }
            
            return trad;
        }

        static string INITCONDNEXT() {
            // cout  << "INITCONDNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(isopbin(key)) {
                trad += ' ';
                trad += OPBIN();
                trad += ' ';
                trad += PARAMVAL();
            } else if(isopnum(key)) {
                trad += OPNUM();
                trad += INITMATH();
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
            } else if(key == Token::TOKEN_PAR_DER) {
                return "";
            } else {
                // TODO: incomplete report
                int expToks[] = {Token::TOKEN_AND};
                catch_error_sintactico(expToks, 1);
            }

            return trad;
        }

        static string INITFOR(int indent) {
            // cout  << "INITFOR(" << indent << ") -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::RWORD_FOR) {
                trad += followup(Token::RWORD_FOR);
                trad += ' ';
                trad += followup(Token::TOKEN_ID);
                trad += ' ';
                trad += followup(Token::RWORD_IN);
                trad += ' ';
                trad += IDARRAY();
                trad += ":\n";
                trad += BLOCK(indent+1);
            }

            return trad;
        }

        static string INITMATH() {
            // cout  << "INITMATH -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_ID) {
                trad += IDCALL();
                trad += INITMATHNEXT();
            } else if(key == Token::TOKEN_FLOAT || key == Token::TOKEN_INT) {
                trad += NUMBER();
                trad += INITMATHNEXT();
            } else if(key == Token::TOKEN_STRING) {
                trad += followup(Token::TOKEN_STRING);
                trad += INITMATHNEXT();
            } else if(key == Token::TOKEN_PAR_IZQ) {
                trad += followup(Token::TOKEN_PAR_IZQ);
                trad += INITMATH();
                trad += followup(Token::TOKEN_PAR_DER);
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_INT, Token::TOKEN_FLOAT};
                catch_error_sintactico(exptoks,3);
            }

            return trad;
        }

        static string INITMATHNEXT() {
            // cout  << "INITMATHNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(ismathop(key)) {
                trad += MATHOP();
                trad += INITMATH();
            } else if(isopnum(key)) {
                trad += OPNUM();
                trad += INITMATH();
            } else {
                return "";
            }

            return trad;
        }
        
        static string INST(int indent) {
            // cout  << "INST(" << indent << ") -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_ID) {
                trad += ID();
                trad += INSTIDNEXT();
            } else if(key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT) {
                trad += followup(key);
                trad += STATIDNEXT();
            } else if(key == Token::RWORD_IF) {
                trad = IF(indent);
            } else if(key == Token::RWORD_WHILE) {
                trad = WHILE(indent);
            } else if(key == Token::RWORD_FOR) {
                trad = INITFOR(indent);
            } else if(isrwfunc(key)) {
                trad = RWFUNC();
            } else if(key == Token::RWORD_IMPORTAR || key == Token::RWORD_DESDE) {
                trad = IMPORT();
            } else if(key == Token::TOKEN_STRING) {
                trad += followup(Token::TOKEN_STRING);
                trad += STATIDNEXT();
            } else if(key == Token::TOKEN_LLAVE_IZQ) {
                trad = STRUCT();
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                trad += '\n';
                trad = INST(indent);
            } else if(key == Token::RWORD_RETORNO) {
                trad += RETORNO();
            } else {
                int exptoks[] = {-1};   // TODO: actual values
                catch_error_sintactico(exptoks, 1);
            } 
            return getIndent(indent) + trad + '\n';
        }
        
        static string INSTIDNEXT() {
            // cout  << "INSTIDNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_PAR_IZQ) {
                trad = CALL();
            } else if(key == Token::TOKEN_ASSIGN) {
                trad += followup(Token::TOKEN_ASSIGN);
                trad += ASSIGNVAL();
            } else if(ismathop(key)) {
                trad += MATHOP();
                trad += PARAMVAL();
            } else if(key == Token::TOKEN_NEW_LINE || key == Token::TOKEN_PAR_DER 
                || key == Token::RWORD_ELSE || key == Token::TOKEN_EOF) {
                return "";
            } else {
                int exptoks[] = {Token::TOKEN_PAR_IZQ, Token::TOKEN_POINT, Token::TOKEN_NEW_LINE};
                catch_error_sintactico(exptoks, 3);
            }

            return trad;
        }

        static string INSTS(int indent) {
            // cout  << "INSTS(" << indent << ") -> ";
            int key = currentToken.get_key();
            string trad = "";
            
            if(key != Token::TOKEN_EOF && key != Token::TOKEN_NEW_LINE) {
                trad += INST(indent);
                trad += INSTS(indent);
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                trad += INSTS(indent);
            }            
            return trad;
        }

        static string MATHOP() {
            // cout  << "MATHOP -> ";
            int key = currentToken.get_key();
            string trad = "";
            
            if(ismathop(key)) {
                trad = followup(key);
            } else {
                catch_error_sintactico(ARR_OPMATH, N_OPMATH);
            }

            return trad;
        }
        
        static string NUMBER() {
            // cout  << "NUMBER -> ";
            int tokenType = currentToken.get_key();
            string trad = "";
            if(tokenType == Token::TOKEN_INT || tokenType == Token::TOKEN_FLOAT)
                trad = followup(tokenType);
            else {
                int expToks[] = {Token::TOKEN_INT, Token::TOKEN_FLOAT};
                catch_error_sintactico(expToks, 2);
            }

            return trad;
        }

        static string OPBIN() {
            // cout  << "OPBIN -> ";
            int key = currentToken.get_key();
            string trad = "";
            if(isopbin(key)) {
                trad = followup(key);
            } else {
                catch_error_sintactico(ARR_OPBIN, N_OPBIN);
            }

            return trad;
        }

        static string OPNUM() {
            // cout  << "OPNUM -> ";
            string trad = "";
            int key = currentToken.get_key();
            if(isopnum(key)) {
                trad = followup(key);
            } else {
                catch_error_sintactico(ARR_OPNUM, N_OPNUM);
            }
            return trad;
        }

        static string PARAMS() {
            // cout  << "PARAMS ->";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_PAR_DER || key == Token::TOKEN_COR_DER) {   // FIXME: just, ugh.
                return "";
            } else if(key == Token::TOKEN_ID || key == Token::TOKEN_FLOAT
                || key == Token::TOKEN_STRING || key == Token::TOKEN_INT
                || key == Token::RWORD_TRUE || key == Token::RWORD_FALSE
                || key == Token::TOKEN_PAR_IZQ || key == Token::TOKEN_COR_IZQ) {
                trad += PARAMVAL();
                trad += PARAMSNEXT();
            } else if(key == Token::TOKEN_NOT) {
                followup(Token::TOKEN_NOT);
                CONDNOT();
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                PARAMS();
            } else {
                int expToks[] = {Token::TOKEN_ID, Token::TOKEN_FLOAT, Token::TOKEN_STRING,
                Token::TOKEN_INT, Token::RWORD_TRUE, Token::RWORD_FALSE, Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(expToks, 7);
            }
            
            return trad;
        }

        static string PARAMSNEXT() {
            // cout  << "PARAMSNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_COMMA) {
                followup(Token::TOKEN_COMMA);
                trad += ",";
                trad += PARAMS();
            } else if(key == Token::TOKEN_PAR_DER || key == Token::TOKEN_COR_DER) {
                return "";
            } else if(key == Token::TOKEN_NEW_LINE) {
                followup(Token::TOKEN_NEW_LINE);
                PARAMSNEXT();
            } else {
                int exptoks[] = {Token::TOKEN_PAR_DER};
                catch_error_sintactico(exptoks, 1);
            }

            return trad;
        }

        static string PARAMVAL() {
            // cout  << "PARAMVAL -> ";
            int key = currentToken.get_key();
            string trad = "";
            
            if(key == Token::TOKEN_INT || key == Token::TOKEN_FLOAT
                || key == Token::TOKEN_STRING) {
                trad += INITMATH();
                trad += STATIDNEXT();
            } else if( key == Token::RWORD_TRUE || key == Token::RWORD_FALSE) {
                trad = followup(key);
            } else if(key == Token::TOKEN_ID) {
                trad += IDCALL();
                trad += STATIDNEXT();
            } else if(key == Token::RWORD_NIL) {
                followup(Token::RWORD_NIL);
            } else if(key == Token::TOKEN_PAR_IZQ) {
                trad += followup(Token::TOKEN_PAR_IZQ);
                trad += PARAMVAL();
                trad += followup(Token::TOKEN_PAR_DER);
                trad += STATIDNEXT();
            } else if(key == Token::TOKEN_COR_IZQ) {
                trad += ARRAY();
            } else {
                int exptoks[] = {Token::TOKEN_ID, Token::TOKEN_INT, Token::TOKEN_FLOAT,
                    Token::TOKEN_STRING, Token::RWORD_TRUE, Token::RWORD_FALSE};
                catch_error_sintactico(exptoks, 6);
            }

            return trad;
        }

        static string RETORNO() {
            // cout  << "RETORNO -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::RWORD_RETORNO) {
                trad += followup(Token::RWORD_RETORNO);
                trad += RETVAL();
            } else {
                int exptoks[] = {Token::RWORD_RETORNO};
                catch_error_sintactico(exptoks, 1);
            }

            return trad;
        }
        
        static string RETVAL() {
            // cout  << "RETVAL -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_LLAVE_DER) {
                return "";
            } else if(key == Token::TOKEN_PAR_IZQ) {
                followup(Token::TOKEN_PAR_IZQ);
                trad += " ";
                trad += PARAMVAL();
                followup(Token::TOKEN_PAR_DER);
            } else {
                int exptoks[] = {Token::TOKEN_LLAVE_DER, Token::TOKEN_PAR_IZQ};
                catch_error_sintactico(exptoks, 2);
            }

            return trad;
        }

        static string RWFUNC() {
            // cout  << "RWFUNC -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(isrwfunc(key)) {
                string tradKey = "", tradCall = "";
                tradKey = followup(key);
                tradCall = CALL();
                if(key == Token::RWORD_LEER) {
                    string id = tradCall.substr(1, tradCall.length()-2);
                    trad = id + "=" + tradKey + "()";
                }
            } else if(key == Token::RWORD_LEER) {
            } else {
                catch_error_sintactico(ARR_RWFUNC, N_RWFUNC);
            }

            return trad;
        }

        static string STATIDNEXT() {
            // cout  << "STATIDNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(ismathop(key)) {
                trad = INITMATHNEXT();
            } else if(isopbin(key) || isopnum(key)) {
                trad = INITCONDNEXT();
            } else if(key == Token::TOKEN_PAR_DER || key == Token::TOKEN_NEW_LINE
                || key == Token::TOKEN_COR_DER || key == Token::TOKEN_COMMA
                || key == Token::TOKEN_LLAVE_DER) 
                return "";
            else {
                int exptoks[] = {Token::TOKEN_MAS, Token::TOKEN_MENOS, Token::TOKEN_MUL,
                    Token::TOKEN_DIV, Token::TOKEN_IGUAL_NUM, Token::TOKEN_DIFF_NUM};
                catch_error_sintactico(exptoks, 6);
            }

            return trad;
        }

        static string STRUCT() {
            // cout  << "STRUCT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_LLAVE_IZQ) {
                trad += followup(Token::TOKEN_LLAVE_IZQ);
                trad += FIELDS();
                trad += followup(Token::TOKEN_LLAVE_DER);
            } else {
                int exptoks[] = {Token::TOKEN_LLAVE_IZQ};
                catch_error_sintactico(exptoks, 1);
            }

            return trad;
        }

        static string TFVAL() {
            // cout << "TFVAL -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::RWORD_TRUE || key == Token::RWORD_FALSE) {
                trad += followup(key);
                trad += TFNEXT();
            } else {
                int expToks[] = {Token::RWORD_TRUE, Token::RWORD_FALSE};
                catch_error_sintactico(expToks, 2);
            }

            return trad;
        }

        static string TFNEXT() {
            // cout << "TFNEXT -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::TOKEN_PAR_DER) {  // FIXME: this feels wrong
                return "";
            } else if(isopbin(key)) {
                trad += OPBIN();
                trad += INITCOND();
            } else {
                int expToks[N_OPBIN+1];
                for(int i=0; i<N_OPBIN; i++)
                    expToks[i] = ARR_OPBIN[i];
                expToks[N_OPBIN] = Token::TOKEN_PAR_DER;
                catch_error_sintactico(expToks, N_OPBIN+1);
            }

            return trad;
        }

        static string WHILE(int indent) {
            // cout  << "WHILE -> ";
            int key = currentToken.get_key();
            string trad = "";

            if(key == Token::RWORD_WHILE) {
                trad += followup(Token::RWORD_WHILE);
                followup(Token::TOKEN_PAR_IZQ);
                trad += ' ';
                trad += INITCOND();
                followup(Token::TOKEN_PAR_DER);
                trad += ":\n";
                trad += BLOCK(indent+1);
            } else {
                int expToks[] = {Token::RWORD_WHILE};
                catch_error_sintactico(expToks, 1);                
            }

            return trad;
        }

        static string followup(int expectedKey) {
            string trad = "";
            if(currentToken.get_key() == expectedKey) {
                trad = currentToken.trad();
                currentToken = get_next_token();
                if(currentToken.get_key() < 1) {
                    Token token(Token::T_OP, Token::TOKEN_EOF, global_line_it, global_col_it);
                    currentToken = token;
                }
                // cout  << "OK\n\n";
                // currentToken.print();
            } else {
                int expToks[] = {expectedKey};
                catch_error_sintactico(expToks, 1);
            }

            return trad;
        }
};

int Grammar::ARR_OPMATH[Grammar::N_OPMATH] = {
    Token::TOKEN_MAS, Token::TOKEN_MENOS, Token::TOKEN_MUL, Token::TOKEN_DIV,
    Token::TOKEN_POT, Token::TOKEN_MOD
};

int Grammar::ARR_OPSTRCMP[Grammar::N_OPSTRCMP] = {
    Token::TOKEN_IGUAL_NUM, Token::TOKEN_DIFF_NUM
};

int Grammar::ARR_OPBIN[Grammar::N_OPBIN] = {
    Token::TOKEN_AND, Token::TOKEN_IGUAL_NUM, Token::TOKEN_OR,
    Token::TOKEN_DIFF_NUM
};

int Grammar::ARR_OPNUM[Grammar::N_OPNUM] = {
    Token::TOKEN_MAYOR_IG, Token::TOKEN_MENOR, Token::TOKEN_MENOR_IG,
    Token::TOKEN_IGUAL_NUM, Token::TOKEN_DIFF_NUM, Token::TOKEN_MAYOR
};

int Grammar::ARR_RWFUNC[Grammar::N_RWFUNC] = {
    Token::RWORD_LEER, Token::RWORD_LOG
};

class GrammarProgram {
    public:
        static string PROGRAM() {
            int key = currentToken.get_key();
            string tradFuncs = "";
            string tradInsts = "";
            if(key == Token::RWORD_FUNCION) {
                tradFuncs = Grammar::FUNCS();
                tradInsts = Grammar::INSTS(0);
            } else if(key != Token::TOKEN_EOF) {
                tradInsts = Grammar::INSTS(0);
            } else return "";

            return tradFuncs + '\n' + tradInsts;
        }
};

int main (int argc, char *argv[]) {
    global_state = ESTADO_INICIAL;
    global_line_it = 0;
    
    tl_getline();
    currentToken = get_next_token();
    cout << GrammarProgram::PROGRAM();
    // cout  << "El analisis sintactico ha finalizado correctamente.\n";

    if(global_state != ESTADO_INICIAL) {
        cout  << "finished at state " << global_state << '\n'; 
        --global_line_it;
        catch_error_lexico();
    }

    return 0;
}
