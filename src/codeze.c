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

   - mouse input

 */

/* DONE:

   - gap buffer
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


static void
initialize_window(Window* window, f32 width, f32 height, f32 x, f32 y) {
	
	window->position.x = x;
	window->position.y = y;
	window->size.x = width;
	window->size.y = height;
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

	Window* windows = array_create(4, sizeof(Window));
	for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {

		windows[i].active = 0;
	}

	i32 windowFocused = 0;
	{
		Window window = {1, {0.0f, 0.0f}, {editor->width, editor->height}};
		windows = array_push(windows, &window);
	}


	while (!glfwWindowShouldClose(editor->window)) {
		
		Event event;
		while(event_queue_next(&event)) { 
			if (event.type == KEY_PRESSED || event.type == KEY_REPEAT) {
				switch(event.key) {
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
				case KEY_S:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						Window window = {
							1,
							{windows[windowFocused].size.x / 2, 0.0f},
							{windows[windowFocused].size.x / 2, windows[windowFocused].size.y}
						};
						windows[windowFocused].size.x /= 2;

						windows = array_push(windows, &window);
					}
					break;
				case KEY_L:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						
					}
					break;
				}
				
			}
			else if (event.type == CHAR_INPUTED) {

				buffer_insert_char(buffer, event.character);
			}
			else if (event.type == MOUSE_BUTTON_PRESSED) {

				f64 xpos, ypos;
				glfwGetCursorPos(editor->window, &xpos, &ypos);

				i32 mouseY = (i32)ypos;
				i32 mouseX = (i32)xpos;

				mouseY -= (mouseY % renderer->fontSize);

				while (mouseY > buffer->currentLine * renderer->fontSize) {

					buffer_cursor_down(buffer);
				}
				while (mouseY < buffer->currentLine * renderer->fontSize) {

					buffer_cursor_up(buffer);
				}


				string lineStr = buffer_string_before_cursor(buffer);

				i32 cursorAdvanceX = 0;

				for (sizet i = 0; i < STR_LENGTH(lineStr); ++i) {
					
					cursorAdvanceX += renderer->glyphs[lineStr[i]].advanceX;
				}

				if (cursorAdvanceX > mouseX) {
					while (cursorAdvanceX > mouseX) {
						//printf("curosrX %i, mouseX %i, \n", cursorAdvanceX, mouseX);
						cursorAdvanceX -=
							renderer->glyphs[buffer_char_before_cursor(buffer)].advanceX;
						buffer_cursor_previous(buffer);
					}
				}
				else if (cursorAdvanceX < mouseX){
					while (cursorAdvanceX < mouseX &&
						   (mouseX - cursorAdvanceX) >=
						   renderer->glyphs[buffer_char_before_cursor(buffer)].advanceX) {
						
						cursorAdvanceX +=
							renderer->glyphs[buffer_char_before_cursor(buffer)].advanceX;
						buffer_cursor_next(buffer);
					}
						   
				}


				str_release(lineStr);
			}
			else if	(event.type == WINDOW_RESIZED) {
				editor->width = event.width;
				editor->height = event.height;
				mat_ortho(renderer->projection, 0.0f, event.width, event.height, 0.0f); break;
				windows[windowFocused].position.x = 0.0f;
				windows[windowFocused].position.y = 0.0f;
				windows[windowFocused].size.x = editor->width / 2;
				windows[windowFocused].size.y = editor->height;
			}
		} 


		renderer_begin(renderer);

		string bufferText = buffer_get_text(buffer);
		Vec2 cursorLine = buffer_get_cursor_line_vec(buffer);
		Vec2 cursorAdvance;

		for (sizet i = cursorLine.start; i < cursorLine.end; ++i) {

			cursorAdvance.x += renderer->glyphs[buffer->text[i]].advanceX;
		}
		cursorAdvance.y = renderer->fontSize * buffer->currentLine + renderer->fontSize / 5;

		TokenArray tokens = tokens_make(bufferText);

		for (i8 i = 0; i < ARRAY_LENGTH(windows); ++i) {
			
			render_buffer(renderer, buffer, &windows[i], tokens);
		}

		array_release(tokens);

		DEBUG_TEXT(renderer, 0, "- DEBUG TEXT -", NULL);
		DEBUG_TEXT(renderer, 1, "cursorX %i", (i32)buffer->cursorX);
		DEBUG_TEXT(renderer, 2, "current line %i", (i32)buffer->currentLine);
		DEBUG_TEXT(renderer, 3, "pre length %i", (i32)buffer->preLen);
		DEBUG_TEXT(renderer, 4, "post length %i", (i32)buffer->postLen);
		DEBUG_TEXT(renderer, 5, "gap length %i", (i32)buffer->gapLen);
		DEBUG_TEXT(renderer, 6, "line length %i", (i32)buffer->lineLengths[buffer->currentLine]);
		DEBUG_TEXT(renderer, 7, "char under cursor %c", (i32)buffer->text[buffer->preLen + buffer->gapLen]);
		DEBUG_TEXT(renderer, 8, "line count %i", (i32)ARRAY_LENGTH(buffer->lineLengths));

		renderer_end(renderer);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
