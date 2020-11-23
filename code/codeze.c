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
	Vec2 pos1 = {100.0f, 100.0f};
	Vec2 pos2 = {200.0f, 200.0f};
	Vec2 pos3 = {300.0f, 300.0f};
	Vec2 pos4 = {400.0f, 400.0f};
	Vec2 size = {100.0f, 100.0f};
	Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	Vec4 color1 = {1.0f, 1.0f, 0.0f, 1.0f};
	Vec4 color2 = {0.0f, 1.0f, 0.0f, 1.0f};



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
		render_quad(renderer, pos1, size, color1);
		render_textured_quad(renderer, pos2, size, WHITE_TEXTURE_INDEX);
		render_textured_quad(renderer, pos3, size, GREEN_TEXTURE_INDEX);
		render_textured_quad(renderer, pos4, size, GREEN_TEXTURE_INDEX);

		renderer_end(renderer);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
