#pragma once
#include "types.h"

typedef struct String {

	sizet length;
	sizet capacity;
	char* data;
  
} String;

String* str_create_c(const char* text);
String* str_create_s(sizet size);

sizet str_len(const String* str);
sizet cstr_len(const char* str);
void str_copy(String* dest, const char* src);
char str_at(String* str, sizet index);
void str_concat(String* s1, const char* s2);
char* cstr_concat(const char* s1, const char* s2);
i8 str_equal(const char* s1, const char* s2);

void str_delete_from_back(String* str, sizet count);
void str_delete(String* str);
void str_print(const String* str);

