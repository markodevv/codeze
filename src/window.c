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
	
	i32 remainder = totalWidth % winCount;
	i32 width = totalWidth / winCount;

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].position.x = xpos;
			parent->children[i].size.w = width;
			xpos += width;
		}
	}

	for (sizet i = ARRAY_LENGTH(parent->children) - 1; i >= 0; --i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].size.w += remainder;
			break;
		}
	}

}
static void
resize_windows_evenly_height(Node* parent, i32 ypos, i32 totalHeight, i32 winCount) {
	
	i32 remainder = totalHeight % winCount;
	i32 height = totalHeight / winCount;

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].position.y = ypos;
			parent->children[i].size.h = height;
			ypos += height;
		}
	}

	for (sizet i = ARRAY_LENGTH(parent->children) - 1; i >= 0; --i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].size.h += remainder;
			break;
		}
	}
}

static void
resize_windows_recursively_width(Node* parent, i32 startX, i32 width) {
	
	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			if (!parent->isVertical) {
				
				parent->children[i].position.x = startX;
				parent->children[i].size.w += width;
			}
			else {
				
				parent->children[i].position.x = startX;
				parent->children[i].size.w += width;
				startX += parent->children[i].size.w;
			}
		}
	}

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_CONTAINER) {

			if (!parent->isVertical)
				resize_windows_recursively_width(&parent->children[i], startX, width / 2);
			else 
				resize_windows_recursively_width(&parent->children[i], startX, width);
		}
	}

}

static void
resize_windows_recursively_height(Node* parent, i32 startY, i32 height) {
	
	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			if (!parent->isVertical) {
				
				parent->children[i].position.y = startY;
				parent->children[i].size.h += height;
				startY += parent->children[i].size.h;
			}
			else {
				
				parent->children[i].position.y = startY;
				parent->children[i].size.h += height;
			}
		}
	}

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_CONTAINER) {

			if (!parent->isVertical)
				resize_windows_recursively_height(&parent->children[i], startY, height);
			else 
				resize_windows_recursively_height(&parent->children[i], startY, height / 2);
		}
	}

}


static Window*
find_first_window_recursively(Node* node) {

	for (sizet i = 0; i < (ARRAY_LENGTH(node->children)); ++i) {

		if (node->children[i].nodeType == NODE_WINDOW) {
			return &node->children[i];
		}
	}

	for (sizet i = 0; i < (ARRAY_LENGTH(node->children)); ++i) {

		if (node->children[i].nodeType == NODE_CONTAINER) {
			Window* out = find_first_window_recursively(&node->children[i]);
			return out;
		}
	}

	// ASSERT_MSG(NULL, "now window found");
	return NULL;
}

static Window*
find_first_window(Node* parent) {

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW)
			return &parent->children[i];
	}
	return NULL;
}

static inline Window*
window_find(Node* parent, i32 id) {
	
	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			if (parent->children[i].id == id) {
				return &parent->children[i];
			}
		}
		else {
			window_find(&parent->children[i], id);
		}
	}
	return NULL;
}

