#pragma once
#include "string.h"
#include "types.h"
typedef enum TokenType {
	
	TOK_IDENTIFIER = 0,
	TOK_HASH,
	TOK_NUMBER,
	TOK_OPEN_PAREN,
	TOK_CLOSED_PAREN,
	TOK_OPEN_CURLY,
	TOK_CLOSED_CURLY,
	TOK_OPEN_SQUARE,
	TOK_CLOSED_SQUARE,
	TOK_KEYWORD,

} TokenType;

typedef struct Token {
	
	TokenType type;
	sizet length;
	sizet pos;

} Token;

typedef struct Lexer {
	
	Token* tokens;
	sizet size;

} Lexer;


Lexer* lexer_lex(String* text);
void lexer_print_tokens(Lexer* lexer);
