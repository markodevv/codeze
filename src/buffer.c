#include "buffer.h"

#define TAB_SIZE 4

Buffer*
buffer_create(File* file) {
  
	Buffer* buf = malloc(sizeof(Buffer));

	buf->preLen = 0;
	buf->gapLen = 0;
	buf->cursorX = 0;
	buf->postLen = STR_LENGTH(file->buffer);
	buf->lineLengths = array_create(file->lineCount, sizeof(sizet));

	buf->text = str_create_s(STR_LENGTH(file->buffer));
	str_copy(buf->text, file->buffer);

	sizet i = 0;
	// foreach line
	sizet zero = 0;
	for (sizet line = 0; line < file->lineCount; ++line) {
		
		buf->lineLengths = array_push(buf->lineLengths, &zero);
		// foreach char in line
		for (i; buf->text, buf->text[i] != '\n'; ++i) {

			// add to line length
			if (buf->text[i] == '\t') 
				buf->lineLengths[line] += TAB_SIZE;
			else
				buf->lineLengths[line]++;
		}
		i++;
		buf->lineLengths[line]++;

	}

	return buf;
}

inline static void
buffer_forward(Buffer* b) {
	
	b->text[b->preLen] = b->text[b->preLen + b->gapLen];
	b->preLen++;
	b->postLen--;

}

inline static void
buffer_backward(Buffer* b) {
	
	b->text[b->preLen + b->gapLen - 1] = b->text[b->preLen - 1];
	b->preLen--;
	b->postLen++;

}

void
buffer_cursor_next(Buffer* b) {
  
		// TODO: dunno if this is right
	if (b->text[b->preLen + b->gapLen] == '\n' ||
		b->postLen <= 0) {
		return;
	}

	if (b->text[b->preLen + b->gapLen] == '\t')
		b->cursorX += TAB_SIZE;
	else 
		b->cursorX++;

	buffer_forward(b);

}

void
buffer_cursor_previous(Buffer* b) {

	if (b->preLen == 0 || b->text[b->preLen - 1] == '\n') return;

	if (b->text[b->preLen - 1] == '\t')
		b->cursorX -= TAB_SIZE;
	else 
		b->cursorX--;

	buffer_backward(b);

}


void
buffer_cursor_down(Buffer* b) {
	
	if (b->currentLine == ARRAY_LENGTH(b->lineLengths) - 1) return;


	while (b->text[b->preLen + b->gapLen] != '\n') {
		buffer_forward(b);
	}
	buffer_forward(b);
	b->currentLine++;

	u32 currLineLen = b->lineLengths[b->currentLine] - 1;

	if (currLineLen >= b->cursorX) {

		sizet prevX = b->cursorX;
		while (prevX > 0) {

			if (b->text[b->preLen + b->gapLen] == '\t') {
				buffer_forward(b);
				prevX -= TAB_SIZE;
			}
			else {
				buffer_forward(b);
				prevX--;
			}
				
		}
	}
	else {

		b->cursorX = 0;
		while (b->text[b->preLen + b->gapLen] != '\n') {
			
			if (b->text[b->preLen + b->gapLen] == '\t') {
				b->cursorX += TAB_SIZE;
			}
			else {
				b->cursorX++;
			}
			buffer_forward(b);
		}
	}

}

void
buffer_cursor_up(Buffer* b) {
	
	if (b->currentLine == 0) return;

	while (b->text[b->preLen - 1] != '\n') {

		buffer_backward(b);
	}
	buffer_backward(b);
	b->currentLine--;

	i32 backwardSteps = (b->lineLengths[b->currentLine] - 1) - b->cursorX;

	if (backwardSteps > 0) {

		while (backwardSteps > 0) {

			buffer_backward(b);
			if (b->text[b->preLen] == '\t')
				backwardSteps -= TAB_SIZE;
			else
				backwardSteps--;
		}
	}
	else {
		
		b->cursorX = b->lineLengths[b->currentLine] - 1;
	}

}

buffer_insert_char(Buffer* b, char c) {
	
	if (b->gapLen == 0) {
		
		
	}
}
