#include "cursor.h"
#include "renderer.h"

Vec2
cursor_render_pos(Buffer* buf, Window* win) {
  
	string cursorString = buffer_string_before_cursor(buf);
	Vec2 pos;
	pos.x = win->position.x;
	pos.y = win->position.y + renderer_font_size() * buf->currentLine;

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
		glyphs[buffer_char_before_cursor(buf)].advanceX,
		renderer_font_size()
	};

	return size;
}
