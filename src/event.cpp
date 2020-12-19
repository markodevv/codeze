#include "event.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>



static EventQueue gEventQueue;
#define EVENT_QUEUE_CAPACITY 1024


static void
cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

	gEventQueue.tail->type = MOUSE_MOVED;
	gEventQueue.tail->x = xpos;
	gEventQueue.tail->y = ypos;
	gEventQueue.tail++;

}

static void
mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	switch(action) {
		case GLFW_PRESS:
			gEventQueue.tail->type = MOUSE_BUTTON_PRESSED;
			gEventQueue.tail->button = button;
			gEventQueue.tail->mods = mods;
			gEventQueue.tail++;
			break;
		case GLFW_RELEASE:
			gEventQueue.tail->type = MOUSE_BUTTON_RELEASED;
			gEventQueue.tail->button = button;
			gEventQueue.tail->mods = mods;
			gEventQueue.tail++;
			break;

	}

}

static void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	
	gEventQueue.tail->type = MOUSE_SCROLLED;
	gEventQueue.tail->offsetX = xoffset;
	gEventQueue.tail->offsetY = yoffset;
	gEventQueue.tail++;

}

static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

	switch (action) {
		case GLFW_PRESS: {
			gEventQueue.tail->type = KEY_PRESSED;
			gEventQueue.tail->mods = mods;
			gEventQueue.tail->key = key;
			gEventQueue.tail++;
			break;
		}
		case GLFW_RELEASE: {
			gEventQueue.tail->type = KEY_RELEASED;
			gEventQueue.tail->mods = mods;
			gEventQueue.tail->key = key;
			gEventQueue.tail++;
			break;
		}
		case GLFW_REPEAT: {
			gEventQueue.tail->type = KEY_REPEAT;
			gEventQueue.tail->mods = mods;
			gEventQueue.tail->key = key;
			gEventQueue.tail++;
			break;
		}
		case GLFW_KEY_UNKNOWN:
			printf("Unknown key %i \n", key);
			break;
	}
}

static void
char_callback(GLFWwindow *window, unsigned int c) {

	gEventQueue.tail->type = CHAR_INPUTED;
	gEventQueue.tail->character = c;
	gEventQueue.tail++;
}

static void
framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

}

static void
window_resize_callback(GLFWwindow* window, int width, int height) {

	gEventQueue.tail->type = WINDOW_RESIZED;
	gEventQueue.tail->width = width;
	gEventQueue.tail->height = height;
	gEventQueue.tail++;
	
}

void
events_initialize(GLFWwindow* window) {

	gEventQueue.events = (Event*)malloc(sizeof(Event) * EVENT_QUEUE_CAPACITY);
	gEventQueue.tail = gEventQueue.events;
	gEventQueue.head = gEventQueue.events;

	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, char_callback);
	glfwSetWindowSizeCallback(window, window_resize_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


}

i32
event_queue_next(Event* event) {

	if (gEventQueue.head == gEventQueue.tail) {
		gEventQueue.head = gEventQueue.events;
		gEventQueue.tail = gEventQueue.events;
		return 0;
	}
	*event = *gEventQueue.head;
	gEventQueue.head++;
	return 1;

}
