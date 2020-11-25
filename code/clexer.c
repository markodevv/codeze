#include "clexer.h"
#include "debug.h"

#define INITIAL_TOKEN_CAPACITY 100

static i8
is_char_identifier(char c) {

	switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '_':
      return 1;
    default:
      return 0;

	}
}


static i8
is_str_equal(const char* w1, const char* w2) {
	
	sizet i = 0;
	sizet len = cstr_len(w1);
	for (i; i < len; ++i) {

		if (w1[i] != w2[i]) return 0;
	}
	return 1;

}

static i8
is_keyword(const char* word) {
	
	if (is_str_equal(word, "case")) return 1;
	if (is_str_equal(word, "if")) return 1;
	if (is_str_equal(word, "else")) return 1;
	if (is_str_equal(word, "while")) return 1;
	if (is_str_equal(word, "switch")) return 1;
	if (is_str_equal(word, "for")) return 1;
	if (is_str_equal(word, "continue")) return 1;
	if (is_str_equal(word, "break")) return 1;
	if (is_str_equal(word, "struct")) return 1;
	if (is_str_equal(word, "typedef")) return 1;
	if (is_str_equal(word, "return")) return 1;
	else return 0;

}

static i8
is_number(char c) {
	
	switch(c) {
		
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return 1;
    default:
      return 0;
	  
	}
}


Lexer*
lexer_lex(String* text) {

	sizet capacity = INITIAL_TOKEN_CAPACITY;
	Lexer* lexer = malloc(sizeof(Lexer));
	lexer->tokens = malloc(sizeof(Token) * capacity);

	Token* currTok = lexer->tokens;
	char* begin = text->data;
	char* pos = text->data;

	sizet size = 0;

	while (*pos != '\0') {

		if (size >= capacity) {

			capacity *= 2;
			lexer->tokens = realloc(lexer->tokens, sizeof(Token) * capacity);
			currTok = lexer->tokens + size;
			
		}

		switch(*pos) {

		default:
			pos++;
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			currTok->pos = pos - begin;
			static char word[128];
			i32 i = 0;
			while (is_char_identifier(*pos)) {
				word[i] = *pos;
				i++;
				pos++;
			}
			word[i] = '\0';

			currTok->length = i;
			if (is_keyword(word)) {
				currTok->type = TOK_KEYWORD;
			}
			else {
				currTok->type = TOK_IDENTIFIER;
			}
			currTok++;
			size++;
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			currTok->type = TOK_NUMBER;
			currTok->pos = pos - begin;
			i = 0;
			while (is_number(*pos)) {
				i++;
				pos++;
			}
			currTok->length = i;
			currTok++;
			size++;
			break;

		case '(':
			currTok->type = TOK_OPEN_PAREN;
			currTok->pos = pos - begin;
			currTok->length = 1;
			currTok++;
			pos++;
			size++;
			break;

		case ')':
			currTok->type = TOK_CLOSED_PAREN;
			currTok->length = 1;
			currTok->pos = pos - begin;
			currTok++;
			pos++;
			size++;
			break;

		case '{':
			currTok->type = TOK_OPEN_CURLY;
			currTok->length = 1;
			currTok->pos = pos - begin;
			currTok++;
			pos++;
			size++;
			break;

		case '}':
			currTok->type = TOK_CLOSED_CURLY;
			currTok->length = 1;
			currTok->pos = pos - begin;
			currTok++;
			pos++;
			size++;
			break;
		case '#':
			currTok->type = TOK_HASH;
			currTok->length = 1;
			currTok->pos = pos - begin;
			currTok++;
			pos++;
			size++;
			break;

		}
	}

	lexer->size = size;

	return lexer;

}

static void
print_token(TokenType type) {
	
	switch(type) {
		
	case TOK_IDENTIFIER: printf("%s \n","identifier"); break;
	case TOK_HASH: printf("%s \n", "hash"); break;
	case TOK_NUMBER: printf("%s \n", "number"); break;
	case TOK_OPEN_PAREN: printf("%s \n", "open paren"); break;
	case TOK_CLOSED_PAREN: printf("%s \n", "closed paren"); break;
	case TOK_OPEN_CURLY: printf("%s \n", "open curly"); break;
	case TOK_CLOSED_CURLY: printf("%s \n", "closed curly"); break;
	case TOK_OPEN_SQUARE: printf("%s \n", "open square"); break;
	case TOK_CLOSED_SQUARE: printf("%s \n", "closed square"); break;
	case TOK_KEYWORD: printf("%s \n", "keyword"); break;

	}

}

void
lexer_print_tokens(Lexer* lexer) {

	for (sizet i = 0; i < lexer->size; ++i) {

		print_token(lexer->tokens[i].type);
		
	}
	
}
