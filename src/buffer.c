#include "buffer.h"
#include "debug.h"
#include "math.h"
#include "config.h"
#include <string.h>

#define BUFFER_RESIZE_FACTOR 2
#define BUFFER_EMPTHY_SIZE 20

Buffer* CurBuffer;
Buffer* CommandBuffer;

Buffer
buffer_create(File* file) {
  
	Buffer buf;

	buf.preLen = 0;
	buf.gapLen = 0;
	buf.cursorXtabed = 0;
	buf.curX = 0;
	buf.currentLine = 0;
	buf.postLen = file->buffer.length;
	buf.cursorLines = array_create(file->lineCount, sizeof(i32));
	buf.lineLengths = array_create(file->lineCount, sizeof(i32));

	buf.text = str_create_s(file->buffer.length);
	str_copy(&buf.text, &file->buffer);

	sizet i = 0;
	// foreach line
	sizet zero = 0;
	for (sizet line = 0; line < file->lineCount; ++line) {
		
		buf.lineLengths = array_push(buf.lineLengths, &zero);
		buf.cursorLines = array_push(buf.cursorLines, &zero);
		// foreach char in line
		for (i; buf.text.data[i] != '\n'; ++i) {
			// add to line length
			if (buf.text.data[i] == '\t') 
				buf.cursorLines[line] += TAB_SIZE;
			else
				buf.cursorLines[line]++;

			buf.lineLengths[line]++;
		}
		i++;
		buf.cursorLines[line]++;
		buf.lineLengths[line]++;

	}

	return buf;
}

Buffer
buffer_create_empthy() {
	
	Buffer buf;

	buf.preLen = 0;
	buf.gapLen = BUFFER_EMPTHY_SIZE;
	buf.cursorXtabed = 0;
	buf.curX = 0;
	buf.currentLine = 0;
	buf.postLen = 0;
	buf.cursorLines = array_create(1, sizeof(i32));
	buf.lineLengths = array_create(1, sizeof(i32));


	sizet zero = 0;
	buf.lineLengths = array_push(buf.lineLengths, &zero);
	buf.cursorLines = array_push(buf.cursorLines, &zero);

	buf.text = str_create_s(BUFFER_EMPTHY_SIZE);

	//str_skip(buf.text, BUFFER_EMPTHY_SIZE);

	return buf;

}

void
buffer_forward() {
	
	CurBuffer->text.data[CurBuffer->preLen]
		= CurBuffer->text.data[CurBuffer->preLen + CurBuffer->gapLen];
	CurBuffer->preLen++;
	CurBuffer->postLen--;

}

void
buffer_backward() {
	
	CurBuffer->text.data[CurBuffer->preLen + CurBuffer->gapLen - 1] = CurBuffer->text.data[CurBuffer->preLen - 1];
	CurBuffer->preLen--;
	CurBuffer->postLen++;

}

String
buffer_get_text(Buffer* buf) {
	
	String newstr = str_create_s(buf->preLen + buf->postLen);

	for (sizet i = 0; i < buf->preLen; ++i) {
		str_push(&newstr, buf->text.data[i]);
	}

	for (sizet i = buf->preLen; i + buf->gapLen < buf->text.capacity; ++i) {
		str_push(&newstr, buf->text.data[i + buf->gapLen]);
	}

	return newstr;

}

void
buffer_insert_char(char c) {
	
	if (CurBuffer->gapLen == 0) {
		
		sizet gap = CurBuffer->text.capacity;
		String newstr = str_create_s(gap * BUFFER_RESIZE_FACTOR);

		// copy first part of string
		sizet i = 0;
		for (i; i < CurBuffer->preLen; ++i) {
			str_push(&newstr, CurBuffer->text.data[i]);
		}

		str_skip(&newstr, gap);
		//copy second part of the string
		for (i = CurBuffer->preLen; i < CurBuffer->text.capacity; ++i) {
			str_push(&newstr, CurBuffer->text.data[i]);
		}

		str_free(&CurBuffer->text);
		CurBuffer->text = newstr;
	}

	CurBuffer->text.data[CurBuffer->preLen] = c;
	CurBuffer->preLen++;
	CurBuffer->cursorLines[CurBuffer->currentLine]++;
	CurBuffer->lineLengths[CurBuffer->currentLine]++;

	CurBuffer->cursorXtabed++;
	CurBuffer->curX++;

	CurBuffer->gapLen = CurBuffer->text.capacity - (CurBuffer->preLen + CurBuffer->postLen);
}

