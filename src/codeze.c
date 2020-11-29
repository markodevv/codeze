#include "renderer.h"
#include "my_string.h"
#include "event.h"
#include "debug.h"
#include "fileio.h"
#include "tokenizer.h"
#include "container.h"
#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

/* TODO:

   - gap buffer

 */

/* DONE:

   - syntax highliting
   - tiny up code
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
	renderer_load_font(renderer, "assets/CONSOLA.TTF", 18);


	Vec2 pos = {};
	Vec4 cursorColor = {1.0f, 1.0f, 1.0f, 1.0f};

	File* file = file_open("test.c", "r");

	Buffer* buffer = buffer_create(file);
	Token* tokens = tokens_make(file->buffer);


	//buffer_insert_char(buffer, 'A');
	file_close(file);

	//	string* text = buffer_get_text(buffer);

	// thrd_t thread;
	// thrd_t thread2;
	// i32 success = thrd_create(&thread, some_function, "thread1");
	// ASSERT(success == thrd_success);

	// i32 result;
	// thrd_join(thread, &result);
	// printf("\n");


	while (!glfwWindowShouldClose(editor->window)) {
		
		while(event_queue_next(event)) { 
			if (event->type == KEY_PRESSED || event->type == KEY_REPEAT) {
				switch(event->key) {
				case KEY_Left:
					buffer_cursor_previous(buffer); break;
				case KEY_Right:
					buffer_cursor_next(buffer); break;
				case KEY_Down:
					buffer_cursor_down(buffer); break;
				case KEY_Up:
					buffer_cursor_up(buffer); break;
				}
			}
			else if (event->type == CHAR_INPUTED) {
				buffer_insert_char(buffer, event->character);
			}
			else if	(event->type == WINDOW_RESIZED) {
				ortho(renderer->projection, 0.0f, event->width, event->height, 0.0f); break;
			}
		} 

		renderer_begin(renderer);


		render_buffer(renderer, buffer, pos, tokens);

		DEBUG_TEXT(renderer, 0, "- DEBUG TEXT -", NULL);
		DEBUG_TEXT(renderer, 1, "cursorX %i", buffer->cursorX);
		DEBUG_TEXT(renderer, 2, "current line %i", buffer->currentLine);
		DEBUG_TEXT(renderer, 3, "pre length %i", buffer->preLen);
		DEBUG_TEXT(renderer, 4, "post length %i", buffer->postLen);
		DEBUG_TEXT(renderer, 5, "line length %i", buffer->lineLengths[buffer->currentLine]);
		DEBUG_TEXT(renderer, 6, "char under cursor %c", buffer->text[buffer->preLen]);

		renderer_end(renderer);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
