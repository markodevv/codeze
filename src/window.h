#pragma once
#include "math.h"
#include "types.h"

typedef enum WinDirection {

	WIN_UP,
	WIN_DOWN,
	WIN_RIGHT,
	WIN_LEFT,
	WIN_DIRECTIONS

} WinDirection;

typedef enum NodeType {

	NODE_WINDOW,
	NODE_CONTAINER
	
} NodeType;

typedef struct Node {
  
	union {
		struct {
			i32 id;
			Vec2i renderView;
			Vec2i position;
			Vec2i size;
		};
		struct {
			b8 isVertical;
			Vec2i containerSize;
			struct Node* children;
		};
	};
	NodeType nodeType;
	struct Node* parent;

} Node;


typedef Node* NodeTree;
typedef Node Window;
typedef Node* WindowArray;

struct Buffer;

Window* window_split_vertical(NodeTree tree, Window* focusedWindow);
Window* window_split_horizontal(NodeTree tree, Window* focusedWindow);
Window* window_switch(NodeTree tree, struct Buffer* buf, Window* focusedWindow, WinDirection dir);
Window* window_close(NodeTree tree, Window* focusedWindow);
Window empthy_window();
NodeTree window_tree_create(Window window);
WindowArray tree_get_windows(Node* node, WindowArray windows);
void print_tree(Node* node);
i32 new_window_id();
/*
WindowArray window_split_horizontaly(WindowArray windows, i32 winId);
i32 window_switch(struct Buffer* buf, WindowArray windows, i32 winId, WinDirection dir);
i32 window_close(struct Buffer* buf, WindowArray windows, i32 winId);
*/
