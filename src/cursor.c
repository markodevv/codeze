#include "cursor.h"
#include "renderer.h"
#include "buffer.h"
#include "window.h"
#include "config.h"

Vec2
cursor_render_pos(Buffer* buf, Window* win) {
  
	string cursorString = buffer_string_before_cursor(buf);
	Vec2 pos;
	pos.x = win->position.x;
	pos.y = win->position.y + renderer_font_size() * (buf->currentLine - win->renderView.start);

	GlyphData* glyphs = renderer_get_glyphs();

	for (sizet i = 0; i < STR_LENGTH(cursorString); ++i) {

		pos.x += glyphs[cursorString[i]].advanceX;
	}

	str_release(cursorString);
	return pos;
}

Vec2
cursor_render_size(Buffer* buf) {
	GlyphData* glyphs = renderer_get_glyphs();
	Vec2 size = {
		glyphs[buffer_char_under_cursor(buf)].advanceX,
		renderer_font_size()
	};

	return size;
}

void
cursor_right(Buffer* b) {
  
	if (b->text[b->preLen + b->gapLen] == '\n'
		|| b->postLen <= 0) {
		return;
	}

	if (b->text[b->preLen + b->gapLen] == '\t')
		b->cursorXtabed += TAB_SIZE;
	else 
		b->cursorXtabed++;

	b->curX++;
	buffer_forward(b);
}

void
cursor_left(Buffer* b) {

	if (b->preLen == 0) return;

	if (b->text[b->preLen - 1] == '\n')
		return;
	else if (b->text[b->preLen - 1] == '\t')
		b->cursorXtabed -= TAB_SIZE;
	else 
		b->cursorXtabed--;

	b->curX--;
	buffer_backward(b);

}

void
cursor_down(Buffer* b, Window* win) {
	
	if (b->currentLine == ARRAY_LENGTH(b->cursorLines) - 1) return;


	while (b->text[b->preLen + b->gapLen] != '\n') {
		buffer_forward(b);
	}
	buffer_forward(b);
	b->currentLine++;

	u32 currLineLen = b->cursorLines[b->currentLine] - 1;

	if (currLineLen >= b->cursorXtabed) {

		sizet prevX = b->cursorXtabed;
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

		b->cursorXtabed = 0;
		b->curX = 0;
		while (b->text[b->preLen + b->gapLen] != '\n') {
			
			if (b->text[b->preLen + b->gapLen] == '\t') {
				b->cursorXtabed += TAB_SIZE;
			}
			else {
				b->cursorXtabed++;
			}
			b->curX++;
			buffer_forward(b);
		}
	}

}

void
cursor_up(Buffer* b, Window* win) {
	
	if (b->currentLine == 0) return;

	while (b->text[b->preLen - 1] != '\n') {

		buffer_backward(b);
	}
	buffer_backward(b);
	b->currentLine--;

	i32 backwardSteps = (b->cursorLines[b->currentLine] - 1) - b->cursorXtabed;

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
		
		b->cursorXtabed = b->cursorLines[b->currentLine] - 1;
		b->curX = b->lineLengths[b->currentLine] - 1;
	}

}
