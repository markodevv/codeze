#include "buffer.h"
#include "debug.h"
#include "math.h"
#include "config.h"
#include "globals.h"

#include <string.h>

#define BUFFER_RESIZE_FACTOR 2
#define BUFFER_EMPTHY_SIZE 20

static List<Buffer> Buffers;
static HashTable<Buffer*> BufferTable;

void
buffers_init() {
	
	list_init(&Buffers);
	hash_table_init(&BufferTable);
}

void
buffer_add(const char* path) {

	File file = file_open(path, "r");
	String key = get_filestr_from_path(path);
	str_push(&key, '\0');

	list_add(&Buffers, buffer_create(file));
	Buffers.tail->data.name = str_create(key.data);

	hash_table_put(&BufferTable, key.data, &Buffers.tail->data);
}

void
buffer_add_empthy() {

	static i32 index = 0;
	static i8 numberIndex = 8;
	static char bufferName[] = "<empthy 0>";

	bufferName[numberIndex] = '0' + index;

	list_add(&Buffers, buffer_create_empthy());
	Buffers.tail->data.name = str_create(bufferName);
	hash_table_put(&BufferTable, bufferName, &Buffers.tail->data);

	index++;
}

void
buffer_add_empthy(const char* bufferName) {

	list_add(&Buffers, buffer_create_empthy());
	Buffers.tail->data.name = str_create(bufferName);
	hash_table_put(&BufferTable, bufferName, &Buffers.tail->data);
}

i32
buffer_hash_index_from_key(const char* key) {

	return hash_table_index_from_key(&BufferTable, key);
}

Buffer*
buffer_get(const char* key) {

	return hash_table_get(&BufferTable, key);
}

Buffer*
buffer_get(i32 index) {

	Buffer* out = BufferTable[index];
	ASSERT_MSG(out, "invalid index for hash table");
	return out;
}


