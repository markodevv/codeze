#include "buffer.h"

Buffer*
buffer_create(File* file) {
  
	Buffer* buf = malloc(sizeof(Buffer));

	buf->preLen = 0;
	buf->gapLen = 0;
	buf->postLen = STR_LENGTH(file->buffer);
	buf->lineCount = file->lineCount;
	buf->lineData = array_create(buf->lineCount, sizeof(sizet));

	buf->text = str_create_s(STR_LENGTH(file->buffer));
	str_copy(buf->text, file->buffer);

	sizet i = 0;
	// foreach line
	sizet temp = 0;
	for (sizet line = 0; line < buf->lineCount; ++line) {
		
		buf->lineData = array_push(buf->lineData, &temp);
		// foreach char in line
		for (i; buf->text, buf->text[i] != '\n'; ++i) {

			// add to line length
			buf->lineData[line]++;
		}
		i++;
		buf->lineData[line]++;

	}

	return buf;
}

void
buffer_cursor_forward(Buffer* b) {
	

}

void
buffer_cursor_backward(Buffer* b) {
	
	b->text[b->preLen + b->gapLen - 1] = b->text[b->preLen - 1];
	b->preLen--;
	b->postLen++;

}
