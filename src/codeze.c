#include "renderer.h"
#include "my_string.h"
#include "event.h"
#include "debug.h"
#include "fileio.h"
#include "tokenizer.h"
#include "container.h"
#include "buffer.h"
#include "cursor.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

/* TODO:
   - closing windows
   - resizing windws

 */

/* DONE:

   - window scrolling
   - split window feature
   - mouse input basics
   - gap buffer basics
   - syntax highliting basics
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


static void
initialize_window(Window* window, f32 width, f32 height, f32 x, f32 y) {
	
	window->position.x = x;
	window->position.y = y;
	window->size.x = width;
	window->size.y = height;
}
	

int main() {

	Editor* editor = malloc(sizeof(Editor));
	editor->width = 1916;
	editor->height = 1041;

	editor->window = renderer_create_window();
	events_initialize(editor->window);
	renderer_initialize(editor->width, editor->height);
	renderer_load_font("assets/CONSOLA.TTF", 18);


	Vec2 pos = {};
	Vec4 cursorColor = {1.0f, 1.0f, 1.0f, 1.0f};

	File* file = file_open("test.c", "r");

	Buffer* buffer = buffer_create(file);

	file_close(file);

	Window window;
	window.renderView.start = 0;
	window.renderView.end = 0;
	window.nodeType = NODE_WINDOW;
	window.position.x = 0;
	window.position.y = 0;
	window.id = new_window_id();
	window.size.w = editor->width;
	window.size.h = editor->height;
	NodeTree windowTree = window_tree_create(window);
	Window* focusedWindow = windowTree->children + 0;


	while (!glfwWindowShouldClose(editor->window)) {
		
		Event event;
		while(event_queue_next(&event)) { 
			// mabey keys should be commands instead and commands are bound to keys
			if (event.type == KEY_PRESSED || event.type == KEY_REPEAT) {
				switch(event.key) {
				case KEY_Left:
					cursor_left(buffer); break;
				case KEY_Right:
					cursor_right(buffer); break;
					/*
				case KEY_Down:
					if (buffer->currentLine <
						windows[focusedWindow].renderView.end) {
						
						cursor_down(buffer); 
					} break;
				case KEY_Up:
					if (buffer->currentLine >
						windows[focusedWindow].renderView.start) {
						
						cursor_up(buffer);
					} break;
					*/
				case KEY_Backspace:
					buffer_backspace_delete(buffer); break;
				case KEY_Tab:
					buffer_insert_tab(buffer); break;
				case KEY_Enter:
					buffer_insert_newline(buffer); break;
				case KEY_V:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {

						focusedWindow = window_split_vertical(windowTree, focusedWindow);
					} break;
				case KEY_S:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {

						focusedWindow = window_split_horizontal(windowTree, focusedWindow);
					} break;
				case KEY_Q:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						focusedWindow = window_close(focusedWindow);
					} break;
				case KEY_L:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {

						focusedWindow =
							window_switch(windowTree, buffer, focusedWindow, WIN_RIGHT);
					} break;
				case KEY_H:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						focusedWindow =
							window_switch(windowTree, buffer, focusedWindow, WIN_LEFT);
					} break;
				case KEY_J:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						focusedWindow =
							window_switch(windowTree, buffer, focusedWindow, WIN_DOWN);
					} break;
				case KEY_K:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						focusedWindow =
							window_switch(windowTree, buffer, focusedWindow, WIN_UP);
					} break;
					/*
				case KEY_J:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {

						focusedWindow = window_switch(buffer, windows, focusedWindow, WIN_DOWN);
					} break;
				case KEY_K:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						focusedWindow = window_switch(buffer, windows, focusedWindow, WIN_UP);
					} break;
				case KEY_S:
					if ((event.mods & MOD_CONTROL) == MOD_CONTROL) {
						
						windows = window_split_horizontaly(windows, focusedWindow);
					} break;
				case KEY_PageDown: {
					if (windows[focusedWindow].renderView.start
						!= ARRAY_LENGTH(buffer->lineLengths) - 1 &&
						windows[focusedWindow].renderView.end <
						ARRAY_LENGTH(buffer->lineLengths)) {

						if (buffer->currentLine == windows[focusedWindow].renderView.start)
							cursor_down(buffer);

						windows[focusedWindow].renderView.start++;
					}
				} break;
				case KEY_PageUp: {
					if (windows[focusedWindow].renderView.start != 0) {
						
						if (buffer->currentLine == windows[focusedWindow].renderView.end)
							cursor_up(buffer);

						windows[focusedWindow].renderView.start--;
					}
				} break;

				}
				*/
			  
			}
			/*
			else if (event.type == CHAR_INPUTED) {

				buffer_insert_char(buffer, event.character);
			}
			//                    TODO:
			else if (event.type == -5) {

				// TODO: make it work for last line 
				f64 xpos, ypos;
				glfwGetCursorPos(editor->window, &xpos, &ypos);

				i32 mouseY = (i32)ypos;
				i32 mouseX = (i32)xpos;

				mouseY -= (mouseY % renderer_font_size());

				while (mouseY > buffer->currentLine * renderer_font_size()) {

					cursor_down(buffer); 
				} 

				while (mouseY < buffer->currentLine * renderer_font_size()) {

					cursor_up(buffer);
				} 


				string lineStr = buffer_string_before_cursor(buffer);

				i32 cursorAdvanceX = 0; 

				for (sizet i = 0; i < STR_LENGTH(lineStr); ++i) {
					
					//cursorAdvanceX += renderer->glyphs[lineStr[i]].advanceX;
				} 

				// if (cursorAdvanceX > mouseX) { 
				// 	while (cursorAdvanceX > mouseX) {
				// 		//cursorAdvanceX -= 0;
				// 			//	renderer->glyphs[buffer_char_before_cursor(buffer)].advanceX;
				// 		buffer_cursor_previous(buffer); 
				//  	} 
				// } 
				// else if (cursorAdvanceX < mouseX){ 
				// 	while (cursorAdvanceX < mouseX && 
				// 	   (mouseX - cursorAdvanceX) >= 
				// 		   //renderer->glyphs[buffer_char_under_cursor(buffer)].advanceX) {
						
				// 		   // cursorAdvanceX += 0;
				// 		   //renderer->glyphs[buffer_char_under_cursor(buffer)].advanceX;
				// 		buffer_cursor_next(buffer); 
				// 	}
					
				// } 

				str_release(lineStr);
			}
			else if	(event.type == WINDOW_RESIZED) {
				editor->width = event.width;
				editor->height = event.height;
				renderer_on_window_resize(event.width, event.height);
				windows[focusedWindow].position.x = 0.0f;
				windows[focusedWindow].position.y = 0.0f;
				windows[focusedWindow].size.w = editor->width;
				windows[focusedWindow].size.h = editor->height;
			*/
			}
		} 


		renderer_begin();


		string bufferText = buffer_get_text(buffer);
		TokenArray tokens = tokens_make(bufferText);

		WindowArray windows = array_create(26, sizeof(Window));
		windows = tree_get_windows(windowTree, windows);

		for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {

			render_buffer(buffer, &windows[i], tokens);
		}
		render_cursor(buffer, focusedWindow);


		Vec2 pos;
		pos.x = editor->width - 200.0f;
		pos.y = editor->height - 333.0f;
		DEBUG_TEXT(pos, "- DEBUG TEXT -", NULL); pos.y += 20.0f;

		DEBUG_TEXT(pos, "cursorX tabed %i", (i32)buffer->cursorXtabed); pos.y += 20.0f;
		DEBUG_TEXT(pos, "cursorX %i", (i32)buffer->curX); pos.y += 20.0f;
		DEBUG_TEXT(pos, "tabed line length %i", (i32)buffer->cursorLines[buffer->currentLine]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "line length %i", (i32)buffer->lineLengths[buffer->currentLine]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "line count %i", (i32)ARRAY_LENGTH(buffer->lineLengths)); pos.y += 20.0f;
		DEBUG_TEXT(pos, "current line %i", (i32)buffer->currentLine); pos.y += 20.0f;
		DEBUG_TEXT(pos, "pre length %i", (i32)buffer->preLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "post length %i", (i32)buffer->postLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "gap length %i", (i32)buffer->gapLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "char under cursor %c", (i32)buffer->text[buffer->preLen + buffer->gapLen]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "Window count %i", ARRAY_LENGTH(windows)); pos.y += 20.0f;

		renderer_end();

		array_release(tokens);
		array_release(windows);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
