#include "renderer.h"
#include "my_string.h"
#include "event.h"
#include "debug.h"
#include "fileio.h"
#include "tokenizer.h"
#include "container.h"

#include <stdio.h>
#include <stdlib.h>
#include <re.h>

/* TODO:

   - tiny up code
   - gap buffer
   - syntax highliting

 */

/* DONE:

   - Font rendering
   - string struct
   - fix offscreen rendering

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
	renderer_load_font(renderer, "assets/CONSOLA.TTF", 20);


	Vec2 pos = {};
	Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	Vec2 ryukSize = {1920.0f, 1080.0f};
	Vec4 ryukColor = {1.0f, 1.0f, 1.0f, 0.5f};

	File* file = file_open("test.c", "r");
	String* textFile = str_create_c(file->buffer);

	file_close(file);

	Token* tokens = lexer_lex(textFile);

									   

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


		render_textured_quad(renderer, pos, ryukSize, ryukColor, RYUK_TEXTURE_INDEX);
		render_text_syntax(renderer, textFile, pos, tokens);

		renderer_end(renderer);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
