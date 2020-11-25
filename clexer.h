#pragma once
#include "string.h"

typedef enum TokenType {
  
	TOK_NUMBER,
	TOK_INDENTIFIER,
	TOK_OPEN_PAREN,
	TOK_CLOSED_PAREN,
	TOK_OPEN_CURLY,
	TOK_CLOSED_CURLY,
	TOK_OPEN_SQUARE,
	TOK_CLOSED_SQUARE,
	TOK_KEYWORD,
	TOK_UNKNOWN

} TokenType;

struct Token {
  


};

void lexer_lex(String* text);
