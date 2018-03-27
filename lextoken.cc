#include "lextoken.h"

#include <iostream>
#include <string>

// https://stackoverflow.com/questions/5620256/understanding-how-to-correctly-treat-c-class-constants

const int TOKENS = 32;

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
const int Token::TOKEN_IN = 12;         // in
const int Token::TOKEN_IGUAL_NUM = 13;  // ==
const int Token::TOKEN_POINT = 14;      // .
const int Token::TOKEN_DIFF_NUM = 15;   // !=
const int Token::TOKEN_AND = 16;        // &&
const int Token::TOKEN_OR = 17;         // 
const int Token::TOKEN_NOT = 18;
const int Token::TOKEN_MAS = 19;
const int Token::TOKEN_MENOS = 20;
const int Token::TOKEN_MUL = 21;
const int Token::TOKEN_DIV = 22;
const int Token::TOKEN_MOD = 23;
const int Token::TOKEN_POD = 24;
const int Token::TOKEN_ASSIGN = 25;
const int Token::TOKEN_STRING = 26;
const int Token::TOKEN_ID = 27;
const int Token::TOKEN_RESWORD = 28;
const int Token::TOKEN_INT = 29;
const int Token::TOKEN_FLOAT = 30;
const int Token::TOKEN_DOS_PUNTOS = 31;
const int Token::TOKEN_COMMA = 32;

const std::string TYPE2STR [TOKENS] =
    { "token_llave_izq", "token_llave_der", "token_comentario",
    "token_cor_izq", "token_cor_der", "token_par_izq", "token_par_der",
    "token_mayor", "token_menor", "token_mayor_igual", "token_menor_igual",
    "token_in", "token_igual_num", "token_point", "token_diff_num",
    "token_and", "token_or", "token_not", "token_mas", "token_menos", 
    "token_mul", "token_div", "token_mod", "token_pod", "token_assign",
    "token_string", "id", "token_reserved_word", "int", "float", 
    "token_dos_puntos", "token_comma" };
    
std::string Token::type2str(int type) {
    type--;
    return (type >= 0 && type < TOKENS) ? TYPE2STR[type] : "ERROR_TKN";
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
    } if(s == "in") {
        return Token::TOKEN_IN;
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

void Operator::print() {
    std::cout << "<" << Token::type2str(type) << "," << line << "," << col << ">\n";
}

void Lexeme::print() {
    std::cout << "<" << Token::type2str(type) << "," << lexeme << "," << line << "," << col << ">\n";
}

const int ResWord::N_RWORDS = 10;

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

std::string ResWord::RESWORDS [ResWord::N_RWORDS] = {
    "log", "false", "true", "importar", "for", "if", "funcion", "retorno",
    "end", "while"
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