void
buffer_insert_tab() {
	
	CurBuffer->cursorLines[CurBuffer->currentLine] += TAB_SIZE - 1;
	CurBuffer->cursorXtabed += TAB_SIZE - 1;
	buffer_insert_char('\t');
}


void
buffer_insert_newline() {
	
	buffer_insert_char('\n');
	u32 zero = 0;
	CurBuffer->cursorLines = array_insert(CurBuffer->cursorLines, CurBuffer->currentLine, &zero);
	CurBuffer->lineLengths = array_insert(CurBuffer->lineLengths, CurBuffer->currentLine, &zero);

	if (CurBuffer->cursorLines[CurBuffer->currentLine] != CurBuffer->cursorXtabed) {

		u32 splitLineLen = CurBuffer->cursorLines[CurBuffer->currentLine] - CurBuffer->cursorXtabed;
		CurBuffer->cursorLines[CurBuffer->currentLine] -= splitLineLen;
		CurBuffer->cursorLines[CurBuffer->currentLine + 1] += splitLineLen;

		splitLineLen = CurBuffer->lineLengths[CurBuffer->currentLine] - CurBuffer->curX;
		CurBuffer->lineLengths[CurBuffer->currentLine] -= splitLineLen;
		CurBuffer->lineLengths[CurBuffer->currentLine + 1] += splitLineLen;
	}
	CurBuffer->currentLine++;
	CurBuffer->cursorXtabed = 0;
	CurBuffer->curX = 0;
}

void
buffer_backspace_delete() {
	
	if (CurBuffer->preLen == 0) return;
	CurBuffer->preLen--;
	CurBuffer->gapLen++;

	if (CurBuffer->text.data[CurBuffer->preLen] == '\n') {

		i32 delCurosrLine = CurBuffer->cursorLines[CurBuffer->currentLine];
		i32 delLine = CurBuffer->lineLengths[CurBuffer->currentLine];

		array_erase(CurBuffer->cursorLines, CurBuffer->currentLine);
		array_erase(CurBuffer->lineLengths, CurBuffer->currentLine);

		CurBuffer->currentLine--;

		CurBuffer->cursorXtabed = CurBuffer->cursorLines[CurBuffer->currentLine];
		CurBuffer->curX = CurBuffer->lineLengths[CurBuffer->currentLine];
		CurBuffer->cursorLines[CurBuffer->currentLine] += delCurosrLine;
		CurBuffer->lineLengths[CurBuffer->currentLine] += delLine;


	}
	else if (CurBuffer->text.data[CurBuffer->preLen] == '\t') {
		CurBuffer->cursorLines[CurBuffer->currentLine] -= TAB_SIZE - 1;
		CurBuffer->cursorXtabed -= TAB_SIZE - 1;
	}

	CurBuffer->cursorXtabed--;
	CurBuffer->curX--;
	CurBuffer->cursorLines[CurBuffer->currentLine]--;
	CurBuffer->lineLengths[CurBuffer->currentLine]--;

}

String
buffer_string_before_cursor() {
	
	String line = str_create_s(CurBuffer->lineLengths[CurBuffer->currentLine]);

	i32 i = CurBuffer->preLen - 1;
	if (CurBuffer->currentLine == 0) {
		while (i >= 0) {
			str_push(&line, CurBuffer->text.data[i]);
			i--;
		}
	}
	else {
		while (CurBuffer->text.data[i] != '\n')  {
			str_push(&line, CurBuffer->text.data[i]);
			i--;
		}
	}

	return line;
}

char
buffer_char_under_cursor() {
	
	return CurBuffer->text.data[CurBuffer->preLen + CurBuffer->gapLen];
}

char
buffer_char_before_cursor() {
	
	return CurBuffer->text.data[CurBuffer->preLen];
}

sizet
buffer_index_based_on_line(Buffer* buf, i32 line) {

	sizet result = 0;
	for (sizet i = 0; i < line; ++i) {
		
		result += buf->lineLengths[i];
	}

	return result;
}

void
buffer_clear(Buffer* buf) {

	str_clear(&buf->text);
	array_reset(buf->lineLengths);
	array_reset(buf->cursorLines);
	i32 zero = 0;
	buf->lineLengths = array_push(buf->lineLengths, &zero);
	buf->cursorLines = array_push(buf->cursorLines, &zero);

	buf->preLen = 0;
	buf->postLen = 0;
	buf->gapLen = buf->text.capacity;
	buf->currentLine = 0;
	buf->curX = 0;
	buf->cursorXtabed = 0;
}
