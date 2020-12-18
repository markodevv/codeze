#pragma once
#include "types.h"

typedef struct String {

	char* data;
	sizet length;
	sizet capacity;

} String;

String str_create_c(const char* text);
String str_create_s(sizet size);
void str_copy(String* dest, String* src);
void str_push(String* str, char c);
void str_concat(String* s1, char* s2);
i8 cstr_equal(const char* s1, const char* s2);
void str_skip(String* str, sizet count);
void str_clear(String* str);
void str_free(String* str);

