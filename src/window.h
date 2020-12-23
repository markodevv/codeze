#pragma once
#include "math.h"
#include "types.h"
#include "container.h"

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
			struct Array<Node> children;
		};
	};
	NodeType nodeType;
	struct Node* parent;

} Node;


typedef Node WindowTree;
typedef Node Window;
typedef Node* WindowArray;


void window_split_vertical();
void window_split_horizontal();
void window_switch_up();
void window_switch_down();
void window_switch_left();
void window_switch_right();
void window_close();
Window window_create_empthy();
void windows_init(i32 focusedBuffId, i32 commandBuffId);
void windows_get_all(Array<Window>* windows);
void print_tree(Node* node);
i32 new_window_id();
