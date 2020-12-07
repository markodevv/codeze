#pragma once
#include "math.h"

struct Buffer;
struct Node;
Vec2 cursor_render_pos(struct Buffer* buf, struct Node* win);
Vec2 cursor_render_size(struct Buffer* buf);
void cursor_right(struct Buffer* b);
void cursor_left(struct Buffer* b);
void cursor_up(struct Buffer* b);
void cursor_down(struct Buffer* b);

