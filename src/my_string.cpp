#include "my_string.h"
#include "debug.h"
#include "memory.h"
#include <string.h>

String
str_create(const char* text) {
	
	sizet len = strlen(text);

	String out;
	out.capacity = len;
	out.length = len;
	out.data = new char[len];
	memcpy(out.data, text, len * sizeof(char));
	ASSERT_MSG(out.data, "string malloc failed");

	return out;

}

String
str_create(sizet size) {
	
	String out;
	out.capacity = size;
	out.length = 0;
	out.data = new char[size];

	ASSERT_MSG(out.data, "string malloc failed");

	return out;
}

char&
String::operator[](sizet index) {

	return data[index];
}

b8
String::operator==(const String& str) {

	if (str.length == length) {

		for (sizet i = 0; i < length; ++i) {
			if (str.data[i] != data[i])
				return 0;
		}
		return 1;
	}

	return 0;
}
// Only works if dest >= src
void
str_copy(String* dest, String* src) {

	ASSERT(dest->capacity >= src->capacity);

	for (sizet i = 0; i < src->length; ++i) {
		
		dest->data[i] = src->data[i];
	}

}



void
str_push(String* str, char c) {

	if (str->length >= str->capacity) {
		str->capacity *= 2;
		char* old = str->data;
		str->data = new char[str->capacity];
		memcpy(str->data, old, str->length);
		delete[] old;
	}

	str->data[str->length] = c;
	str->length++;
}

void
str_skip(String* str, sizet count) {
  
	str->length += count;
}


void
str_concat(String* s1, char* s2) {
	
	i32 len = strlen(s2);
	for (sizet i = 0; i < len; ++i) {
		str_push(s1, s2[i]);
	}

}

String
str_substring(String* str, sizet start, sizet end) {
	
	sizet len = end - start;
	String out = str_create(len);

	for (sizet i = start; i < end; ++i) {
		
		str_push(&out, str->data[i]);
	}

	return out;
}


i8
cstr_equal(const char* s1, const char* s2) {
	
	sizet i = 0;

	sizet len = strlen(s1);
	for (i; i < len; ++i) {

		if (s1[i] != s2[i]) return 0;
	}

	if (s2[i] != '\0') {
		return 0;
	} else {
		return 1;
	}

}

void
str_clear(String* str) {

	str->length = 0;
}

void str_free(String* str) {

	delete[] str->data;
}