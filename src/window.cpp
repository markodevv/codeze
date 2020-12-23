#include "window.h"
#include "buffer.h"
#include "container.h"
#include "debug.h"
#include "cursor.h"

#define MIN_WINDOW_WIDTH 256
#define MIN_WINDOW_HEIGHT 256
#define BIG_NUMBER 69696969

static Node* WinTree;

static Window*
find_window_at_point(Node* node, Vec2i point) {
	
	for (sizet i = 0; i < node->children.length; ++i) {
		
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
window_create_empthy() {
	Window window = {
		new_window_id(),
		0,
		{0, 0},
		{0, 0},
		{0, 0},
		NODE_WINDOW,
		NULL
	};
	return window;
}

void
window_switch_up() {

	Vec2i point;
	point.x = FocusedWindow->position.x + 10.0f;
	point.y = FocusedWindow->position.y - 10.0f;

	Window* out = find_window_at_point(WinTree, point);
	if (out != NULL)  {
		FocusedWindow = out;
	}
}

void
window_switch_down() {

	Vec2i point;
	point.x = FocusedWindow->position.x + 10.0f;
	point.y = FocusedWindow->position.y + FocusedWindow->size.h + 10.0f;

	Window* out = find_window_at_point(WinTree, point);
	if (out != NULL)  {
		FocusedWindow = out;
	}
}

void
window_switch_left() {

	Vec2i point;
	point.x = FocusedWindow->position.x - 10.0f;
	point.y = FocusedWindow->position.y + 10.0f;

	Window* out = find_window_at_point(WinTree, point);
	if (out != NULL)  {
		FocusedWindow = out;
	}
}

void
window_switch_right() {

	Vec2i point;
	point.x = FocusedWindow->position.x + FocusedWindow->size.w + 10.0f;
	point.y = FocusedWindow->position.y + 10.0f;

	Window* out = find_window_at_point(WinTree, point);
	if (out != NULL)  {
		FocusedWindow = out;
	}
}


void
windows_init(i32 focusedBuffId, i32 commandBuffId) {
  
	WinTree = (Node*)malloc(sizeof(Node));
	array_init(&WinTree->children, 1);

	Window window = window_create_empthy();
	window.size.w = Width;
	window.size.h = Height;
	window.parent = WinTree;
	window.buffId = focusedBuffId;

	array_push(&WinTree->children, window);
	WinTree->isVertical = 0;
	WinTree->nodeType = NODE_CONTAINER;
	WinTree->containerSize.w = window.size.w;
	WinTree->containerSize.h = window.size.h;
	WinTree->parent = NULL;

	FocusedWindow = &WinTree->children[0];
	//FocusedWindow->parent = WinTree;

	CommandWindow = (Window*)malloc(sizeof(Window));
	CommandWindow->position.x = 0;
	CommandWindow->position.y = FocusedWindow->size.h - 200.0f;
	CommandWindow->size.w = FocusedWindow->size.w;
	CommandWindow->size.h = 200.0f;
	CommandWindow->nodeType = NODE_WINDOW;
	CommandWindow->buffId = commandBuffId;

}


static void
resize_windows_evenly_width(Node* parent, i32 xpos, i32 totalWidth, i32 winCount) {
	
	i32 remainder = totalWidth % winCount;
	i32 width = totalWidth / winCount;

	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].position.x = xpos;
			parent->children[i].size.w = width;
			xpos += width;
		}
	}

	for (sizet i = parent->children.length - 1; i >= 0; --i) {

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

	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].position.y = ypos;
			parent->children[i].size.h = height;
			ypos += height;
		}
	}

	for (sizet i = parent->children.length - 1; i >= 0; --i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			parent->children[i].size.h += remainder;
			break;
		}
	}
}

