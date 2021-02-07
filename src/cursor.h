#pragma once
#include "math.h"


enum CursorStyle {
				  
				  CURSOR_LINE,
				  CURSOR_BLOCK,
};

struct Buffer;
struct Node;
Vec2 cursor_render_pos(Buffer* buf, Node* win);
Vec2 cursor_render_size(CursorStyle style);
void cursor_right();
void cursor_left();
void cursor_up();
void cursor_down();
char char_under_cursor();

