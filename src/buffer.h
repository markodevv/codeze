#pragma once
#include "types.h"
#include "my_string.h"
#include "math.h"
#include "fileio.h"
#include "container.h"

typedef struct Buffer {
  
	string text;

	sizet preLen;
	sizet postLen;
	sizet gapLen;

	i32 currentLine;
	i32* lineLengths;

	i32 cursorX;

} Buffer;

Buffer* buffer_create(File* file);
void buffer_cursor_down(Buffer* b);
void buffer_cursor_up(Buffer* b);
void buffer_cursor_previous(Buffer* b);
void buffer_cursor_next(Buffer* b);
void buffer_insert_char(Buffer* b, char c);
string buffer_get_text(Buffer* b);
void buffer_insert_tab(Buffer* b);
void buffer_insert_newline(Buffer* b);
void buffer_backspace_delete(Buffer* b);
Vec2 buffer_get_cursor_line_vec(Buffer* b);
string buffer_string_before_cursor(Buffer* b);
char buffer_char_under_cursor(Buffer* b);
char buffer_char_before_cursor(Buffer* b);
