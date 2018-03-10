#ifndef LEX_TOKEN_H   // protect against multiple inclusions!
#define LEX_TOKEN_H

#include <string>

class Token {
    protected:
        int type;
        int line;
        int col;
        
    private:    // TODO
        int TYPES[];
        
    public:
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
        
        static bool is_operator(char c);
        static int get_op_key(std::string c);
        
        static std::string type2str(int type);
};

class Operator: public Token {
    public:
        Operator(int t, int l, int c) : Token(t, l, c) {
            
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
        ResWord(int t, int l, int c) : Token(t, l, c) {
            type = t;
            line = l;
            col = c;
        }
        
        static int get_word_key(std::string word);
        static std::string get_word(int idx);
        void print();
};

#endif