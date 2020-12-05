#pragma once
#include "math.h"
#include "types.h"

typedef enum WinDirection {

	WIN_UP,
	WIN_DOWN,
	WIN_LEFT,
	WIN_RIGHT,
	WIN_DIRECTIONS

} WinDirection;

typedef struct Window {
  
	b8 active;
	Vec2i renderView;
	Vec2i position;
	Vec2i size;

} Window;

typedef Window* WindowArray;

struct Buffer;
WindowArray window_split_verticaly(WindowArray windows, i32 winId);
WindowArray window_split_horizontaly(WindowArray windows, i32 winId);
i32 window_switch(struct Buffer* buf, WindowArray windows, i32 winId, WinDirection dir);
i32 window_close(struct Buffer* buf, WindowArray windows, i32 winId);
