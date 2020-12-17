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
#include "dispatch_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

/* TODO:

   - closing windows
   - undo redo
   - file opening/closing
   - switching buffers
 */

/* DONE:

   - window scrolling
   - split window feature
   - mouse input basics
   - gap buffer basics
   - syntax highliting basics
   - tidy up code
   - Font rendering
   - string struct
   - fix offscreen rendering
 */

typedef enum EditorState {

	STATE_NORMAL,
	STATE_COMMAND,
	
} EditorState;

typedef struct Editor {

	GLFWwindow* window;
	i16 width;
	i16 height;
	EditorState state;
  

} Editor;

static void
initialize_window(Window* window, f32 width, f32 height, f32 x, f32 y) {
	
	window->position.x = x;
	window->position.y = y;
	window->size.x = width;
	window->size.y = height;
}


	

int main() {
	
	Command commandmap[KEYMAP_SIZE];

	Editor* editor = malloc(sizeof(Editor));
	editor->width = 1916;
	editor->height = 1041;
	editor->state = STATE_NORMAL;

	editor->window = renderer_create_window();
	events_initialize(editor->window);
	renderer_initialize(editor->width, editor->height);
	renderer_load_font("assets/CONSOLA.TTF", 18);

	File* file = file_open("test.c", "r");

	CurBuffer = malloc(sizeof(Buffer));
	Buffer* buffers = array_create(1, sizeof(Buffer));
	Buffer b = buffer_create(file);
	buffers = array_push(buffers, &b);
	CurBuffer = &buffers[0];

	file_close(file);

	Window window = window_create_empthy();
	window.buffId = 0;
	window.size.w = editor->width;
	window.size.h = editor->height;
	window_tree_create(window);

	dt_initialize();
	dt_bind(cursor_down, KEY_Down, 0);
	dt_bind(cursor_up, KEY_Up, 0);
	dt_bind(cursor_left, KEY_Left, 0);
	dt_bind(cursor_right, KEY_Right, 0);
	dt_bind(buffer_insert_newline, KEY_Enter, 0);
	dt_bind(buffer_insert_tab, KEY_Tab, 0);
	dt_bind(buffer_backspace_delete, KEY_Backspace, 0);
	dt_bind(window_split_horizontal, KEY_H, MOD_CONTROL);
	dt_bind(window_split_vertical, KEY_V, MOD_CONTROL);
	dt_bind(window_close, KEY_Q, MOD_CONTROL | MOD_ALT);

	while (!glfwWindowShouldClose(editor->window)) {
		
		Event event;
		while(event_queue_next(&event)) { 

			if (event.type == KEY_PRESSED) {

				dt_dispatch(event.key, event.mods);
			}
			else if (event.type == KEY_REPEAT) {
				
				dt_dispatch(event.key, event.mods);
			}
			else if (event.type == CHAR_INPUTED) {

				buffer_insert_char(event.character);
			}
			//                    TODO:
			else if (event.type == -5) {
				/*

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

				  if (cursorAdvanceX > mouseX) { 
				  	while (cursorAdvanceX > mouseX) {
				  		//cursorAdvanceX -= 0;
				  			//	renderer->glyphs[buffer_char_before_cursor(buffer)].advanceX;
				  		buffer_cursor_previous(buffer); 
				   	} 
				  } 
				  else if (cursorAdvanceX < mouseX){ 
				  	while (cursorAdvanceX < mouseX && 
				  	   (mouseX - cursorAdvanceX) >= 
				  		   //renderer->glyphs[buffer_char_under_cursor(buffer)].advanceX) {
				
				  		   // cursorAdvanceX += 0;
				  		   //renderer->glyphs[buffer_char_under_cursor(buffer)].advanceX;
				  		buffer_cursor_next(buffer); 
				  	}
				
				  } 

				 str_release(lineStr);
			*/
			}
			else if	(event.type == WINDOW_RESIZED) {

				editor->width = event.width;
				editor->height = event.height;
				renderer_on_window_resize(event.width, event.height);
				FocusedWindow->position.x = 0.0f;
				FocusedWindow->position.y = 0.0f;
				FocusedWindow->size.w = editor->width;
				FocusedWindow->size.h = editor->height;
			}
		} 


		renderer_begin();


		string bufferText = buffer_get_text(&buffers[0]);
		TokenArray tokens = tokens_make(bufferText);

		WindowArray windows = array_create(26, sizeof(Window));
		windows = tree_get_windows(WinTree, windows);

		for (sizet i = 0; i < ARRAY_LENGTH(windows); ++i) {

			b8 focused = 0;
			if (windows[i].id == FocusedWindow->id) {
				focused = 1;
			}
			render_buffer(&buffers[windows[i].buffId], &windows[i], tokens, focused);
			focused = 0;
		}
		render_cursor(CurBuffer, FocusedWindow);


		Vec2 pos;
		pos.x = editor->width - 200.0f;
		pos.y = editor->height - 333.0f;
		DEBUG_TEXT(pos, "- DEBUG TEXT -", NULL); pos.y += 20.0f;

		DEBUG_TEXT(pos, "cursorX tabed %i", (i32)CurBuffer->cursorXtabed); pos.y += 20.0f;
		DEBUG_TEXT(pos, "cursorX %i", (i32)CurBuffer->curX); pos.y += 20.0f;
		DEBUG_TEXT(pos, "tabed line length %i", (i32)CurBuffer->cursorLines[CurBuffer->currentLine]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "line length %i", (i32)CurBuffer->lineLengths[CurBuffer->currentLine]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "line count %i", (i32)ARRAY_LENGTH(CurBuffer->lineLengths)); pos.y += 20.0f;
		DEBUG_TEXT(pos, "current line %i", (i32)CurBuffer->currentLine); pos.y += 20.0f;
		DEBUG_TEXT(pos, "pre length %i", (i32)CurBuffer->preLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "post length %i", (i32)CurBuffer->postLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "gap length %i", (i32)CurBuffer->gapLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "char under cursor %c", (i32)CurBuffer->text[CurBuffer->preLen + CurBuffer->gapLen]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "Window count %zu", ARRAY_LENGTH(windows)); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView start %i", FocusedWindow->renderView.start); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView  end %i", FocusedWindow->renderView.end); pos.y += 20.0f;

		renderer_end();

		array_release(tokens);
		array_release(windows);

		glfwSwapBuffers(editor->window);

		glfwWaitEvents();

	}

	return 0;
}
