#include "renderer.h"
#include "string.h"
#include "event.h"
#include "debug.h"
#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>

/* TODO:

   - string struct
   - fix offscreen rendering

 */

/* DONE:

   - Font rendering

 */
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
	renderer_load_font(renderer, "assets/CONSOLA.TTF", 32);

	String* str = str_create_char("hello world");
	str_print(str);
	str_concat(str, ", and merry christmass");
	str_print(str);
	str_concat(str, ", and merry christmass");
	str_print(str);
	printf("%c \n", str_at(str, 10));


	Vec2 pos = {};
	Vec2 pos2 = {0.0f, 32.0f};
	Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};



	while (!glfwWindowShouldClose(editor->window)) {
		
		while(event_queue_next(event)) { 
			switch(event->type) { 
		 		case KEY_PRESSED: 
					break;
				case WINDOW_RESIZED:
					ortho(renderer->projection, 0.0f, event->width, event->height, 0.0f);
					break;
				}
		} 

		renderer_begin(renderer);

		const char* text = "hello, world";
		int len = 0;
		while (text[len] != '\0') {
			len++;
		}

		render_text(renderer, text, len, pos, color);
		render_text(renderer, text, len, pos2, color);
		render_quad(renderer, pos, pos, color);

		renderer_end(renderer);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
