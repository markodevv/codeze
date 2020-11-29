#pragma once
#include "types.h"
#include "my_string.h"
#include "fileio.h"
#include "container.h"

typedef struct Buffer {
  
	string* text;

	sizet preLen;
	sizet postLen;
	sizet gapLen;

	u32 currentLine;
	u32* lineLengths;

	u32 cursorX;

} Buffer;

Buffer* buffer_create(File* file);
void buffer_cursor_down(Buffer* b);
void buffer_cursor_up(Buffer* b);
void buffer_cursor_previous(Buffer* b);
void buffer_cursor_next(Buffer* b);


