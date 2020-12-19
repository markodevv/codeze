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

static Editor App;
static Window* LastWindow;
static Buffer* LastBuffer;

static void
editor_enter_command_state() {
	buffer_clear(CommandBuffer);

	App.state = STATE_COMMAND;
	LastWindow = FocusedWindow;
	FocusedWindow = CommandWindow;
	LastBuffer = CurBuffer;
	CurBuffer = CommandBuffer;

	for (sizet i = 0; i < WorkingDirectory.length; ++i) {

		buffer_insert_char(WorkingDirectory.data[i]);
	}

	// String s;
	// while ((s = directory_filename_next()) != -1) {

	// 	s = str_push(s, '\0');
	// 	printf("%s \n", s);
	// }
}

static void
editor_enter_normal_state() {

	App.state = STATE_NORMAL;
	FocusedWindow = LastWindow;
	CurBuffer = LastBuffer;
}

void
print_array(Array<int> arr) {
	for (sizet i = 0; i < arr.length; ++i) {
		printf("%i, ", arr[i]);
	}
	printf("\n");
}

i32
main(int argc, char* argv[]) {

	Array<int> arr;
	arr.init(10);
	for (sizet i = 0; i < arr.capacity; ++i) {
		arr.push(i);
	}
	print_array(arr);

	arr.erase(5);

	print_array(arr);

	arr.insert(17, 5);

	print_array(arr);
	
	
	App.state = STATE_NORMAL;
	App.width = 1916;
	App.height = 1041;
	App.window = renderer_create_window();

	events_initialize(App.window);
	renderer_initialize(App.width, App.height);
	renderer_load_font("assets/CONSOLA.TTF", 18);


	fileio_init();
	File* file = file_open("test.c", "r");

	Array<Buffer> buffers;
	buffers.init(2);
	Buffer b = buffer_create(file);
	buffers.push(buffer_create(file));
	buffers.push(buffer_create_empthy());

	CurBuffer = &buffers[0];
	CommandBuffer= &buffers[1];

	file_close(file);

	Window window = window_create_empthy();
	window.buffId = 0;
	window.size.w = App.width;
	window.size.h = App.height;
	window_tree_create(window);

	DispatchTable *dtNormalMode = new DispatchTable;
	DispatchTable *dtCommandMode = new DispatchTable;
	dt_init(dtNormalMode);
	dt_init(dtCommandMode);

	dt_bind(dtNormalMode, cursor_down,             KEY_Down, 0);
	dt_bind(dtNormalMode, cursor_up,               KEY_Up, 0);
	dt_bind(dtNormalMode, cursor_left,             KEY_Left, 0);
	dt_bind(dtNormalMode, cursor_right,            KEY_Right, 0);
	dt_bind(dtNormalMode, buffer_insert_newline,   KEY_Enter, 0);
	dt_bind(dtNormalMode, buffer_insert_tab,       KEY_Tab, 0);
	dt_bind(dtNormalMode, buffer_backspace_delete, KEY_Backspace, 0);
	dt_bind(dtNormalMode, window_split_horizontal, KEY_S, MOD_CONTROL);
	dt_bind(dtNormalMode, window_split_vertical,   KEY_V, MOD_CONTROL);
	dt_bind(dtNormalMode, window_close,            KEY_Q, MOD_CONTROL | MOD_ALT);
	dt_bind(dtNormalMode, window_switch_up,        KEY_K, MOD_CONTROL);
	dt_bind(dtNormalMode, window_switch_down,      KEY_J, MOD_CONTROL);
	dt_bind(dtNormalMode, window_switch_left,      KEY_H, MOD_CONTROL);
	dt_bind(dtNormalMode, window_switch_right,     KEY_L, MOD_CONTROL);
	dt_bind(dtNormalMode, editor_enter_command_state, KEY_Enter, MOD_CONTROL);

	dt_bind(dtCommandMode, cursor_left,             KEY_Left, 0);
	dt_bind(dtCommandMode, cursor_right,            KEY_Right, 0);
	dt_bind(dtCommandMode, buffer_insert_tab,       KEY_Tab, 0);
	dt_bind(dtCommandMode, editor_enter_normal_state, KEY_Escape, 0);
	dt_bind(dtCommandMode, buffer_backspace_delete, KEY_Backspace, 0);

	while (!glfwWindowShouldClose(App.window)) {
		
		Event event;
		while(event_queue_next(&event)) { 
			if (App.state == STATE_NORMAL) {
				
				if (event.type == KEY_PRESSED) {

					dt_dispatch(dtNormalMode, (KeyCode)event.key, event.mods);
				}
				else if (event.type == KEY_REPEAT) {

					dt_dispatch(dtNormalMode, (KeyCode)event.key, event.mods);
				}
				else if (event.type == CHAR_INPUTED) {

					buffer_insert_char(event.character);
				}
			}
			else if (App.state == STATE_COMMAND) {
				
				if (event.type == KEY_PRESSED) {

					dt_dispatch(dtCommandMode, (KeyCode)event.key, event.mods);
				}
				else if (event.type == KEY_REPEAT) {

					dt_dispatch(dtCommandMode, (KeyCode)event.key, event.mods);
				}
				else if (event.type == CHAR_INPUTED) {

					buffer_insert_char(event.character);
				}
			}
			//                    TODO:
			else if (event.type == -5) {
				/*

				 // TODO: make it work for last line 
				 f64 xpos, ypos;
				 glfwGetCursorPos(Editor.window, &xpos, &ypos);

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

			*/
			}
			else if	(event.type == WINDOW_RESIZED) {

				App.width = event.width;
				App.height = event.height;
				renderer_on_window_resize(event.width, event.height);
			}
		} 


		renderer_begin();


		String bufferText = buffer_get_text(&buffers[0]);
		Array<Token> tokens = tokens_make(bufferText);

		Array<Window> windows;
		windows.init(26);
		tree_get_windows(WinTree, &windows);


		for (sizet i = 0; i < windows.length; ++i) {

			b8 focused = 0;
			if (windows[i].id == FocusedWindow->id) {
				focused = 1;
			}
			render_buffer(&buffers[windows[i].buffId], &windows[i], tokens, focused);
			focused = 0;
		}

		if (App.state == STATE_COMMAND) {


			Vec2 winPos = {
				(f32)CommandWindow->position.x,
				(f32)CommandWindow->position.y
			};
			Vec2 winSize = {
				(f32)CommandWindow->size.w,
				(f32)CommandWindow->size.h
			};

			Vec4 winColor = {0.1f, 0.1f, 0.1f, 0.8f};
			Vec4 textColor = {1.0f, 1.0f, 1.0f, 1.0f};

			render_quad(winPos, winSize, winColor);
			render_text(CurBuffer, winPos, textColor);
		}
		render_cursor(CurBuffer, FocusedWindow);

		Vec2 pos;
		pos.x = App.width - 200.0f;
		pos.y = App.height - 333.0f;
		DEBUG_TEXT(pos, "- DEBUG TEXT -", NULL); pos.y += 20.0f;

		DEBUG_TEXT(pos, "cursorX tabed %i", (i32)CurBuffer->cursorXtabed); pos.y += 20.0f;
		DEBUG_TEXT(pos, "cursorX %i", (i32)CurBuffer->curX); pos.y += 20.0f;
		DEBUG_TEXT(pos, "tabed line length %i", (i32)CurBuffer->cursorLines[CurBuffer->currentLine]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "line length %i", (i32)CurBuffer->lineLengths[CurBuffer->currentLine]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "line count %i", (i32)CurBuffer->lineLengths.length) pos.y += 20.0f;
		DEBUG_TEXT(pos, "current line %i", (i32)CurBuffer->currentLine); pos.y += 20.0f;
		DEBUG_TEXT(pos, "pre length %i", (i32)CurBuffer->preLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "post length %i", (i32)CurBuffer->postLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "gap length %i", (i32)CurBuffer->gapLen); pos.y += 20.0f;
		DEBUG_TEXT(pos, "char under cursor %c", (i32)CurBuffer->text.data[CurBuffer->preLen + CurBuffer->gapLen]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "Window count %zu", windows.length); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView start %i", FocusedWindow->renderView.start); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView  end %i", FocusedWindow->renderView.end); pos.y += 20.0f;

		renderer_end();

		windows.free_data();
		tokens.free_data();
		str_free(&bufferText);

		glfwSwapBuffers(App.window);

		glfwWaitEvents();

	}

	return 0;
}
