#include "my_string.h"
#include "debug.h"
#include <string.h>

#define STR_LEN_ADD_ONE(str) str_field_set(str, S_LENGTH, STR_LENGTH(str) + 1);
#define STR_LEN_SUBTRACT_ONE(str) str_field_set(str, S_LENGTH, STR_LENGTH(str) - 1);

void*
str_create_c(const char* text) {
	
	sizet strLen = cstr_len(text);
	sizet headerSize = S_FIELDS * sizeof(sizet);
	sizet strSize = strLen * sizeof(char);

	sizet* str = (sizet*)malloc(strSize + headerSize);
	ASSERT_MSG(str, "string malloc failed");

	string out = (void*)(str + S_FIELDS);
	memcpy(out, text, strLen * sizeof(char));

	str[S_LENGTH] = strLen;
	str[S_CAPACITY] = strLen;

	return out;

}

void*
str_create_s(sizet size) {
	
	sizet headerSize = S_FIELDS * sizeof(sizet);
	sizet strSize = size * sizeof(char);

	sizet* str = (sizet*)malloc(strSize + headerSize);
	ASSERT_MSG(str, "string malloc failed");

	str[S_CAPACITY] = size;
	str[S_LENGTH] = 0;

	return (void*)(str + S_FIELDS);
}

sizet
str_field_get(void* str, sizet field) {
	
	return ((sizet*)(str) - S_FIELDS)[field];

}

void
str_field_set(void* str, sizet field, sizet value) {

	((sizet*)(str) - S_FIELDS)[field] = value;

}

void
str_release(void* str) {
  
	ASSERT(str);
	free(str - S_FIELDS * sizeof(sizet));
}



sizet
cstr_len(const char* str) {
	
	sizet len = 0;
	while(str[len] != '\0') {
		len++;
	}
	return len;

}

void
str_copy(string dest, string src) {

	ASSERT(STR_CAPACITY(dest) >= STR_LENGTH(src));

	sizet i = 0;
	for (i; i < STR_LENGTH(src); ++i) {
		
		dest[i] = src[i];
	}
	str_field_set(dest, S_LENGTH, STR_LENGTH(src));

}


void*
str_resize(void* str) {
	
	void *temp = str_create_s(STR_RESIZE_FACTOR * STR_CAPACITY(str));

	str_field_set(temp, S_LENGTH, STR_LENGTH(str));

	memcpy(temp, str, STR_LENGTH(str) * sizeof(char));
	str_release(str);

	return temp;

}

void*
str_push(void *str, char c) {

	if (STR_LENGTH(str) >= STR_CAPACITY(str)) {
		str = str_resize(str);
	}

	((string)str)[STR_LENGTH(str)] = c;
	STR_LEN_ADD_ONE(str);
    return str;
}

void
str_skip(void* str, sizet count) {
  
	str_field_set(str, S_LENGTH, STR_LENGTH(str) + count);
}

char* str_as_cstr(string str) {
	
	str[STR_LENGTH(str) - 1] = '\0';
	return str;

}


void*
str_concat(string s1, string s2) {
	
	if ((STR_LENGTH(s1) + STR_LENGTH(s2)) >= STR_CAPACITY(s1)) {

		s1 = str_resize(s1);
	}

	for (sizet i = 0; i < STR_LENGTH(s2); ++i) {
		
		s1[STR_LENGTH(s1) - 1] = s2[i];
		STR_LEN_ADD_ONE(s1);
		
	}
	return s1;

}


void
str_pop(string str) {

	if (STR_LENGTH(str) == 0) return;

	str[STR_LENGTH(str)] = '\0';
	STR_LEN_SUBTRACT_ONE(str);
	
}


i8
cstr_equal(const char* s1, const char* s2) {
	
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
