#pragma once
#include "types.h"
#include "my_string.h"
#include "math.h"
#include "fileio.h"
#include "container.h"


typedef struct Buffer {
  
	String text;

	sizet preLen;
	sizet postLen;
	sizet gapLen;

	i32 currentLine;
	Array<i32> cursorLines;
	Array<i32> lineLengths;

	i32 curX;
	i32 cursorXtabed;

} Buffer;

extern Buffer* CurBuffer;
extern Buffer* CommandBuffer;

void buffer_forward();
void buffer_backward();
Buffer buffer_create(File* file);
Buffer buffer_create_empthy();
void buffer_insert_char(char c);
String buffer_get_text(Buffer* buf);
void buffer_insert_tab();
void buffer_insert_newline();
void buffer_backspace_delete();
String buffer_string_before_cursor();
char buffer_char_under_cursor();
char buffer_char_before_cursor();
sizet buffer_index_based_on_line(Buffer* buf, i32 line);
void buffer_clear(Buffer* buf);
