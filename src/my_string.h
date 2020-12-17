#pragma once
#include "types.h"

#define STR_RESIZE_FACTOR 2

typedef char* string;

enum {
	S_LENGTH,
	S_CAPACITY,
	S_FIELDS
};

#define STR_LENGTH(str) str_field_get(str, S_LENGTH)
#define STR_CAPACITY(str) str_field_get(str, S_CAPACITY)

void* str_create_c(const char* text);
void* str_create_s(sizet size);
void str_release(void* str);
sizet cstr_len(const char* str);
void str_copy(string dest, string src);
void* str_push(void *str, char c);
void* str_resize(void* str);
void* str_concat(string s1, string s2);
void str_pop(string str);
char* str_as_cstr(string str);
i8 cstr_equal(const char* s1, const char* s2);
void str_skip(void* str, sizet count);

sizet str_field_get(void* str, sizet field);
void str_field_set(void* str, sizet field, sizet value);

