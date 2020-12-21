#include "my_string.h"
#include "debug.h"
#include "memory.h"
#include <string.h>

#define STRING_MIN_SIZE 10

String
str_create(const char* text) {
	
	sizet len = strlen(text);

	String out;
	if (len < STRING_MIN_SIZE) {
		out.data = (char*)malloc(sizeof(char) * STRING_MIN_SIZE);
		out.capacity = STRING_MIN_SIZE;
	}
	else {
		out.data = (char*)malloc(sizeof(char) * len);
		out.capacity = len;
	}
	out.length = len;
	out.refCount = (i16*)malloc(sizeof(i16));
	(*out.refCount) = 1;
	memcpy(out.data, text, len * sizeof(char));
	ASSERT_MSG(out.data, "string malloc failed");

	return out;

}

String
str_create(sizet size) {
	
	String out;
	out.capacity = size;
	if (size < STRING_MIN_SIZE) {
		out.data = (char*)malloc(sizeof(char) * STRING_MIN_SIZE);
		out.capacity = STRING_MIN_SIZE;
	}
	else {
		out.data = (char*)malloc(sizeof(char) * size);
		out.capacity = size;
	}
	out.length = 0;
	out.data = (char*)malloc(sizeof(char) * size);
	out.refCount = (i16*)malloc(sizeof(i16));
	(*out.refCount) = 1;

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

b8
String::operator==(const char* str) {

	sizet len = strlen(str);
	if (len == length) {

		for (sizet i = 0; i < length; ++i) {
			if (str[i] != data[i])
				return 0;
		}
		return 1;
	}

	return 0;
}

String&
String::operator=(const String& other) {

	if (this != &other) {
		
		if (refCount && (*refCount)-- == 0) {

			free(refCount);
			free(data);
		}

		data = other.data;
		refCount = other.refCount;
		capacity = other.capacity;
		length = other.length;

		(*refCount) += 1;
	}

	return *this;
}


String::~String() {

	if (refCount) {
		(*refCount)--;
		if ((*refCount) == 0) {

			free(refCount);
			free(data);

			refCount = NULL;
			data = NULL;
		}
	}
}

String::String(const String& other) {

	data = other.data;
	refCount = other.refCount;
	capacity = other.capacity;
	length = other.length;

	(*refCount) += 1;

}

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
		str->data = (char*)realloc(str->data, sizeof(char) * str->capacity);
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
	for (; i < len; ++i) {

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