static void
resize_windows_recursively_width(Node* parent, i32 startX, i32 width) {
	
	for (sizet i = 0; i < parent->children.length; ++i) {

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

	for (sizet i = 0; i < parent->children.length; ++i) {

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
	
	for (sizet i = 0; i < parent->children.length; ++i) {

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

	for (sizet i = 0; i < parent->children.length; ++i) {

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

	for (sizet i = 0; i < node->children.length; ++i) {

		if (node->children[i].nodeType == NODE_WINDOW) {
			return &node->children[i];
		}
	}

	for (sizet i = 0; i < node->children.length; ++i) {

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

	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW)
			return &parent->children[i];
	}
	return NULL;
}

static inline Window*
find_window(Node* parent, i32 id) {
	
	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {
			if (parent->children[i].id == id) {
				return &parent->children[i];
			}
		}
		else {
			find_window(&parent->children[i], id);
		}
	}
	return NULL;
}

void
window_close() {

	// check if there is container or window at parent
	if (FocusedWindow->parent->children.length == 1) return;


	if (FocusedWindow->parent->children.length == 2) {
		Node* parent = FocusedWindow->parent;
		Window focusWinData = *FocusedWindow;

		// get the other child
		Node* node;
		if (parent->children[0].id == FocusedWindow->id)
		  node = &parent->children[1];
		else
		  node = &parent->children[0];

		if (node->nodeType == NODE_CONTAINER) {

			Node* childWindow = find_first_window_recursively(node);

			if (parent->isVertical) {
				
				i32 xpos;

				if (parent->children[0].id == FocusedWindow->id)
					array_erase(&parent->children, 0);
				else
					array_erase(&parent->children, 1);

				node = &parent->children[0];

				// if child window is to the right
				if (childWindow->position.x > focusWinData.position.x)
					xpos = focusWinData.position.x;
				else
					xpos = childWindow->position.x;

				resize_windows_recursively_width(node, xpos, focusWinData.size.w);


			} else {
				
				i32 ypos;

				if (parent->children[0].id == FocusedWindow->id)
					array_erase(&parent->children, 0);
				else
					array_erase(&parent->children, 1);

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

				if (parent->children[0].id == FocusedWindow->id)
					array_erase(&parent->children, 0);
				else
					array_erase(&parent->children, 1);

				other = &parent->children[0];

				if (focusWinData.position.x > other->position.x) {
					
					other->size.w += focusWinData.size.w;
				}
				else {
					
					other->position.x = focusWinData.position.x;
					other->size.w += focusWinData.size.w;
				}

				FocusedWindow = other;
			} else {
				
				Window* other;

				if (parent->children[0].id == FocusedWindow->id)
					array_erase(&parent->children, 0);
				else
					array_erase(&parent->children, 1);

				other = &parent->children[0];

				if (focusWinData.position.y > other->position.y) {
					
					other->size.h += focusWinData.size.h;
				}
				else {
					
					other->position.y = focusWinData.position.y;
					other->size.h += focusWinData.size.h;
				}

				FocusedWindow = other;
			}
		}

		if (parent->parent != NULL && parent->children.length == 1) {

			Node* root = parent->parent;
			b8 isContainer = parent->children[0].nodeType == NODE_CONTAINER;

			// move child children to parent children
			// delete parent container
			Array<Node> nodesToMoveUp;
			Array<Node> toDelete;

			if (isContainer) {
				// if its a container we move parent->children->children
				// to root
				nodesToMoveUp = parent->children[0].children;
				toDelete = parent->children;
			}
			else {
				// if not we only move the window parent->children[0]
				nodesToMoveUp = parent->children;
			}


			sizet insertIndex = 0;
			// get parent index
			for (sizet i = 0; i < root->children.length; ++i) {

				if (&root->children[i] == parent) {
					insertIndex = i;
					break;
				}
			}
			// convert parent to 1 window
			root->children[insertIndex] = nodesToMoveUp[0];
			root->children[insertIndex].parent = root;

			if (isContainer) {

				for (sizet i = 1; i < nodesToMoveUp.length; ++i) {

					array_insert(&root->children, nodesToMoveUp[i], insertIndex);
					root->children[insertIndex].parent = root;
					insertIndex++;
				}

				for (sizet i = 0; i < root->children.length; ++i) {

					if (root->children[i].nodeType == NODE_CONTAINER) {

						for (sizet k = 0; k < root->children[i].children.length; ++k)
							root->children[i].children[k].parent = &root->children[i];
					}
				}
			}


			array_free(&nodesToMoveUp);
			if (isContainer)
				array_free(&toDelete);

			FocusedWindow = find_first_window_recursively(root);

		}
		printf("--------- \nTREE _______ \n");
		print_tree(WinTree);

	} else if (FocusedWindow->parent->children.length > 2) {
		// TODO:
		
		/*
		Node* parent = FocusedWindow.parent;
		Window focusWinData = focusedWindow;

		i32 xpos = BIG_NUMBER;
		if (parent->isVertical) {

			for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

				if (parent->children[i].nodeType == NODE_WINDOW) {
					
					if (xpos > parent->children[i].position.x) 
						xpos = parent->children[i].position.x;
				}
			}

			for (sizet i = 0; i < ARRAY_LENGTH(parent->children); ++i) {

				if (parent->children[i].nodeType == NODE_WINDOW) {

					if (parent->children[i].id == focusWinData.id) 
						array_erase(parent->children, i);
				}
			}

		}
		Window* out = find_first_window(parent);
		*/
	}

}



void
window_split_horizontal() {
	
	if (FocusedWindow->size.h / 2 <= MIN_WINDOW_HEIGHT) return;

	Node* parent = FocusedWindow->parent;

	if (parent->isVertical) {
		
		Array<Window> children;
		array_init(&children , 2);
		array_push(&children, *FocusedWindow);

		Vec2i winSize = FocusedWindow->size;
		Node* container = FocusedWindow;
		container->nodeType = NODE_CONTAINER;
		container->isVertical = 0;
		container->children = children;
		container->containerSize = winSize;

		FocusedWindow = &children[0];
		FocusedWindow->parent = container;
		FocusedWindow->size.h /= 2;

		Window newWindow = window_create_empthy();
		newWindow.position.x = FocusedWindow->position.x;
		newWindow.position.y = FocusedWindow->position.y + FocusedWindow->size.h;
		newWindow.size = FocusedWindow->size;
		newWindow.buffId = FocusedWindow->buffId;
		newWindow.parent = container;

		array_push(&container->children, newWindow);

		printf("\n\n TREE \n\n");
		print_tree(WinTree);

		return;
	}

	Window win;
	i32 height = 0;
	i32 ypos = BIG_NUMBER;

	Window* childWindow = find_first_window(parent);
	ASSERT(childWindow);

	win = window_create_empthy();
	win.size.w = childWindow->size.w;
	win.position.x = childWindow->position.x;
	win.size.h = 0;
	win.parent = parent;
	win.buffId = FocusedWindow->buffId;

	i32 winCount = 0;
	i32 focusedwinId = FocusedWindow->id;

	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			winCount++;
			height += parent->children[i].size.h;
			if (ypos > parent->children[i].position.y) {
				ypos = parent->children[i].position.y;
			}
			if (parent->children[i].id == FocusedWindow->id) {
				array_insert(&parent->children, win, i + 1);
				i++;
				winCount++;
			}
		}
	}

	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_CONTAINER) {

			for (sizet k = 0; k < parent->children[i].children.length; ++k)
				parent->children[i].children[k].parent = &parent->children[i];
		}
	}

	FocusedWindow = find_window(parent, focusedwinId);
	ASSERT_MSG(FocusedWindow, "No window with that id in parent->children");

	resize_windows_evenly_height(parent, ypos, height, winCount);

	printf("\n\n TREE \n\n");
	print_tree(WinTree);
}


