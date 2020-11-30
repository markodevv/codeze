#pragma once
#include "my_string.h"
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
	TOK_STRING,
	TOK_SEMICOLON,
	TOK_COMMENT,
	TOK_TYPE,
	TOK_UNKNOWN,
	TOK_TOTAL

} TokenType;


typedef struct Token {

	TokenType type;
	sizet length;
	sizet pos;

} Token;

typedef Token* TokenArray;


TokenArray tokens_make(string args);
void print_tokens(Token* tokens);
