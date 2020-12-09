#include "window.h"
#include "buffer.h"
#include "container.h"
#include "debug.h"
#include "cursor.h"

#define MIN_WINDOW_WIDTH 256
#define MIN_WINDOW_HEIGHT 256


static Window*
find_window_at_point(Node* node, Vec2i point) {
	
	for (sizet i = 0; i < ARRAY_LENGTH(node->children); ++i) {
		
		if (node->children[i].nodeType == NODE_WINDOW) {

			if (is_point_in_rect_i(point,
								   node->children[i].position,
								   node->children[i].size)) {

				return &node->children[i];
			}
		}
		else {
			Window* out;
			out = find_window_at_point(node->children, point);
			if (out)
				return out;
		}
	}

	return NULL;
}

Window*
window_switch(NodeTree tree, Buffer* buf, Window* focusedWindow, WinDirection dir) {
	
	Vec2 temp = cursor_render_pos(buf, focusedWindow);
	Vec2i point;
	point.x = (i32)temp.x;
	point.y = (i32)temp.y;

	switch (dir) {
	case WIN_LEFT:
		point.x = focusedWindow->position.x;
		point.y = focusedWindow->position.y;
		point.x -= 10.0f;
		point.y += 10.0f;
		break;
	case WIN_RIGHT:
		point.x = focusedWindow->position.x + focusedWindow->size.w;
		point.y = focusedWindow->position.y;
		point.x += 10.0f;
		point.y += 10.0f;
		break;
	case WIN_UP:
		point.x = focusedWindow->position.x;
		point.y = focusedWindow->position.y;
		point.x += 10.0f;
		point.y -= 10.0f;
		break;
	case WIN_DOWN:
		point.x = focusedWindow->position.x;
		point.y = focusedWindow->position.y + focusedWindow->size.h;
		point.x += 10.0f;
		point.y += 10.0f;
		break;
	}

	Window* out = find_window_at_point(tree, point);
	if (out != NULL)  {
		return out;
	}
	return focusedWindow;
}

NodeTree
window_tree_create(Window window) {
  
	NodeTree tree = malloc(sizeof(Node));
	tree->isVertical = 0;
	tree->children = array_create(1, sizeof(Window));
	tree->nodeType = NODE_CONTAINER;
	window.parent = tree;
	tree->children = array_push(tree->children, &window);
	tree->parent = NULL;

	return tree;
}

static WindowArray
get_all_windows_from_parent(Node* children) {
	
	WindowArray windows = array_create(2, sizeof(Window));

	for (sizet i = 0; i < ARRAY_LENGTH(children); ++i) {
		
		if (children[i].nodeType == NODE_WINDOW) {
			
			windows = array_push(windows, children + i);
		}
	}

	return windows;
}

Window*
window_close(Window* focusedWindow) {

	// check is there is container or window at parent
	if (ARRAY_LENGTH(focusedWindow->parent->children) == 1) return focusedWindow;

	if (ARRAY_LENGTH(focusedWindow->parent->children) == 2) {
		
		Node* node;
		for (sizet i = 0; i < 2; ++i) {

			// if check if other child is container or window
			if (focusedWindow->id != focusedWindow->parent->children[i].id) {

				if (focusedWindow->parent->children[i].nodeType == NODE_WINDOW) {
					
					node = focusedWindow->parent->children + i;
					break;
				}
				node = focusedWindow->parent->children + i;
			}
		}

		if (node->nodeType == NODE_CONTAINER) {
			
			// move nodes up
			Node* toDelete = focusedWindow->parent->children;
			focusedWindow->parent->children = node->children;
			focusedWindow->parent->isVertical = node->isVertical;
			array_release(toDelete);

			// if container is vertical do positioning and scaling on y axis
			if (focusedWindow->parent->isVertical) {
				
				WindowArray windows =
				get_all_windows_from_parent(focusedWindow->parent->children);

				if (focusedWindow->position.y > windows[0].position.y) {
					for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {
					
						windows[i].size.h += focusedWindow->size.h;
					}
				}
				else {
					for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {
						
						windows[i].position.y = focusedWindow->position.y;
						windows[i].size.h += focusedWindow->size.h;
					}
				}
				array_release(windows);

				return focusedWindow->parent->children;
			}
			// else do it on x axis
			else {
				WindowArray windows =
				get_all_windows_from_parent(focusedWindow->parent->children);

				if (focusedWindow->position.x > windows[0].position.x) {
					for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {
						
						windows[i].size.w += focusedWindow->size.w;
					}
				}
				else {
					for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {
						
						windows[i].position.x = focusedWindow->position.x;
						windows[i].size.w += focusedWindow->size.w;
					}
				}
				array_release(windows);

				return focusedWindow->parent->children;
			}
		}
		// "delete" container and move windows up, and close focusedWindow
		else {
			Node* parentparent = focusedWindow->parent->parent;
			Node* other;
			if (focusedWindow->parent->children[0].id != focusedWindow->id)
				other = focusedWindow->parent->children + 0;
			else
				other = focusedWindow->parent->children + 1;
				
			Node* toDelete = focusedWindow->parent->children;
			parentparent->children = array_push(parentparent->children, other);
			other = array_top(parentparent->children);


			// change container to window
			for (sizet i = 0; i < ARRAY_LENGTH(parentparent->children); ++i) {
				if (parentparent->children[i].nodeType == NODE_CONTAINER) {
					parentparent->children[i].position = focusedWindow->position;
					parentparent->children[i].size = focusedWindow->size;
					parentparent->children[i].renderView = focusedWindow->renderView;
					parentparent->children[i].id = focusedWindow->id;
					parentparent->children[i].nodeType = focusedWindow->nodeType;
					focusedWindow = parentparent->children + i;
				}
			}
			array_release(toDelete);

			if (!focusedWindow->parent->isVertical) {

				if (other->position.x > focusedWindow->position.x) {
					
					other->position.x = focusedWindow->position.x;
					other->size.w += focusedWindow->size.w;
				}
				else {

					other->size.w += focusedWindow->size.w;
				}

			}
			else {

				if (other->position.y > focusedWindow->position.y) {
					
					other->position.y = focusedWindow->position.y;
					other->size.w += focusedWindow->size.w;
				}
				else {

					other->size.w += focusedWindow->size.w;
				}

			}
			// printf("\n \n \n");
			// printf("---- Window Other ----\n");
			// printf("Adress : %i \n", other);
			// printf("Parent : %i \n", other->parent);
			// printf("id : %i \n", other->id);
			// printf("Position :");
			// vec2i_print(other->position);
			// printf("Size :");
			// vec2i_print(other->size);


			i32 otherid = other->id;
			for (sizet i = 0; i < ARRAY_LENGTH(parentparent->children); ++i) {

				if (parentparent->children[i].id == focusedWindow->id) {

					array_erase(parentparent->children, i);
					break;
				}
			}
			printf("array lnegth :: %i \n", ARRAY_LENGTH(parentparent->children));
			for (sizet i = 0; i < ARRAY_LENGTH(parentparent->children); ++i) {

				if (parentparent->children[i].id == otherid) {
					other = &parentparent->children[i];
					other->parent = parentparent;
					break;
				}
			}
			
			// printf("\n \n \n");
			// printf("---- Window Other ----\n");
			// printf("Adress : %i \n", other);
			// printf("Parent : %i \n", other->parent);
			// printf("id : %i \n", other->id);
			// printf("Position :");
			// vec2i_print(other->position);
			// printf("Size :");
			// vec2i_print(other->size);

			printf("\n \n \n");
			print_tree(parentparent);
		}
	}

	return focusedWindow;
}

