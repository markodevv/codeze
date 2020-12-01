#include "buffer.h"
#include "debug.h"
#include "math.h"
#include <string.h>

#define TAB_SIZE 4
#define BUFFER_RESIZE_FACTOR 2

Buffer*
buffer_create(File* file) {
  
	Buffer* buf = malloc(sizeof(Buffer));

	buf->preLen = 0;
	buf->gapLen = 0;
	buf->cursorX = 0;
	buf->postLen = STR_LENGTH(file->buffer);
	buf->lineLengths = array_create(file->lineCount, sizeof(i32));

	buf->text = str_create_s(STR_LENGTH(file->buffer));
	str_copy(buf->text, file->buffer);

	sizet i = 0;
	// foreach line
	sizet zero = 0;
	for (sizet line = 0; line < file->lineCount; ++line) {
		
		buf->lineLengths = array_push(buf->lineLengths, &zero);
		// foreach char in line
		for (i; buf->text[i] != '\n'; ++i) {
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

string
buffer_get_text(Buffer* b) {
	
	string newstr = str_create_s(b->preLen + b->postLen);

	for (sizet i = 0; i < b->preLen; ++i) {
		newstr = str_push(newstr, b->text[i]);
	}

	for (sizet i = b->preLen; i + b->gapLen < STR_LENGTH(b->text); ++i) {
		newstr = str_push(newstr, b->text[i + b->gapLen]);
	}

	return newstr;

}

void
buffer_insert_char(Buffer* b, char c) {
	
	if (b->gapLen == 0) {
		
		sizet gap = STR_LENGTH(b->text);
		string newstr = str_create_s(gap * BUFFER_RESIZE_FACTOR);

		// copy first part of string
		sizet i = 0;
		for (i; i < b->preLen; ++i) {
			newstr = str_push(newstr, b->text[i]);
		}

		for (i; i < gap; ++i) {
			newstr = str_push(newstr, 'a');
		}

		//copy second part of the string
		for (i = b->preLen; i < STR_LENGTH(b->text); ++i) {
			newstr = str_push(newstr, b->text[i]);
		}

		str_release(b->text);
		b->text = newstr;
	}

	b->text[b->preLen] = c;
	b->preLen++;
	b->lineLengths[b->currentLine]++;
	b->cursorX++;

	b->gapLen = STR_LENGTH(b->text) - (b->preLen + b->postLen);
}

void
buffer_insert_tab(Buffer* b) {
	
	b->lineLengths[b->currentLine] += TAB_SIZE - 1;
	b->cursorX += TAB_SIZE - 1;
	buffer_insert_char(b, '\t');
}

void
buffer_insert_newline(Buffer* b) {
	
	buffer_insert_char(b, '\n');
	u32 one = 0;
	b->lineLengths = array_insert(b->lineLengths, b->currentLine, &one);

	if (b->lineLengths[b->currentLine] != b->cursorX) {

		u32 splitLineLen = b->lineLengths[b->currentLine] - b->cursorX;
		b->lineLengths[b->currentLine] -= splitLineLen;
		b->lineLengths[b->currentLine + 1] += splitLineLen;
	}
	b->currentLine++;
	b->cursorX = 0;
}

void
buffer_backspace_delete(Buffer* b) {
	
	if (b->preLen == 0) return;
	b->preLen--;
	b->gapLen++;

	if (b->text[b->preLen] == '\n') {

		u32 deletedLine = b->lineLengths[b->currentLine];

		array_erase(b->lineLengths, b->currentLine);
		b->currentLine--;
		b->cursorX = b->lineLengths[b->currentLine];
		b->lineLengths[b->currentLine] += deletedLine;


	}
	else if (b->text[b->preLen] == '\t') {
		b->lineLengths[b->currentLine] -= TAB_SIZE - 1;
		b->cursorX -= TAB_SIZE - 1;
	}

	b->cursorX--;
	b->lineLengths[b->currentLine]--;

}

Vec2
buffer_get_cursor_line_vec(Buffer* b) {

	Vec2 out;
  
	sizet i = b->preLen;
	if (b->currentLine != 0) {
		
		while(b->text[i - 1] != '\n') {
			i--;
		}
		out.start = i;
	}
	else {
		out.start = 0;
	}

	i = b->preLen;
	while(b->text[i + b->gapLen] != '\n') {
		i++;
	}

	out.end = i;

	return out;
}

string
buffer_string_before_cursor(Buffer* b) {
	
	string line = str_create_s(b->lineLengths[b->currentLine]);

	i32 i = b->preLen - 1;
	if (b->currentLine == 0) {
		while (i >= 0) {
			line = str_push(line, b->text[i]);
			i--;
		}
	}
	else {
		while (b->text[i] != '\n')  {
			line = str_push(line, b->text[i]);
			i--;
		}
		//	line = str_push(line, b->text[i]);
		//line = str_push(line, b->text[i]);
	}

	printf("string before cursor:[");
	for (sizet j = STR_LENGTH(line); j > 0; --j) {
		
		printf("%c", line[j - 1]);
	}
	printf("]\n");

	return line;
}

char
buffer_char_under_cursor(Buffer* b) {
	
	return b->text[b->preLen + b->gapLen];
}

char
buffer_char_before_cursor(Buffer* b) {
	
	return b->text[b->preLen];
}
