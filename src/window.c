#include "window.h"
#include "buffer.h"
#include "container.h"
#include "debug.h"
#include "cursor.h"

#define MIN_WINDOW_WIDTH 420
#define MIN_WINDOW_HEIGHT 256

/*
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
get_window_at_point(WindowArray windows, Vec2i point) {
	
	for (i32 i = 0; i < ARRAY_LENGTH(windows); ++i) {
		
		if (windows[i].active) {

			if (is_point_in_rect_i(point, windows[i].position, windows[i].size)) {

				return i;
			}
		}
	}
	return -1;

}

typedef enum Dimension {

	DIM_WIDTH,
	DIM_HEGHT

} Dimension;

static i32
window_sum(WindowArray windows, i32* winIds, Dimension dim) {
	
	i32 sum = 0;
	if (dim == DIM_WIDTH) {
		for (sizet i = 0; i < ARRAY_LENGTH(winIds); ++i) {

			sum += windows[winIds[i]].size.w;
		}
		
	}
	else {
		for (sizet i = 0; i < ARRAY_LENGTH(winIds); ++i) {

			sum += windows[winIds[i]].size.h;
		}
		
	}

	return sum;
}

i32
window_close(Buffer* buf, WindowArray windows, i32 focusedWin) {
	
	for (sizet dir = 0; dir < WIN_DIRECTIONS; ++dir) {
		
		i32 id = window_switch(buf, windows, focusedWin, (WinDirection)dir);

		if (id != focusedWin) {
			switch ((WinDirection)dir) {
			case WIN_UP: {

				i32* ids = array_create(10, sizeof(i32));
				windows[id].size.h += windows[focusedWin].size.h;
				ids = array_push(ids, &id);

				for (;;) {

					if (window_sum(windows, ids, DIM_WIDTH) + 10 <
						windows[focusedWin].size.w) {

						i32 last = ids[ARRAY_LENGTH(ids) - 1];
						Vec2i point = {
							windows[last].position.x + windows[last].size.w,
							windows[last].position.y + (windows[last].size.h / 2),
						};
						point.x += 10;
						point.y -= 10;
						i32 another = get_window_at_point(windows, point);
						if (another != -1) {
							windows[another].size.h += windows[focusedWin].size.h;
							ids = array_push(ids, &another);
						}
						else  {
							break;
						}

					}
					else {
						break;
					}
				}

				array_release(ids);
				windows[focusedWin].active = 0;
				return id;
			}

			case WIN_DOWN: {

				i32* ids = array_create(10, sizeof(i32));
				windows[id].position.y -= windows[focusedWin].size.h;
				windows[id].size.h += windows[focusedWin].size.h;
				ids = array_push(ids, &id);

				for (;;) {

					if (window_sum(windows, ids, DIM_WIDTH) + 10 <
						windows[focusedWin].size.w) {

						i32 last = ids[ARRAY_LENGTH(ids) - 1];
						Vec2i point = {
							windows[last].position.x + windows[last].size.w,
							windows[last].position.y + (windows[last].size.h / 2),
						};
						point.x += 10;
						point.y -= 10;
						i32 another = get_window_at_point(windows, point);
						if (another != -1) {
							windows[another].position.y -= windows[focusedWin].size.h;
							windows[another].size.h += windows[focusedWin].size.h;
							ids = array_push(ids, &another);
						}
						else  {
							break;
						}

					}
					else {
						break;
					}
				}

				array_release(ids);
				windows[focusedWin].active = 0;
				return id;

			} break;

			case WIN_RIGHT: {

				windows[id].position.x -= windows[focusedWin].size.w;
				windows[id].size.w += windows[focusedWin].size.w;
				windows[focusedWin].active = 0;
				return id;
			} break;

			case WIN_LEFT: {

				windows[id].size.w += windows[focusedWin].size.w;
				windows[focusedWin].active = 0;
				return id;
			}break;

			}
		}
	}

	return focusedWin;
}


i32
window_switch(Buffer* buf, WindowArray windows, i32 winId, WinDirection dir) {
	
	Vec2 temp = cursor_render_pos(buf, windows);
	Vec2i point;
	point.x = (i32)temp.x;
	point.y = (i32)temp.y;

	switch (dir) {
	case WIN_LEFT:
		point.x = windows[winId].position.x;
		point.y = windows[winId].position.y;
		point.x -= 10.0f;
		point.y += 10.0f;
		break;
	case WIN_RIGHT:
		point.x = windows[winId].position.x + windows[winId].size.w;
		point.y = windows[winId].position.y;
		point.x += 10.0f;
		point.y += 10.0f;
		break;
	case WIN_UP:
		point.x = windows[winId].position.x;
		point.y = windows[winId].position.y;
		point.x += 10.0f;
		point.y -= 10.0f;
		break;
	case WIN_DOWN:
		point.x = windows[winId].position.x;
		point.y = windows[winId].position.y + windows[winId].size.h;
		point.x += 10.0f;
		point.y += 10.0f;
		break;
	}


	for (i32 i = 0; i < ARRAY_LENGTH(windows); ++i) {
		
		if (windows[i].active &&
			is_point_in_rect_i(point, windows[i].position, windows[i].size)) {

			return i;
		}
	}
	return winId;
}

*/

