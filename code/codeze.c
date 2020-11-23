#include "codeze.h"
#include "renderer.h"
#include "event.h"
#include "debug.h"
#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>


typedef struct Editor {

	GLFWwindow* window;

} Editor;

int main() {
  
	Editor* editor = malloc(sizeof(Editor));
	Event* event = malloc(sizeof(Event));
	Renderer* renderer = malloc(sizeof(Renderer));

	editor->window = renderer_create_window();
	events_initialize(editor->window);
	renderer_initialize(renderer, 1280, 768);


	Vec2 pos = {};
	Vec2 size = {100.0f, 100.0f};
	Vec4 color = {0.0f, 1.0f, 1.0f, 1.0f};



	while (!glfwWindowShouldClose(editor->window)) {
		
		/* while(event_queue_next(event)) { */
		/* 	switch(event->type) { */
		/* 		case KeyPressed: */
		/* 			printf("key is pressed \n"); */
		/* 			break; */
		/* 	} */
		/* } */

		renderer_begin(renderer);

		render_quad(renderer, pos, size, color);

		renderer_end(renderer);

		glfwSwapBuffers(editor->window);

		glfwPollEvents();

	}

	return 0;
}
