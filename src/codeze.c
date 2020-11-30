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
	i16 width;
	i16 height;

} Editor;


void
print_array(i32* arr) {
	
	for (sizet i = 0; i < ARRAY_LENGTH(arr); ++i) {
		
		printf("%i, ", arr[i]);
	}
	printf("\n");
}

int
main() {
  
	// i32* arr = array_create(10, sizeof(i32));

	// for (sizet i = 0; i < 10; ++i) {
		
	// 	arr = array_push(arr, &i);
	// }
	// print_array(arr);

	// int num = 69;
	// arr = array_insert(arr, 5, &num);
	// print_array(arr);
	// arr = array_insert(arr, 5, &num);
	// print_array(arr);
	// num = 420;
	// arr = array_insert(arr, 7, &num);
	// print_array(arr);

	// return 0;
	Editor* editor = malloc(sizeof(Editor));
	editor->width = 1920;
	editor->height = 1080;

	Event* event = malloc(sizeof(Event));
	Renderer* renderer = malloc(sizeof(Renderer));

	editor->window = renderer_create_window();
	events_initialize(editor->window);
	renderer_initialize(renderer, editor->width, editor->height);
	renderer_load_font(renderer, "assets/CONSOLA.TTF", 18);


	Vec2 pos = {};
	Vec4 cursorColor = {1.0f, 1.0f, 1.0f, 1.0f};

	File* file = file_open("test.c", "r");

	Buffer* buffer = buffer_create(file);

	file_close(file);

	Window* windows = array_create(2, sizeof(Window));
	i32 activeWindow = 0;
	windows[activeWindow].position.x = 0.0f;
	windows[activeWindow].position.y = 0.0f;
	windows[activeWindow].size.x = editor->width / 2;
	windows[activeWindow].size.y = editor->height;


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
				case KEY_Backspace:
					buffer_backspace_delete(buffer); break;
				case KEY_Tab:
					buffer_insert_tab(buffer); break;
				case KEY_Enter:
					buffer_insert_newline(buffer); break;
				}

			}
			else if (event->type == CHAR_INPUTED) {
				buffer_insert_char(buffer, event->character);
			}
			else if	(event->type == WINDOW_RESIZED) {
				editor->width = event->width;
				editor->height = event->height;
				ortho(renderer->projection, 0.0f, event->width, event->height, 0.0f); break;
				windows[activeWindow].position.x = 0.0f;
				windows[activeWindow].position.y = 0.0f;
				windows[activeWindow].size.x = editor->width / 2;
				windows[activeWindow].size.y = editor->height;
			}
		} 

		renderer_begin(renderer);

		TokenArray tokens = tokens_make(buffer_get_text(buffer));

		render_buffer(renderer, buffer, &windows[activeWindow], tokens);

		array_release(tokens);

		DEBUG_TEXT(renderer, 0, "- DEBUG TEXT -", NULL);
		DEBUG_TEXT(renderer, 1, "cursorX %i", buffer->cursorX);
		DEBUG_TEXT(renderer, 2, "current line %i", buffer->currentLine);
		DEBUG_TEXT(renderer, 3, "pre length %i", buffer->preLen);
		DEBUG_TEXT(renderer, 4, "post length %i", buffer->postLen);
		DEBUG_TEXT(renderer, 5, "gap length %i", buffer->gapLen);
		DEBUG_TEXT(renderer, 6, "line length %i", buffer->lineLengths[buffer->currentLine]);
		DEBUG_TEXT(renderer, 7, "char under cursor %c", buffer->text[buffer->preLen + buffer->gapLen]);
		DEBUG_TEXT(renderer, 8, "line count %i", ARRAY_LENGTH(buffer->lineLengths));

		renderer_end(renderer);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
