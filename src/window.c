#include "window.h"
#include "buffer.h"
#include "container.h"
#include "debug.h"
#include "cursor.h"

#define MIN_WINDOW_WIDTH 420
#define MIN_WINDOW_HEIGHT 256

static Window*
find_inactive_window(WindowArray windows) {
	
	for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {
		
		if (!windows[i].active) {
			return &windows[i];
		}
	}

	return NULL;
}

WindowArray
window_split_verticaly(WindowArray windows, i32 winId) {

	if (windows[winId].size.w / 2 <= MIN_WINDOW_WIDTH)
		return windows;

	windows[winId].size.w /= 2;

	Window* window = find_inactive_window(windows);
	if (window != NULL) {

		window->renderView.start = 0;
		window->renderView.end = 0;
		window->position.x = windows[winId].size.w + windows[winId].position.x;
		window->position.y = windows[winId].position.y;
		window->size.x = windows[winId].size.w;
		window->size.y = windows[winId].size.h;
		window->active = 1;
	}
	else {

		printf("Unable to split window \n");
	}
	// Window window = {
	// 	1, 
	// 	{0, 0},
	// 	{windows[winId].size.w + windows[winId].position.x, windows[winId].position.y},
	// 	{windows[winId].size.w, windows[winId].size.h}
	// };

	//windows = array_push(windows, &window);
  
	return windows;
}

WindowArray
window_split_horizontaly(WindowArray windows, i32 winId) {
	
	if (windows[winId].size.h / 2 <= MIN_WINDOW_HEIGHT)
		return windows;
	
	windows[winId].size.h /= 2;
	Window window = {
		1, 
		{0, 0},
		{windows[winId].position.x, windows[winId].position.y + windows[winId].size.h},
		{windows[winId].size.w, windows[winId].size.h}
	};

	windows = array_push(windows, &window);

	return windows;
}

static i32
get_window_at_point(WindowArray windows, Vec2i at) {
	
	Vec2i winSize;

	for (i32 i = 0; i < ARRAY_LENGTH(windows); ++i) {
		
		if (windows[i].active) {
			
			winSize.w = windows[i].size.w + windows[i].position.x;
			winSize.h = windows[i].size.h + windows[i].position.y;

			if (is_point_in_rect_i(at, windows[i].position, winSize)) {

				return i;
			}
		}
	}
	return -1;

}

i32
window_close(Buffer* buf, WindowArray windows, i32 winId) {
	
	for (sizet dir = 0; dir < WIN_DIRECTIONS; ++dir) {
		
		i32 id = window_switch(buf, windows, winId, (WinDirection)dir);

		if (id != winId) {
			switch ((WinDirection)dir) {
			case WIN_UP: {

				if (windows[id].size.w < windows[winId].size.w) {

					Vec2i point = {
						windows[id].position.x + windows[id].size.w,
						windows[id].position.y + windows[id].size.h,
					};
					point.x += 1;
					point.y -= 1;
					i32 another = get_window_at_point(windows, point);

					if (another != -1) {

						printf("another \n");
						windows[another].size.h += windows[winId].size.h;
					}
				}
				windows[id].size.h += windows[winId].size.h;
				windows[winId].active = 0;
				return id;
			} break;

			case WIN_DOWN: {

				windows[id].position.y -= windows[winId].size.h;
				windows[id].size.h += windows[winId].size.h;
				windows[winId].active = 0;
				return id;
			} break;

			case WIN_LEFT: {

				windows[id].size.w += windows[winId].size.w;
				windows[winId].active = 0;
				return id;
			}break;

			case WIN_RIGHT: {

				windows[id].position.x -= windows[winId].size.w;
				windows[id].size.w += windows[winId].size.w;
				windows[winId].active = 0;
				return id;
			} break;
			}
		}
	}

	return winId;
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

		if (windows[i].active &&
			is_point_in_rect_i(point, windows[i].position, winSize)) {

			return i;
		}
	}
	return winId;
}