void
print_tree(Node* node) {

	printf("---- Container ---- \n");
	printf("Adress : %i \n", node);
	printf("Parent : %i \n", node->parent);
	printf("Is Vertical : %i \n", node->isVertical);
	printf("Children count : %i \n", ARRAY_LENGTH(node->children));
	printf("Children adress : %i \n", node->children);

	for (sizet i = 0; i < ARRAY_LENGTH(node->children); ++i)  {

		if (node->children[i].nodeType == NODE_WINDOW) {
			printf("---- Window ----\n");
			printf("Adress : %i \n", node->children + i);
			printf("Parent : %i \n", node->children[i].parent);
			printf("id : %i \n", node->children[i].id);
			printf("Position :");
			vec2i_print(node->children[i].position);
			printf("Size :");
			vec2i_print(node->children[i].size);
		}
		else {

			print_tree(&node->children[i]);
		}
	}
}

Window*
window_split_verticaly(NodeTree tree, Window* focusedWindow) {
	
	if (focusedWindow->size.w / 2 <= MIN_WINDOW_WIDTH)
		return focusedWindow;

	if (!focusedWindow->parent->isVertical) {

		Window* children = array_create(2, sizeof(Window));
		children = array_push(children, focusedWindow);
		Node* node = focusedWindow;
		node->nodeType = NODE_CONTAINER;
		node->isVertical = 1;
		node->children = children;

		focusedWindow = node->children + 0;
		focusedWindow->parent = node;
		focusedWindow->size.w /= 2;

		Window newWindow;
		newWindow.position.x = focusedWindow->size.w + focusedWindow->position.x;
		newWindow.position.y = focusedWindow->position.y;
		newWindow.size.x = focusedWindow->size.w;
		newWindow.size.y = focusedWindow->size.h;
		newWindow.renderView.x = 0;
		newWindow.renderView.y = 0;
		newWindow.parent = node;
		newWindow.id = new_window_id();

		node->children = array_push(node->children, &newWindow);

		printf("\n \n \n");
		print_tree(tree);
		return focusedWindow;
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
	newWindow.nodeType = NODE_WINDOW;
	newWindow.id = new_window_id();

	focusedWindow->parent->children = array_push(focusedWindow->parent->children, &newWindow);

	printf("\n \n \n");
	print_tree(tree);
	return focusedWindow;

}


WindowArray
tree_get_windows(Node* node, WindowArray windows) {


	for (sizet i = 0; i < ARRAY_LENGTH(node->children); ++i) {
		
		if (node->children[i].nodeType == NODE_WINDOW) {
			windows = array_push(windows, node->children + i);
		}
		else {
			windows = tree_get_windows(node->children + i, windows);
		}
	}

	return windows;
}

i32
new_window_id() {

	static i32 id = 0;
	id++;
	return id;

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