Window*
window_close(NodeTree tree, Window* focusedWindow) {

	// check is there is container or window at parent
	if (ARRAY_LENGTH(focusedWindow->parent->children) == 1) return focusedWindow;


	if (ARRAY_LENGTH(focusedWindow->parent->children) == 2) {
		Window* outWindow;
		Node* parent = focusedWindow->parent;
		Node* node;
		Window focusWinData = *focusedWindow;

		// get the other child
		if (parent->children[0].id == focusedWindow->id)
		  node = &parent->children[1];
		else
		  node = &parent->children[0];

		if (node->nodeType == NODE_CONTAINER) {

			Node* childWindow = find_first_window_recursively(node);

			if (parent->isVertical) {
				
				i32 xpos;

				if (parent->children[0].id == focusedWindow->id)
					array_erase(parent->children, 0);
				else
					array_erase(parent->children, 1);

				node = &parent->children[0];

				// if child window is to the right
				if (childWindow->position.x > focusWinData.position.x)
					xpos = focusWinData.position.x;
				else
					xpos = childWindow->position.x;

				resize_windows_recursively_width(node, xpos, focusWinData.size.w);


			} else {
				
				i32 ypos;

				if (parent->children[0].id == focusedWindow->id)
					array_erase(parent->children, 0);
				else
					array_erase(parent->children, 1);

				node = &parent->children[0];

				// if child window is down
				if (childWindow->position.y > focusWinData.position.y) 
					ypos = focusWinData.position.y;
				else 
					ypos = childWindow->position.y;

				resize_windows_recursively_height(node, ypos, focusWinData.size.h);

			}
		} else {

			if (parent->isVertical) {
				
				Window* other;

				if (parent->children[0].id == focusedWindow->id)
					array_erase(parent->children, 0);
				else
					array_erase(parent->children, 1);

				other = &parent->children[0];

				if (focusWinData.position.x > other->position.x) {
					
					other->size.w += focusWinData.size.w;
				}
				else {
					
					other->position.x = focusWinData.position.x;
					other->size.w += focusWinData.size.w;
				}

				outWindow = other;
			} else {
				
				Window* other;

				if (parent->children[0].id == focusedWindow->id)
					array_erase(parent->children, 0);
				else
					array_erase(parent->children, 1);

				other = &parent->children[0];

				if (focusWinData.position.y > other->position.y) {
					
					other->size.h += focusWinData.size.h;
				}
				else {
					
					other->position.y = focusWinData.position.y;
					other->size.h += focusWinData.size.h;
				}

				outWindow = other;
			}
		}

		if (parent->parent != NULL && ARRAY_LENGTH(parent->children) == 1) {

			Node* root = parent->parent;
			b8 isContainer = parent->children[0].nodeType == NODE_CONTAINER;

			// move child children to parent children
			// delete parent container
			Node* nodesToMoveUp;
			Node* toDelete;

			if (isContainer) {
				// if its a container we move parent->children->children
				// to root
				nodesToMoveUp = parent->children->children;
				toDelete = parent->children;
			}
			else {
				// if not we only move the window parent->children[0]
				nodesToMoveUp = parent->children;
			}


			sizet insertIndex = 0;
			// get parent index
			for (sizet i = 0; i < ARRAY_LENGTH(root->children); ++i) {

				if (&root->children[i] == parent) {
					insertIndex = i;
					break;
				}
			}
			// convert parent to 1 window
			root->children[insertIndex] = nodesToMoveUp[0];
			root->children[insertIndex].parent = root;

			if (isContainer) {

				for (sizet i = 1; i < ARRAY_LENGTH(nodesToMoveUp); ++i) {

					root->children = array_insert(root->children, insertIndex, &nodesToMoveUp[i]);
					root->children[insertIndex].parent = root;
					insertIndex++;
				}

				for (sizet i = 0; i < ARRAY_LENGTH(root->children); ++i) {

					if (root->children[i].nodeType == NODE_CONTAINER) {

						for (sizet k = 0; k < ARRAY_LENGTH(root->children[i].children); ++k)
							root->children[i].children[k].parent = &root->children[i];
					}
				}
			}


			array_release(nodesToMoveUp);
			if (isContainer)
				array_release(toDelete);

			outWindow = find_first_window_recursively(root);

		}
		printf("--------- \nTREE _______ \n");
		print_tree(tree);
		ASSERT(outWindow);
		return outWindow;

	} else if (ARRAY_LENGTH(focusedWindow->parent->children) > 2) {
		
		// Node* parent = focusedWindow->parent;
		// Window focusWinData = *focusedWindow;
		// i32 xpos;
		// if (parent->isVertical) {

		// 	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		// 		if (parent->children[i].nodeType == NODE_WINDOW) {
		// 			xpos = parent->children[i].position.x;
		// 		}
		// 	}
		// 	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		// 		if (parent->children[i].id == focusedWindow->id) {
		// 			array_erase(parent->children, i);
		// 		}
		// 	}
		// 	resize_windows_recursively_width(parent->children, xpos, focusWinData.size.w);
		// }
	}

	ASSERT(0);
	return focusedWindow;
}



