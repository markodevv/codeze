#pragma once
#include "types.h"
#include "my_string.h"
#include "math.h"
#include "fileio.h"
#include "container.h"

#define TAB_SIZE 4

typedef struct Buffer {
  
	char* text;
	String name;

	sizet preLen;
	sizet postLen;
	sizet gapLen;
	sizet capacity;

	i32 currentLine;
	Array<i32> cursorLines;
	Array<i32> lineLengths;

	i32 curX;
	i32 cursorXtabed;

	String path;

} Buffer;


void buffers_init();
void buffer_add_empthy();
Buffer* buffer_add(File& file);
Buffer* buffer_get(const char* key);
Buffer buffer_create_empthy();

void buffer_forward();
void buffer_backward();
Buffer buffer_create(File& file);
Buffer buffer_create_empthy();
void buffer_insert_char(char c);
String buffer_get_text_copy(Buffer* buf);
void buffer_insert_tab();
void buffer_insert_newline();
void buffer_backspace_delete();
String buffer_string_before_cursor();
sizet buffer_index_based_on_line(Buffer* buf, i32 line);
void buffer_clear(Buffer* buf);
sizet buffer_length(Buffer* buf);
