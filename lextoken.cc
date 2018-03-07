#include "lextoken.h"

#include <iostream>
#include <string>

// https://stackoverflow.com/questions/5620256/understanding-how-to-correctly-treat-c-class-constants

const int Token::TOKEN_LLAVE_IZQ    = 1;
const int Token::TOKEN_LLAVE_DER    = 2;
const int Token::TOKEN_COM          = 3;
const int Token::TOKEN_COR_IZQ      = 4;
const int Token::TOKEN_COR_DER      = 5;
const int Token::TOKEN_PAR_IZQ      = 6;
const int Token::TOKEN_PAR_DER      = 7;
const int Token::TOKEN_MAYOR        = 8;
const int Token::TOKEN_MENOR        = 9;
const int Token::TOKEN_MAYOR_IG     = 10;
const int Token::TOKEN_MENOR_IG     = 11;
const int Token::TOKEN_IN           = 12;
const int Token::TOKEN_IGUAL_NUM    = 13;
const int Token::TOKEN_POINT        = 14;
const int Token::TOKEN_DIFF_NUM     = 15;
const int Token::TOKEN_AND = 16;
const int Token::TOKEN_OR = 17;
const int Token::TOKEN_NOT = 18;
const int Token::TOKEN_MAS = 19;
const int Token::TOKEN_MENOS = 20;
const int Token::TOKEN_MUL = 21;
const int Token::TOKEN_DIV = 22;
const int Token::TOKEN_MOD = 23;
const int Token::TOKEN_POD = 24;
const int Token::TOKEN_ASSIGN = 25;

const std::string TYPE2STR [26] =
    { "token_llave_izquierda", "token_llave_derecha", "token_comentario",
    "token_cor_izq", "token_cor_der", "token_par_izq", "token_par_der",
    "token_mayor", "token_menor", "token_mayor_igual", "token_menor_igual",
    "token_in", "token_igual_num", "token_point", "token_diff_num",
    "token_and", "token_or", "token_mas", "token_menos", "token_mul",
    "token_div", "token_mod", "token_pod", "token_assign" };
    
std::string Token::type2str(int type) {
    return TYPE2STR[type];
}

Token::Token(int t, int l, int c) {
    type = t;
    line = l;
    col = c;
}

void Operator::print() {
    std::cout << "<" << Token::type2str(type) << "," << line << "," << col << ">\n";
}

