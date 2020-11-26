#include "my_string.h"
#include "debug.h"


String*
str_create_c(const char* text) {
	
	String* str = malloc(sizeof(String));
	str->length = cstr_len(text);
	str->capacity = str->length;
	str->data = malloc(sizeof(char) * str->length);
	str_copy(str, text);

	return str;

}

String*
str_create_s(sizet size) {
	
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
str_copy(String* dest, const char* src) {

	ASSERT(dest->capacity >= cstr_len(src));

	sizet i = 0;
	for (i; src[i] != '\0'; ++i) {
		
		dest->data[i] = src[i];
	}
	dest->data[i] = '\0';

}


void
str_concat(String* s1, const char* s2) {
	
	sizet len = cstr_len(s2);

	if ((s1->length + len) >= s1->capacity) {

		s1->capacity = s1->capacity + (len * 2);
		s1->data = realloc(s1->data, s1->capacity * sizeof(char));
	}

	for (sizet i = 0; i < len; ++i) {
		
		s1->data[s1->length] = s2[i];
		s1->length++;
	}
	s1->data[s1->length] = '\0';
		

}

char*
cstr_concat(const char* s1, const char* s2) {
  
	sizet len1 = cstr_len(s1);
	sizet len2 = cstr_len(s2);
	char* out = malloc(len1 + len2);

	for (sizet i = 0; i < len1; ++i) {
		
		out[i] = s1[i];
	}

	for (sizet i = 0; i < len2; ++i) {
		
		out[i] = s2[i];
	}
	out[len1 + len2] = '\0';

	return out;
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

i8
str_equal(const char* s1, const char* s2) {
	
	sizet i = 0;

	sizet len = cstr_len(s1);
	for (i; i < len; ++i) {

		if (s1[i] != s2[i]) return 0;
	}

	if (s2[i] != '\0') {
		return 0;
	} else {
		return 1;
	}

}
