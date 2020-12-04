#include "buffer.h"
#include "debug.h"
#include "math.h"
#include "config.h"
#include <string.h>

#define BUFFER_RESIZE_FACTOR 2

Buffer*
buffer_create(File* file) {
  
	Buffer* buf = malloc(sizeof(Buffer));

	buf->preLen = 0;
	buf->gapLen = 0;
	buf->cursorXtabed = 0;
	buf->curX = 0;
	buf->currentLine = 0;
	buf->postLen = STR_LENGTH(file->buffer);
	buf->cursorLines = array_create(file->lineCount, sizeof(i32));
	buf->lineLengths = array_create(file->lineCount, sizeof(i32));

	buf->text = str_create_s(STR_LENGTH(file->buffer));
	str_copy(buf->text, file->buffer);

	sizet i = 0;
	// foreach line
	sizet zero = 0;
	for (sizet line = 0; line < file->lineCount; ++line) {
		
		buf->lineLengths = array_push(buf->lineLengths, &zero);
		buf->cursorLines = array_push(buf->cursorLines, &zero);
		// foreach char in line
		for (i; buf->text[i] != '\n'; ++i) {
			// add to line length
			if (buf->text[i] == '\t') 
				buf->cursorLines[line] += TAB_SIZE;
			else
				buf->cursorLines[line]++;

			buf->lineLengths[line]++;
		}
		i++;
		buf->cursorLines[line]++;
		buf->lineLengths[line]++;

	}

	return buf;
}

void
buffer_forward(Buffer* b) {
	
	b->text[b->preLen] = b->text[b->preLen + b->gapLen];
	b->preLen++;
	b->postLen--;

}

void
buffer_backward(Buffer* b) {
	
	b->text[b->preLen + b->gapLen - 1] = b->text[b->preLen - 1];
	b->preLen--;
	b->postLen++;

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
	b->cursorLines[b->currentLine]++;
	b->lineLengths[b->currentLine]++;

	b->cursorXtabed++;
	b->curX++;

	b->gapLen = STR_LENGTH(b->text) - (b->preLen + b->postLen);
}

void
buffer_insert_tab(Buffer* b) {
	
	b->cursorLines[b->currentLine] += TAB_SIZE - 1;
	b->cursorXtabed += TAB_SIZE - 1;
	buffer_insert_char(b, '\t');
}


void
buffer_insert_newline(Buffer* b) {
	
	buffer_insert_char(b, '\n');
	u32 zero = 0;
	b->cursorLines = array_insert(b->cursorLines, b->currentLine, &zero);
	b->lineLengths = array_insert(b->lineLengths, b->currentLine, &zero);

	if (b->cursorLines[b->currentLine] != b->cursorXtabed) {

		u32 splitLineLen = b->cursorLines[b->currentLine] - b->cursorXtabed;
		b->cursorLines[b->currentLine] -= splitLineLen;
		b->cursorLines[b->currentLine + 1] += splitLineLen;

		splitLineLen = b->lineLengths[b->currentLine] - b->curX;
		b->lineLengths[b->currentLine] -= splitLineLen;
		b->lineLengths[b->currentLine + 1] += splitLineLen;
	}
	b->currentLine++;
	b->cursorXtabed = 0;
	b->curX = 0;
}

void
buffer_backspace_delete(Buffer* b) {
	
	if (b->preLen == 0) return;
	b->preLen--;
	b->gapLen++;

	if (b->text[b->preLen] == '\n') {

		i32 delCurosrLine = b->cursorLines[b->currentLine];
		i32 delLine = b->lineLengths[b->currentLine];

		array_erase(b->cursorLines, b->currentLine);
		array_erase(b->lineLengths, b->currentLine);

		b->currentLine--;

		b->cursorXtabed = b->cursorLines[b->currentLine];
		b->curX = b->lineLengths[b->currentLine];
		b->cursorLines[b->currentLine] += delCurosrLine;
		b->lineLengths[b->currentLine] += delLine;


	}
	else if (b->text[b->preLen] == '\t') {
		b->cursorLines[b->currentLine] -= TAB_SIZE - 1;
		b->cursorXtabed -= TAB_SIZE - 1;
	}

	b->cursorXtabed--;
	b->curX--;
	b->cursorLines[b->currentLine]--;
	b->lineLengths[b->currentLine]--;

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
	}

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

sizet
buffer_index_based_on_line(Buffer* b, i32 line) {

	sizet result = 0;
	for (sizet i = 0; i < line; ++i) {
		
		result += b->lineLengths[i];
	}

	return result;
}
