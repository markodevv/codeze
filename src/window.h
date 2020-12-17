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
			i32 buffId;
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


typedef Node WindowTree;
typedef Node Window;
typedef Node* WindowArray;

extern Window* FocusedWindow;
extern WindowTree* WinTree;


void window_split_vertical();
void window_split_horizontal();
void window_switch(WinDirection dir);
void window_close();
Window window_create_empthy();
void window_tree_create(Window window);
WindowArray tree_get_windows(Node* tree, WindowArray windows);
void print_tree(Node* node);
i32 new_window_id();
