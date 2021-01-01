#pragma once
#include "types.h"
#include "key.h"
#include <GLFW/glfw3.h>

typedef enum EventType {

	KEY_PRESSED,
	KEY_RELEASED,
	KEY_REPEAT,
	CHAR_INPUTED,
	MOUSE_BUTTON_PRESSED,
	MOUSE_BUTTON_RELEASED,
	MOUSE_MOVED,
	MOUSE_SCROLLED,
	WINDOW_RESIZED,
	WINDOW_MOVED,
	WINDOW_GAIN_FOCUS,
	WINDOW_LOST_FOCUS

} EventType;

typedef struct Event {

	EventType type;
	union {
		struct {
			int x, y;
		};
		struct {
			int width, height;
		};
		struct {
			int offsetX, offsetY;
		};
		struct {
			int key, mods;
		};

		int button;
		char character;
	};

} Event;

typedef struct EventQueue {
	struct Event* events;
	struct Event* tail;
	struct Event* head;
} EventQueue;


typedef enum MouseButton {
	MOUSE_LEFT = 0,
    MOUSE_RIGHT = 1, 
	MOUSE_MIDDLE = 2,
	MOUSE_BUTTON01 = MOUSE_LEFT,
	MOUSE_BUTTON02 = MOUSE_RIGHT,
	MOUSE_BUTTON03 = MOUSE_MIDDLE,
	MOUSE_BUTTON04 = 4,
	MOUSE_BUTTON05 = 5,
	MOUSE_BUTTON06 = 6,
	MOUSE_BUTTON07 = 7,
	MOUSE_BUTTON08 = 8,
} MouseButton;

void events_initialize(GLFWwindow* window);
i32 event_queue_next(Event* event);


