#pragma once
#include "types.h"


typedef struct String {

	char* data = NULL;
	sizet length = 0;
	sizet capacity = 0;
	i16* refCount = NULL;

	char& operator[](sizet index);
	b8 operator==(const String& str);
	b8 operator==(const char* str);
	String operator+(String& str);

	String() = default;
	String(const char* cstr);
	~String();
	String(const String& other);
	String& operator=(const String& other);

	char* as_cstr();

} String;


String str_create(const char* text);
String str_create(sizet size);
String str_create(String& other);
void str_free(String* str);
void str_copy(String* dest, String* src);
void str_push(String* str, char c);
void str_concat(String* s1, char* s2);
i8 cstr_equal(const char* s1, const char* s2);
void str_skip(String* str, sizet count);
void str_clear(String* str);
String str_substring(String* str, sizet start, sizet end);
void str_reverse(String* str);
void str_print(String& str);

template <typename T> class Array;

void str_array_free(Array<String>& arr);