NodeTree
window_tree_create(Window window) {
  
	NodeTree tree = malloc(sizeof(Node));
	tree->isVertical = 0;
	tree->children = array_create(1, sizeof(Window));
	tree->children = array_push(tree->children, &window);
	tree->children[0].parent = tree;
	tree->parent = NULL;

	return tree;
}


Window*
window_split_verticaly(NodeTree tree, Window* focusedWindow) {
	
	if (focusedWindow->size.w / 2 <= MIN_WINDOW_WIDTH)
		return focusedWindow;

	if (!focusedWindow->parent->isVertical) {

		Node* node = focusedWindow;
		Window* children = array_create(2, sizeof(Window));
		children = array_push(children, focusedWindow);

		node->nodeType = NODE_CONTAINER;
		node->isVertical = 1;
		node->children = children;

		focusedWindow = &node->children[0];
		focusedWindow->size.w /= 2;
		Window newWindow;
		newWindow.position.x = focusedWindow->size.w + focusedWindow->position.x;
		newWindow.position.y = focusedWindow->position.y;
		newWindow.size.x = focusedWindow->size.w;
		newWindow.size.y = focusedWindow->size.h;
		newWindow.renderView.x = 0;
		newWindow.renderView.y = 0;
		newWindow.parent = node;

		node->children = array_push(node->children, &newWindow);

		return &node->children[0];
	}

	focusedWindow->size.w /= 2;
	Window newWindow;
	newWindow.position.x = focusedWindow->size.w + focusedWindow->position.x;
	newWindow.position.y = focusedWindow->position.y;
	newWindow.size.x = focusedWindow->size.w;
	newWindow.size.y = focusedWindow->size.h;
	newWindow.renderView.x = 0;
	newWindow.renderView.y = 0;
	newWindow.parent = focusedWindow->parent;

	focusedWindow->parent->children =
		array_push(focusedWindow->parent->children, &newWindow);

	return focusedWindow;


}

Window*
tree_get_windows(Node* node, Window* windows) {


	for (sizet i = 0; i < ARRAY_LENGTH(node->children); ++i) {
		
		if (node->children[i].nodeType == NODE_WINDOW) {
			windows = array_push(windows, &node->children[i]);
		}
		else {
			windows = tree_get_windows(&node->children[i], windows);
		}
	}

	return windows;
}

// window_split_verticaly(WindowArray windows, i32 winId) {

// 	if (windows[winId].size.w / 2 <= MIN_WINDOW_WIDTH)
// 		return windows;

// 	windows[winId].size.w /= 2;

// 	Window* window = find_inactive_window(windows);
// 	if (window != NULL) {

// 		window->renderView.start = 0;
// 		window->renderView.end = 0;
// 		window->position.x = windows[winId].size.w + windows[winId].position.x;
// 		window->position.y = windows[winId].position.y;
// 		window->size.x = windows[winId].size.w;
// 		window->size.y = windows[winId].size.h;
// 		window->active = 1;
// 	}
// 	else {

// 		printf("Unable to split window \n");
// 	}
  
// 	return windows;
// }