Window*
window_split_horizontal(NodeTree tree, Window* focusedWindow) {
	
	if (focusedWindow->size.h / 2 <= MIN_WINDOW_HEIGHT)
		return focusedWindow;

	Node* parent = focusedWindow->parent;

	if (parent->isVertical) {
		
		Window* children = array_create(2, sizeof(Window));
		children = array_push(children, focusedWindow);

		Node* container = focusedWindow;
		container->nodeType = NODE_CONTAINER;
		container->isVertical = 0;
		container->children = children;

		focusedWindow = children + 0;
		focusedWindow->parent = container;
		focusedWindow->size.h /= 2;

		Window newWindow = empthy_window();
		newWindow.position.x = focusedWindow->position.x;
		newWindow.position.y = focusedWindow->position.y + focusedWindow->size.h;
		newWindow.size = focusedWindow->size;
		newWindow.parent = container;

		container->children = array_push(container->children, &newWindow);

	printf("\n\n TREE \n\n");
	print_tree(tree);
		return &container->children[0];
	}

	Window win;
	i32 height = 0;
	i32 ypos = BIG_NUMBER;

	Window* childWindow = find_first_window(parent);
	ASSERT(childWindow);
	win = empthy_window();
	win.size.w = childWindow->size.w;
	win.position.x = childWindow->position.x;
	win.size.h = 0;
	win.parent = parent;

	i32 winCount = 0;
	i32 focusedwinId = focusedWindow->id;

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			winCount++;
			height += parent->children[i].size.h;
			if (ypos > parent->children[i].position.y) {
				ypos = parent->children[i].position.y;
			}
			if (parent->children[i].id == focusedWindow->id) {
				parent->children = array_insert(parent->children, i + 1, &win);
				i++;
				winCount++;
			}
		}
	}

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_CONTAINER) {

			for (sizet k = 0; k < ARRAY_LENGTH(parent->children[i].children); ++k)
				parent->children[i].children[k].parent = &parent->children[i];
		}
	}

	focusedWindow = window_find(parent, focusedwinId);
	ASSERT_MSG(focusedWindow, "No window with that id in parent->children");

	resize_windows_evenly_height(parent, ypos, height, winCount);

	printf("\n\n TREE \n\n");
	print_tree(tree);
	return focusedWindow;
}


Window*
window_split_vertical(NodeTree tree, Window* focusedWindow) {
	
	if (focusedWindow->size.w / 2 <= MIN_WINDOW_WIDTH)
		return focusedWindow;

	Node* parent = focusedWindow->parent;

	if (!parent->isVertical) {

		Window* children = array_create(2, sizeof(Window));
		children = array_push(children, focusedWindow);

		Node* container = focusedWindow;
		container->nodeType = NODE_CONTAINER;
		container->isVertical = 1;
		container->children = children;

		focusedWindow = children + 0;
		focusedWindow->parent = container;
		focusedWindow->size.w /= 2;

		Window newWindow = empthy_window();
		newWindow.position.x = focusedWindow->size.w + focusedWindow->position.x;
		newWindow.position.y = focusedWindow->position.y;
		newWindow.size = focusedWindow->size;
		newWindow.parent = container;

		container->children = array_push(container->children, &newWindow);
		printf("\n\n TREE \n\n");
		print_tree(tree);

		return container->children + 0;
	}

	Window win;
	i32 width = 0;
	i32 xpos = BIG_NUMBER;

	Window* childWindow = find_first_window(parent);
	ASSERT(childWindow);
	win = empthy_window();
	win.size.w = 0;
	win.size.h = childWindow->size.h;
	win.position.y = childWindow->position.y;
	win.parent = parent;

	i32 winCount = 0;
	i32 focusedwinId = focusedWindow->id;

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			winCount++;
			width += parent->children[i].size.w;
			if (xpos > parent->children[i].position.x) {
				xpos = parent->children[i].position.x;
			}
			if (parent->children[i].id == focusedWindow->id) {
				parent->children = array_insert(parent->children, i + 1, &win);
				i++;
				winCount++;
			}
			
		}
	}

	for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

		if (parent->children[i].nodeType == NODE_CONTAINER) {

			for (sizet k = 0; k < ARRAY_LENGTH(parent->children[i].children); ++k)
				parent->children[i].children[k].parent = &parent->children[i];
		}
	}

	focusedWindow = window_find(parent, focusedwinId);
	ASSERT_MSG(focusedWindow, "No window with that id in parent->children");

	resize_windows_evenly_width(parent, xpos, width, winCount);

	printf("\n\n TREE \n\n");
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
		else {
			print_tree(&node->children[i]);
		}
	}

}