void
window_split_vertical() {
	
	if (FocusedWindow->size.w / 2 <= MIN_WINDOW_WIDTH) return;

	Node* parent = FocusedWindow->parent;

	if (!parent->isVertical) {

		Array<Window> children;
		array_init(&children, 2);
		array_push(&children, *FocusedWindow);

		Vec2i winSize = FocusedWindow->size;
		Node* container = FocusedWindow;
		container->nodeType = NODE_CONTAINER;
		container->isVertical = 1;
		container->children = children;
		container->containerSize = winSize;

		FocusedWindow = &children[0];
		FocusedWindow->parent = container;
		FocusedWindow->size.w /= 2;

		Window newWindow = window_create_empthy();
		newWindow.position.x = FocusedWindow->size.w + FocusedWindow->position.x;
		newWindow.position.y = FocusedWindow->position.y;
		newWindow.size = FocusedWindow->size;
		newWindow.buffId = FocusedWindow->buffId;
		newWindow.parent = container;

		array_push(&container->children, newWindow);

		printf("\n\n TREE \n\n");
		print_tree(WinTree);

		return;
	}

	Window win;
	i32 width = 0;
	i32 xpos = BIG_NUMBER;

	Window* childWindow = find_first_window(parent);
	ASSERT(childWindow);

	win = window_create_empthy();
	win.size.w = 0;
	win.size.h = childWindow->size.h;
	win.position.y = childWindow->position.y;
	win.parent = parent;
	win.buffId = FocusedWindow->buffId;

	i32 winCount = 0;
	i32 focusedwinId = FocusedWindow->id;

	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_WINDOW) {

			winCount++;
			width += parent->children[i].size.w;
			if (xpos > parent->children[i].position.x) {
				xpos = parent->children[i].position.x;
			}
			if (parent->children[i].id == FocusedWindow->id) {
				array_insert(&parent->children, win, i + 1);
				i++;
				winCount++;
			}
			
		}
	}

	for (sizet i = 0; i < parent->children.length; ++i) {

		if (parent->children[i].nodeType == NODE_CONTAINER) {

			for (sizet k = 0; k < parent->children[i].children.length; ++k)
				parent->children[i].children[k].parent = &parent->children[i];
		}
	}

	FocusedWindow = find_window(parent, focusedwinId);
	ASSERT_MSG(FocusedWindow, "No window with that id in parent->children");

	resize_windows_evenly_width(parent, xpos, width, winCount);

	printf("\n\n TREE \n\n");
	print_tree(WinTree);

}

static void
tree_get_windows(Node* node, Array<Window>* windows) {

	for (sizet i = 0; i < node->children.length; ++i) {
		
		if (node->children[i].nodeType == NODE_WINDOW) 
			array_push(windows, node->children[i]);
		else 
			tree_get_windows(&node->children[i], windows);
	}

}

void
windows_get_all(Array<Window>* windows) {

	array_init(windows, 10);

	tree_get_windows(WinTree, windows);
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
	printf("Children count : %zu \n", node->children.length);
	printf("Size : ");
	vec2i_print(node->containerSize);

	for (sizet i = 0; i < node->children.length; ++i)  {

		if (node->children[i].nodeType == NODE_WINDOW) {
			printf("\n");
			printf("---- Window ----\n");
			printf("Adress : %p \n", &node->children[i]);
			printf("Parent : %p \n", node->children[i].parent);
			printf("Buffer id : %p \n", node->children[i].buffId);
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
