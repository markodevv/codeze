#pragma once
#include "types.h"

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

#define MOD_SHIFT  0x0001
#define MOD_CONTROL  0x0002
#define MOD_ALT  0x0004
#define MOD_SUPER  0x0008

typedef enum KeyCode
{
	// From glfw3.h
	KEY_Space               = 32,
	KEY_Apostrophe          = 39, /* ' */
	KEY_Comma               = 44, /* , */
	KEY_Minus               = 45, /* - */
	KEY_Period              = 46, /* . */
	KEY_Slash               = 47, /* / */

	KEY_D0                  = 48, /* 0 */
	KEY_D1                  = 49, /* 1 */
	KEY_D2                  = 50, /* 2 */
	KEY_D3                  = 51, /* 3 */
	KEY_D4                  = 52, /* 4 */
	KEY_D5                  = 53, /* 5 */
	KEY_D6                  = 54, /* 6 */
	KEY_D7                  = 55, /* 7 */
	KEY_D8                  = 56, /* 8 */
	KEY_D9                  = 57, /* 9 */

	KEY_Semicolon           = 59, /* ; */
	KEY_Equal               = 61, /* = */

	KEY_A                   = 65,
	KEY_B                   = 66,
	KEY_C                   = 67,
	KEY_D                   = 68,
	KEY_E                   = 69,
	KEY_F                   = 70,
	KEY_G                   = 71,
	KEY_H                   = 72,
	KEY_I                   = 73,
	KEY_J                   = 74,
	KEY_K                   = 75,
	KEY_L                   = 76,
	KEY_M                   = 77,
	KEY_N                   = 78,
	KEY_O                   = 79,
	KEY_P                   = 80,
	KEY_Q                   = 81,
	KEY_R                   = 82,
	KEY_S                   = 83,
	KEY_T                   = 84,
	KEY_U                   = 85,
	KEY_V                   = 86,
	KEY_W                   = 87,
	KEY_X                   = 88,
	KEY_Y                   = 89,
	KEY_Z                   = 90,

	KEY_LeftBracket         = 91,  /* [ */
	KEY_Backslash           = 92,  /* \ */
	KEY_RightBracket        = 93,  /* ] */
	KEY_Tilde         = 96,  /* ` */

	KEY_World1              = 161, /* non-US #1 */
	KEY_World2              = 162, /* non-US #2 */

	/* Function keys */
	KEY_Escape              = 256,
	KEY_Enter               = 257,
	KEY_Tab                 = 258,
	KEY_Backspace           = 259,
	KEY_Insert              = 260,
	KEY_Delete              = 261,
	KEY_Right               = 262,
	KEY_Left                = 263,
	KEY_Down                = 264,
	KEY_Up                  = 265,
	KEY_PageUp              = 266,
	KEY_PageDown            = 267,
	KEY_Home                = 268,
	KEY_End                 = 269,
	KEY_CapsLock            = 280,
	KEY_ScrollLock          = 281,
	KEY_NumLock             = 282,
	KEY_PrintScreen         = 283,
	KEY_Pause               = 284,
	KEY_F1                  = 290,
	KEY_F2                  = 291,
	KEY_F3                  = 292,
	KEY_F4                  = 293,
	KEY_F5                  = 294,
	KEY_F6                  = 295,
	KEY_F7                  = 296,
	KEY_F8                  = 297,
	KEY_F9                  = 298,
	KEY_F10                 = 299,
	KEY_F11                 = 300,
	KEY_F12                 = 301,
	KEY_F13                 = 302,
	KEY_F14                 = 303,
	KEY_F15                 = 304,
	KEY_F16                 = 305,
	KEY_F17                 = 306,
	KEY_F18                 = 307,
	KEY_F19                 = 308,
	KEY_F20                 = 309,
	KEY_F21                 = 310,
	KEY_F22                 = 311,
	KEY_F23                 = 312,
	KEY_F24                 = 313,
	KEY_F25                 = 314,

	/* Keypad */
	KEY_KP0                 = 320,
	KEY_KP1                 = 321,
	KEY_KP2                 = 322,
	KEY_KP3                 = 323,
	KEY_KP4                 = 324,
	KEY_KP5                 = 325,
	KEY_KP6                 = 326,
	KEY_KP7                 = 327,
	KEY_KP8                 = 328,
	KEY_KP9                 = 329,
	KEY_KPDecimal           = 330,
	KEY_KPDivide            = 331,
	KEY_KPMultiply          = 332,
	KEY_KPSubtract          = 333,
	KEY_KPAdd               = 334,
	KEY_KPEnter             = 335,
	KEY_KPEqual             = 336,

	KEY_LeftShift           = 340,
	KEY_LeftControl         = 341,
	KEY_LeftAlt             = 342,
	KEY_LeftSuper           = 343,
	KEY_RightShift          = 344,
	KEY_RightControl        = 345,
	KEY_RightAlt            = 346,
	KEY_RightSuper          = 347,
	KEY_Menu                = 348
} KeyCode;
