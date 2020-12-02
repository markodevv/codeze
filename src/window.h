#pragma once
#include "math.h"
#include "types.h"

typedef enum WinDirection {

	WIN_LEFT,
	WIN_RIGHT,
	WIN_UP,
	WIN_DOWN
} WinDirection;

typedef struct Window {
  
	Vec2i position;
	Vec2i size;

} Window;

typedef Window* WindowArray;

struct Buffer;
WindowArray window_split_verticaly(WindowArray windows, i32 winId);
WindowArray window_split_horizontaly(WindowArray windows, i32 winId);
i32 window_switch(struct Buffer* buf, WindowArray windows, i32 winId, WinDirection dir);
void window_destroy(struct Buffer* buf, WindowArray windows, i32 winId);
