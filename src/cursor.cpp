#include "cursor.h"
#include "config.h"
#include "my_string.h"
#include "renderer.h"
#include "buffer.h"
#include "globals.h"


Vec2
cursor_render_pos() {
  
	String cursorString = buffer_string_before_cursor();
	Vec2 pos;
	pos.x = FocusedWindow->position.x;
	pos.y = FocusedWindow->position.y +
		renderer_font_size() *
		(CurBuffer->currentLine - FocusedWindow->renderView.start);

	GlyphData* glyphs = renderer_get_glyphs();

	for (sizet i = 0; i < cursorString.length; ++i) {

		pos.x += glyphs[cursorString.data[i]].advanceX;
	}

	return pos;
}

char
char_under_cursor() {

	if (CurBuffer->postLen > 0)
		return CurBuffer->text[CurBuffer->preLen + CurBuffer->gapLen];
	else 
		return CurBuffer->text[CurBuffer->preLen - 1];
}

Vec2
cursor_render_size() {

	GlyphData* glyphs = renderer_get_glyphs();
	Vec2 size = {
		glyphs[char_under_cursor()].advanceX,
		(f32)renderer_font_size()
	};

	return size;
}

void
cursor_right() {
  
	if (char_under_cursor() == '\n'
		|| CurBuffer->postLen <= 0) {
		return;
	}

	if (char_under_cursor() == '\t')
		CurBuffer->cursorXtabed += TAB_SIZE;
	else 
		CurBuffer->cursorXtabed++;

	CurBuffer->curX++;
	buffer_forward();
}

void
cursor_left() {

	if (CurBuffer->preLen == 0) return;

	if (CurBuffer->text[CurBuffer->preLen - 1] == '\n')
		return;
	else if (CurBuffer->text[CurBuffer->preLen - 1] == '\t')
		CurBuffer->cursorXtabed -= TAB_SIZE;
	else 
		CurBuffer->cursorXtabed--;

	CurBuffer->curX--;
	buffer_backward();

}

void
cursor_down() {
	
	if (CurBuffer->currentLine == CurBuffer->cursorLines.length - 1) return;


	while (char_under_cursor() != '\n') {
		buffer_forward();
	}
	buffer_forward();
	CurBuffer->currentLine++;

	u32 currLineLen = CurBuffer->cursorLines[CurBuffer->currentLine] - 1;

	if (currLineLen >= CurBuffer->cursorXtabed) {

		sizet prevX = CurBuffer->cursorXtabed;
		while (prevX > 0) {

			if (char_under_cursor() == '\t') {
				buffer_forward();
				prevX -= TAB_SIZE;
			}
			else {
				buffer_forward();
				prevX--;
			}
				
		}
	}
	else {

		CurBuffer->cursorXtabed = 0;
		CurBuffer->curX = 0;
		while (char_under_cursor() != '\n') {
			
			if (char_under_cursor() == '\t') {
				CurBuffer->cursorXtabed += TAB_SIZE;
			}
			else {
				CurBuffer->cursorXtabed++;
			}
			CurBuffer->curX++;
			buffer_forward();
		}
	}

}

void
cursor_up() {
	
	if (CurBuffer->currentLine == 0) return;

	while (CurBuffer->text[CurBuffer->preLen - 1] != '\n') {

		buffer_backward();
	}
	buffer_backward();
	CurBuffer->currentLine--;

	i32 backwardSteps = (CurBuffer->cursorLines[CurBuffer->currentLine] - 1) - CurBuffer->cursorXtabed;

	if (backwardSteps > 0) {

		while (backwardSteps > 0) {

			buffer_backward();
			if (CurBuffer->text[CurBuffer->preLen] == '\t')
				backwardSteps -= TAB_SIZE;
			else
				backwardSteps--;
		}
	}
	else {
		
		CurBuffer->cursorXtabed = CurBuffer->cursorLines[CurBuffer->currentLine] - 1;
		CurBuffer->curX = CurBuffer->lineLengths[CurBuffer->currentLine] - 1;
	}

}

