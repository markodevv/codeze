#include "window.h"
#include "buffer.h"
#include "container.h"
#include "debug.h"
#include "cursor.h"

#define MIN_WINDOW_WIDTH 420
#define MIN_WINDOW_HEIGHT 256

WindowArray
window_split_verticaly(WindowArray windows, i32 winId) {

	if (windows[winId].size.w / 2 <= MIN_WINDOW_WIDTH)
		return windows;

	windows[winId].size.w /= 2;
	Window window = {
		{windows[winId].size.w + windows[winId].position.x, windows[winId].position.y},
		{windows[winId].size.w, windows[winId].size.h}
	};

	windows = array_push(windows, &window);
  
	return windows;
}

WindowArray
window_split_horizontaly(WindowArray windows, i32 winId) {
	
	if (windows[winId].size.h / 2 <= MIN_WINDOW_HEIGHT)
		return windows;
	
	windows[winId].size.h /= 2;
	Window window = {
		{windows[winId].position.x, windows[winId].position.y + windows[winId].size.h},
		{windows[winId].size.w, windows[winId].size.h}
	};

	windows = array_push(windows, &window);

	return windows;
}
void
window_destroy(Buffer* buf, WindowArray windows, i32 winId) {
	
	// i32 id = window_switch_right(buf, windows, winId);
	// if (id != winId) {
	// 	windows[id].size.
	// }
	// array_erase(windows, winId);

}


i32
window_switch(Buffer* buf, WindowArray windows, i32 winId, WinDirection dir) {
	
	Vec2 temp = cursor_render_pos(buf, windows);
	Vec2i point;
	Vec2i winSize;
	point.x = (i32)temp.x;
	point.y = (i32)temp.y;

	switch (dir) {
	case WIN_LEFT:
		point.x = windows[winId].position.x;
		point.y = windows[winId].position.y;
		point.x -= 1.0f;
		point.y += 1.0f;
		break;
	case WIN_RIGHT:
		point.x = windows[winId].position.x + windows[winId].size.w;
		point.y = windows[winId].position.y;
		point.x += 1.0f;
		point.y += 1.0f;
		break;
	case WIN_UP:
		point.x = windows[winId].position.x;
		point.y = windows[winId].position.y;
		point.x += 1.0f;
		point.y -= 1.0f;
		break;
	case WIN_DOWN:
		point.x = windows[winId].position.x;
		point.y = windows[winId].position.y + windows[winId].size.h;
		point.x += 1.0f;
		point.y += 1.0f;
		break;
	}


	for (i32 i = 0; i < ARRAY_LENGTH(windows); ++i) {
		
		winSize.w = windows[i].size.w + windows[i].position.x;
		winSize.h = windows[i].size.h + windows[i].position.y;

		if (is_point_in_rect_i(point, windows[i].position, winSize)) {
			return i;
		}
	}
	return winId;
}