Buffer
buffer_create(File& file) {
  
	Buffer buf;

	buf.preLen = 0;
	buf.gapLen = 0;
	buf.cursorXtabed = 0;
	buf.curX = 0;
	buf.currentLine = 0;
	buf.postLen = file.buffer.length;
	array_init(&buf.lineLengths, file.lineCount);
	array_init(&buf.cursorLines, file.lineCount);

	buf.capacity = file.buffer.length;
	buf.text = (char*)malloc(sizeof(char) * buf.capacity);

	for (sizet i = 0; i < file.buffer.length; ++i) {

		buf.text[i] = file.buffer[i];
	}

	sizet i = 0;
	// foreach line
	for (sizet line = 0; line < file.lineCount; ++line) {
		
		array_push(&buf.lineLengths, 0);
		array_push(&buf.cursorLines, 0);
		// foreach char in line
		for (; buf.text[i] != '\n'; ++i) {
			// add to line length
			if (buf.text[i] == '\t') 
				buf.cursorLines[line] += TAB_SIZE;
			else
				buf.cursorLines[line] += 1;

			buf.lineLengths[line] += 1;
		}
		i++;
		buf.cursorLines[line] += 1;
		buf.lineLengths[line] += 1;

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
	array_init(&buf.cursorLines, 1);
	array_init(&buf.lineLengths, 1);


	array_push(&buf.lineLengths, 0);
	array_push(&buf.cursorLines, 0);

	//buf.text = str_create(BUFFER_EMPTHY_SIZE);
	buf.capacity = BUFFER_EMPTHY_SIZE;
	buf.text = (char*)malloc(sizeof(char) * BUFFER_EMPTHY_SIZE);

	//str_skip(buf.text, BUFFER_EMPTHY_SIZE);

	return buf;

}

void
buffer_forward() {
	
	CurBuffer->text[CurBuffer->preLen]
		= CurBuffer->text[CurBuffer->preLen + CurBuffer->gapLen];
	CurBuffer->preLen++;
	CurBuffer->postLen--;

}

void
buffer_backward() {
	
	CurBuffer->text[CurBuffer->preLen + CurBuffer->gapLen - 1] = CurBuffer->text[CurBuffer->preLen - 1];
	CurBuffer->preLen--;
	CurBuffer->postLen++;

}

String
buffer_get_text_copy(Buffer* buf) {
	
	String out = str_create(buf->preLen + buf->postLen);

	for (sizet i = 0; i < buf->preLen; ++i) {
		str_push(&out, buf->text[i]);
	}

	for (sizet i = buf->preLen; i + buf->gapLen < buf->capacity; ++i) {
		str_push(&out, buf->text[i + buf->gapLen]);
	}

	return out;

}


void
buffer_insert_char(char c) {
	
	if (CurBuffer->gapLen == 0) {
		
		sizet gap = CurBuffer->preLen + CurBuffer->postLen;
		CurBuffer->capacity += gap;
		char* newbuf = (char*)malloc(sizeof(char) * CurBuffer->capacity);

		// copy first part of string
		for (sizet i = 0; i < CurBuffer->preLen; ++i) {
			newbuf[i] = CurBuffer->text[i];
		}

#ifdef DEBUG
		for (sizet i = CurBuffer->preLen; i < CurBuffer->preLen + gap; ++i) {
			newbuf[i] = '%';
		}
#endif

		//copy second part of the string
		for (sizet i = CurBuffer->preLen; i < gap; ++i) {
			newbuf[i + gap] = CurBuffer->text[i];
		}

		free(CurBuffer->text);
		CurBuffer->text = newbuf;
		CurBuffer->gapLen = gap;
	}

	CurBuffer->text[CurBuffer->preLen] = c;
	CurBuffer->preLen++;
	CurBuffer->cursorLines[CurBuffer->currentLine]++;
	CurBuffer->lineLengths[CurBuffer->currentLine]++;

	CurBuffer->cursorXtabed++;
	CurBuffer->curX++;
	CurBuffer->gapLen--;
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
	array_insert(&CurBuffer->cursorLines, 0, CurBuffer->currentLine);
	array_insert(&CurBuffer->lineLengths, 0, CurBuffer->currentLine);

	if (CurBuffer->cursorLines[CurBuffer->currentLine] != CurBuffer->cursorXtabed) {

		i32 splitLineLen = CurBuffer->cursorLines[CurBuffer->currentLine] - CurBuffer->cursorXtabed;
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

sizet
buffer_length(Buffer* buf) {

	return buf->preLen + buf->postLen;
}

void
buffer_backspace_delete() {
	
	if (CurBuffer->preLen == 0) return;

#ifdef DEBUG
	//	CurBuffer->text[CurBuffer->preLen] = '%';
#endif

	CurBuffer->preLen--;
	CurBuffer->gapLen++;

	if (CurBuffer->text[CurBuffer->preLen] == '\n') {

		i32 delCurosrLine = CurBuffer->cursorLines[CurBuffer->currentLine];
		i32 delLine = CurBuffer->lineLengths[CurBuffer->currentLine];

		array_erase(&CurBuffer->cursorLines, CurBuffer->currentLine);
		array_erase(&CurBuffer->lineLengths, CurBuffer->currentLine);

		CurBuffer->currentLine--;

		CurBuffer->cursorXtabed = CurBuffer->cursorLines[CurBuffer->currentLine];
		CurBuffer->curX = CurBuffer->lineLengths[CurBuffer->currentLine];
		CurBuffer->cursorLines[CurBuffer->currentLine] += delCurosrLine;
		CurBuffer->lineLengths[CurBuffer->currentLine] += delLine;


	}
	else if (CurBuffer->text[CurBuffer->preLen] == '\t') {
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
	
	String line = str_create(CurBuffer->lineLengths[CurBuffer->currentLine]);

	i32 i = CurBuffer->preLen - 1;
	if (CurBuffer->currentLine == 0) {
		while (i >= 0) {
			str_push(&line, CurBuffer->text[i]);
			i--;
		}
	}
	else {
		while (CurBuffer->text[i] != '\n')  {
			str_push(&line, CurBuffer->text[i]);
			i--;
		}
	}

	return line;
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

	array_reset(&buf->lineLengths);
	array_reset(&buf->cursorLines);
	array_push(&buf->lineLengths, 0);
	array_push(&buf->cursorLines, 0);

	buf->gapLen = buf->capacity;
	buf->preLen = 0;
	buf->postLen = 0;
	buf->currentLine = 0;
	buf->curX = 0;
	buf->cursorXtabed = 0;
}
