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

	sizet lineCount;
	sizet* lineData;

} Buffer;

Buffer* buffer_create(File* file);
void buffer_cursor_forward(Buffer* b);
void buffer_cursor_backward(Buffer* b);


