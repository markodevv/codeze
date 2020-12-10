#include "window.h"
#include "buffer.h"
#include "container.h"
#include "debug.h"
#include "cursor.h"

#define MIN_WINDOW_WIDTH 256
#define MIN_WINDOW_HEIGHT 256
#define BIG_NUMBER 69696969


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
			out = find_window_at_point(&node->children[i], point);
			if (out)
				return out;
		}
	}

	return NULL;
}

Window
empthy_window() {
	Window window = {
		new_window_id(),
		{0, 0},
		{0, 0},
		{0, 0},
		NODE_WINDOW,
		NULL
	};
	return window;
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

	printf("\n\n--- TREE ---\n");
	print_tree(tree);
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

static void
resize_windows_evenly_width(Node* parent, i32 xpos, i32 totalWidth, i32 winCount) {
	
	i32 width = totalWidth / winCount;

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].position.x = xpos;
			parent->children[i].size.w = width;
			xpos += width;
		}
	}
}
static void
resize_windows_evenly_height(Node* parent, i32 ypos, i32 totalHeight, i32 winCount) {
	
	i32 height = totalHeight / winCount;

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].position.y = ypos;
			parent->children[i].size.h = height;
			ypos += height;
		}
	}
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
			Node* root = focusedWindow->parent->parent;
			Node* other;
			if (focusedWindow->parent->children[0].id != focusedWindow->id)
				other = focusedWindow->parent->children + 0;
			else
				other = focusedWindow->parent->children + 1;
				
			Node* toDelete = focusedWindow->parent->children;
			root->children = array_push(root->children, other);
			other = array_top(root->children);
			i32 otherid = other->id;


			// change container to window
			for (sizet i = 0; i < ARRAY_LENGTH(root->children); ++i) {
				if (root->children[i].nodeType == NODE_CONTAINER) {
					root->children[i].position = focusedWindow->position;
					root->children[i].size = focusedWindow->size;
					root->children[i].renderView = focusedWindow->renderView;
					root->children[i].id = focusedWindow->id;
					root->children[i].nodeType = focusedWindow->nodeType;
					focusedWindow = root->children + i;
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


			for (sizet i = 0; i < ARRAY_LENGTH(root->children); ++i) {

				if (root->children[i].id == focusedWindow->id) {

					array_erase(root->children, i);
					break;
				}
			}
			printf("array lnegth :: %zu \n", ARRAY_LENGTH(root->children));
			for (sizet i = 0; i < ARRAY_LENGTH(root->children); ++i) {

				if (root->children[i].id == otherid) {
					other = &root->children[i];
					other->parent = root;
					break;
				}
			}
			
		}
	}
	else {
		Node* parent = focusedWindow->parent;
		if (parent->isVertical) {
			
			i32 totalWidth = 0;
			i32 startx = BIG_NUMBER;
			i32 eraseAt = 0;
			i32 winCount = 0;

			for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {
			  
				if (parent->children[i].nodeType == NODE_WINDOW) {

					winCount++;
					if (startx > parent->children[i].position.x) {
						startx = parent->children[i].position.x;
					}
					totalWidth += parent->children[i].size.w;

					if (parent->children[i].id == focusedWindow->id) {
						eraseAt = i;
					}
				}
			}
			array_erase(parent->children, eraseAt);

			resize_windows_evenly_width(parent, startx, totalWidth, winCount);

			// This is temporray, windows shoudl be sorted and focused window should
			// be picked acording to last window
			for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {
			  
				if (parent->children[i].nodeType == NODE_WINDOW) {
					return &parent->children[i];
				}
			}

		}
		else {
			

		}
	}

	return focusedWindow;
}


void
print_tree(Node* node) {

	printf("\n \n");
	printf("---- Container ---- \n");
	printf("Adress : %p \n", node);
	printf("Parent : %p \n", node->parent);
	printf("Children : %p \n", node->children);
	printf("Is Vertical : %i \n", node->isVertical);
	printf("Children count : %zu \n", ARRAY_LENGTH(node->children));

	for (sizet i = 0; i < ARRAY_LENGTH(node->children); ++i)  {

		if (node->children[i].nodeType == NODE_WINDOW) {
			printf("\n");
			printf("---- Window ----\n");
			printf("Adress : %p \n", node->children + i);
			printf("Parent : %p \n", node->children[i].parent);
			printf("id : %i \n", node->children[i].id);
			printf("Position :");
			vec2i_print(node->children[i].position);
			printf("Size :");
			vec2i_print(node->children[i].size);
		}
	}
	for (sizet i = 0; i < ARRAY_LENGTH(node->children); ++i)  {

		if (node->children[i].nodeType == NODE_CONTAINER) {

			print_tree(&node->children[i]);
		}
	}
}

