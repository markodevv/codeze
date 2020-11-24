#include "string.h"
#include "debug.h"


String*
str_create_char(const char* text) {
	
	String* str = malloc(sizeof(String));
	str->length = cstr_len(text);
	str->capacity = str->length;
	str->data = malloc(sizeof(char) * str->length);
	str_copy(str->data, text);

	return str;

}

String*
str_create_size(sizet size) {
	
	String* str = malloc(sizeof(String));
	str->length = 0;
	str->capacity = size;
	str->data = malloc(sizeof(char) * size);

	return str;

}

void
str_delete(String* str) {

	ASSERT(str != NULL);

	free(str->data);
	free(str);
	
}

sizet
str_len(const String* str) {

	sizet len = 0;
	while(str->data[len] != '\0') {
		len++;
	}
	return len;
  
}

sizet
cstr_len(const char* str) {
	
	sizet len = 0;
	while(str[len] != '\0') {
		len++;
	}
	return len;

}

char
str_at(String* str, sizet index) {

	return str->data[index];
	
}

void
str_copy(char* src, const char* dest) {

	sizet i = 0;
	for (i; dest[i] != '\0'; ++i) {
		
		src[i] = dest[i];

	}
	src[i] = '\0';

}


void
str_concat(String* s1, const char* s2) {
	
	sizet len = cstr_len(s2);

	if ((s1->length + len) >= s1->capacity) {

		s1->capacity = s1->capacity + (len * 2);
		s1->data = realloc(s1->data, s1->capacity);
	}

	for (sizet i = 0; i < len; ++i) {
		
		s1->data[s1->length] = s2[i];
		s1->length++;
	}
	s1->data[s1->length] = '\0';
		

}

void
str_delete_from_back(String* str, sizet count) {

	str->data[str->length - count] = '\0';
	str->length -= count;
	
}

void
str_print(const String* str) {
	
	for (sizet i = 0; str->data[i] != '\0'; ++i) {

		printf("%c", str->data[i]);
	}
	printf("\n");

}
