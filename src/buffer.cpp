#include "buffer.h"
#include "debug.h"
#include "math.h"
#include "config.h"
#include "globals.h"

#include <string.h>

#define BUFFER_RESIZE_FACTOR 2
#define BUFFER_EMPTHY_SIZE 20

static List<Buffer> Buffers;

void
buffers_init() {
	
	list_init(&Buffers);
}


static String
get_filestr_from_path(String& filepath) {

	String out = str_create("");

	sizet index = filepath.length - 1;

	while (filepath[index] != '/' && index > 0) {
		str_push(&out, filepath[index]);
		index--;
	}
	str_reverse(&out);

	return out;
}

Buffer*
buffer_add(File& file) {

	String key = get_filestr_from_path(file.path);


	Member<Buffer>* Node = Buffers.head;

	while (Node) {
		if (Node->data.path == file.path)  {
			NORMAL_MSG("File already loaded : %s \n", file.path.as_cstr());
			return &Node->data;
		}

		Node = Node->next;
	}

	NORMAL_MSG("Added file: %s \n", file.path.as_cstr());
	list_add(&Buffers, buffer_create(file));
	Buffers.tail->data.name = str_create(key.as_cstr());

	return &Buffers.tail->data;
}

void
buffer_add_empthy() {

	static i32 index = 0;
	static i8 numberIndex = 8;
	static char bufferName[] = "<empthy 0>";

	bufferName[numberIndex] = '0' + index;

	list_add(&Buffers, buffer_create_empthy());
	Buffers.tail->data.name = str_create(bufferName);

	index++;
}

void
buffer_add_empthy(const char* bufferName) {

	list_add(&Buffers, buffer_create_empthy());
	Buffers.tail->data.name = str_create(bufferName);
}


Buffer*
buffer_get(const char* key) {

	String keystr = str_create(key);

	Member<Buffer>* Node = Buffers.head;
	while (Node) {
		if (Node->data.path == key) 
			return &Node->data;

		Node = Node->next;
	}

	return NULL;
}


Buffer
buffer_create(File& file) {
  
	Buffer buf;

	buf.preLen = 0;
	buf.gapLen = 0;
	buf.cursorXtabed = 0;
	buf.curX = 0;
	buf.currentLine = 0;
	buf.postLen = file.size;
	buf.path = file.path;
	array_init(&buf.lineLengths, file.lineCount);
	array_init(&buf.cursorLines, file.lineCount);

	sizet i = 0;
	// foreach line
	for (sizet line = 0; line < file.lineCount; ++line) {
		
		array_push(&buf.lineLengths, 0);
		array_push(&buf.cursorLines, 0);
		// foreach char in line
		while (file.buffer[i] != '\n' &&
			   i < file.size) {
			// add to line length
			if (file.buffer[i] == '\t') 
				buf.cursorLines[line] += TAB_SIZE;
			else
				buf.cursorLines[line] += 1;

			buf.lineLengths[line] += 1;
			i++;
		}
		i++;
		buf.cursorLines[line] += 1;
		buf.lineLengths[line] += 1;

	}

	if (file.size < BUFFER_EMPTHY_SIZE) {

		buf.size = BUFFER_EMPTHY_SIZE;
		buf.gapLen = BUFFER_EMPTHY_SIZE - file.size;
		buf.postLen = file.size;

		buf.text = file.buffer;
		buf.text = (char*)malloc(sizeof(char) * buf.size);

		for (sizet i = buf.preLen; i < buf.size; i++) 
			buf.text[i + buf.gapLen] = file.buffer[i];

		buf.text[buf.size - 1] = '\n';

		free(file.buffer);
	}
	else {
		
		buf.size = file.size;
		buf.text = file.buffer;
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
	buf.size = BUFFER_EMPTHY_SIZE;
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

	for (sizet i = buf->preLen; i + buf->gapLen < buf->size; ++i) {
		str_push(&out, buf->text[i + buf->gapLen]);
	}

	return out;

}


void
buffer_insert_char(char c) {
	
	if (CurBuffer->gapLen == 0) {
		
		//sizet gap = CurBuffer->preLen + CurBuffer->postLen;
		//CurBuffer->size += gap;
		sizet gap = CurBuffer->size;
		CurBuffer->size *= 2;
		char* newbuf = (char*)malloc(sizeof(char) * CurBuffer->size);

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



sizet
buffer_index_based_on_line(Buffer* buf, i32 line) {

	sizet result = 0;
	for (sizet i = 0; i < line; ++i) {
		
		result += buf->lineLengths[i];
	}

	return result;
}

void
buffer_switch(const char* key) {
	
	Member<Buffer>* Node = Buffers.head;
	while (Node) {
		if (Node->data.path == key) {

			CurBuffer = &Node->data;
		}
		Node = Node->next;
	}
}


void
buffer_clear(Buffer* buf) {

	array_reset(&buf->lineLengths);
	array_reset(&buf->cursorLines);
	array_push(&buf->lineLengths, 0);
	array_push(&buf->cursorLines, 0);

	if (buf->path.data) {
		str_free(&buf->path);
	}
	if (buf->name.data) {
		str_free(&buf->path);
	}

	buf->gapLen = buf->size;
	buf->preLen = 0;
	buf->postLen = 0;
	buf->currentLine = 0;
	buf->curX = 0;
	buf->cursorXtabed = 0;
}