static inline Window*
window_find(Node* parent, i32 id) {
	
	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].id == id) {
			return &parent->children[i];
		}
	}
	return NULL;
}

Window*
window_split_horizontal(NodeTree tree, Window* focusedWindow) {
	
	if (focusedWindow->size.h / 2 <= MIN_WINDOW_HEIGHT)
		return focusedWindow;

	if (focusedWindow->parent->isVertical) {
		
		Window* children = array_create(2, sizeof(Window));
		children = array_push(children, focusedWindow);
		Node* node = focusedWindow;
		node->nodeType = NODE_CONTAINER;
		node->isVertical = 0;
		node->children = children;

		focusedWindow = node->children + 0;
		focusedWindow->parent = node;
		focusedWindow->size.h /= 2;

		Window newWindow = empthy_window();
		newWindow.position.x = focusedWindow->position.x;
		newWindow.position.y = focusedWindow->position.y + focusedWindow->size.h;
		newWindow.size = focusedWindow->size;
		newWindow.parent = node;

		node->children = array_push(node->children, &newWindow);

		printf("\n\n-- TREE -- \n");
		print_tree(tree);
		return node->children + 0;
	}

	Window win;
	Node* parent = focusedWindow->parent;
	i32 height = 0;
	i32 ypos = BIG_NUMBER;

	if (parent->children[0].nodeType == NODE_WINDOW) {
		
		win = empthy_window();
		win.size.w = parent->children[0].size.w;
		win.position.x = parent->children[0].position.x;
		win.size.h = 0;
		win.parent= focusedWindow->parent;
	}
	else {

		win = empthy_window();
		win.size.w = parent->children[1].size.w;
		win.position.x = parent->children[1].position.x;
		win.size.h = 0;
		win.parent= focusedWindow->parent;
	}

	i32 winCount = 0;
	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			winCount++;
			height += parent->children[i].size.h;
			if (ypos > parent->children[i].position.y) {
				ypos = parent->children[i].position.y;
			}
		}
	}

	i32 focusedwinId = focusedWindow->id;
	parent->children = array_push(parent->children, &win);
	winCount++;

	focusedWindow = window_find(parent, focusedwinId);
	ASSERT_MSG(focusedWindow, "No window with that id in parent->children");

	resize_windows_evenly_height(parent, ypos, height, winCount);

	printf("\n\n-- TREE -- \n");
	print_tree(tree);
	return focusedWindow;
}



Window*
window_split_vertical(NodeTree tree, Window* focusedWindow) {
	
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
		node->children[0].parent = node;
		node->children[0].size.w /= 2;

		Window newWindow;
		newWindow.position.x = focusedWindow->size.w + focusedWindow->position.x;
		newWindow.position.y = focusedWindow->position.y;
		newWindow.size = focusedWindow->size;
		newWindow.renderView.x = 0;
		newWindow.renderView.y = 0;
		newWindow.parent = node;
		newWindow.id = new_window_id();

		node->children = array_push(node->children, &newWindow);

		printf("\n\n-- TREE -- \n");
		print_tree(tree);
		return node->children + 0;
	}

	Window win;
	Node* parent = focusedWindow->parent;
	i32 width = 0;
	i32 xpos = BIG_NUMBER;

	if (parent->children[0].nodeType == NODE_WINDOW) {
		
		win = empthy_window();
		win.size.w = 0;
		win.size.h = parent->children[0].size.h;
		win.position.y = parent->children[0].position.y;
		win.parent= focusedWindow->parent;
	}
	else {

		win = empthy_window();
		win.size.w = 0;
		win.size.h = parent->children[1].size.h;
		win.position.y = parent->children[1].position.y;
		win.parent= focusedWindow->parent;
	}

	i32 winCount = 0;
	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			winCount++;
			width += parent->children[i].size.w;
			if (xpos > parent->children[i].position.x) {
				xpos = parent->children[i].position.x;
			}
		}
	}

	i32 focusedwinId = focusedWindow->id;
	parent->children = array_push(parent->children, &win);
	winCount++;
	focusedWindow = window_find(parent, focusedwinId);
	ASSERT_MSG(focusedWindow, "No window with that id in parent->children");

	resize_windows_evenly_width(parent, xpos, width, winCount);

	printf("\n\n-- TREE -- \n");
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
