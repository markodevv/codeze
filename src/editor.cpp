#include "editor.h"
#include "window.h"
#include "renderer.h"
#include "buffer.h"
#include "debug.h"
#include "cursor.h"
#include "modes.h"
#include "command.h"
#include "bind.h"
#include "config.h"

#define DO_INIT
#include "globals.h"
#undef DO_INIT

/* TODO:
   - config file parser
   - red black trees
   - file opening/closing
   - undo redo
   - switching buffers
 */


static GLFWwindow* GLFWwin;

void 
editor_change_mode(InputMode mode) {

	Modes[InputMod]->on_end();
	Modes[mode]->on_start();
	InputMod = mode;
}


i32
main(int argc, char* argv[]) {


	InputMod = MODE_NORMAL;
	TheWidth = 1916;
	TheHeight = 1041;
	GLFWwin = renderer_create_window();

	events_initialize(GLFWwin);
	renderer_initialize(TheWidth, TheHeight);
	renderer_load_font("assets/CONSOLA.TTF", 18);

	fileio_update_cwd();
	commands_init();
	bindings_init();

	config_read("config.txt");

	const char* testFileName = "/test.c";
	String filepath = str_create(10);

	String cwd = fileio_get_cwd();
	for (sizet i = 0; i < cwd.length; ++i) {
		str_push(&filepath, cwd[i]);
	}
	for (sizet i = 0; i < strlen(testFileName); ++i) {

		str_push(&filepath, testFileName[i]);
	}
	str_push(&filepath, '\0');

	buffers_init();
	buffer_add(filepath.data);
	buffer_add_empthy("command_buffer");

	CurBuffer = buffer_get("test.c");

	windows_init(buffer_hash_index_from_key("test.c"),
				 buffer_hash_index_from_key("command_buffer"));

	for (sizet i = 0; i < MODES_TOTAL; ++i) {
		Modes[i]->on_init();
	}

	while (!glfwWindowShouldClose(GLFWwin)) {
		
		Event event;
		while(event_queue_next(&event)) { 
			Modes[InputMod]->on_event(event);

			if	(event.type == WINDOW_RESIZED) {

				TheWidth = event.width;
				TheHeight = event.height;
				renderer_on_window_resize(event.width, event.height);
			}
		} 


		renderer_begin();

		window_render_all();
		Modes[InputMod]->update();
		render_cursor(CurBuffer, FocusedWindow);

#ifdef DEBUG
		Vec2 pos;
		pos.x = TheWidth - 200.0f;
		pos.y = TheHeight - 400.0f;
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
		if (CurBuffer->preLen != 0) 
			DEBUG_TEXT(pos, "char before cursor %c", (i32)CurBuffer->text[CurBuffer->preLen - 1]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "char under cursor %c", (i32)CurBuffer->text[CurBuffer->preLen + CurBuffer->gapLen]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView start %i", FocusedWindow->renderView.start); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView  end %i", FocusedWindow->renderView.end); pos.y += 20.0f;
		DEBUG_TEXT(pos, "Width %i", TheWidth); pos.y += 20.0f;
		DEBUG_TEXT(pos, "Height %i", TheHeight); pos.y += 20.0f;
		String mode = ModeToString(InputMod);
		str_push(&mode, '\0');
		DEBUG_TEXT(pos, "Mode %s", mode.data); pos.y += 20.0f;
#endif

		renderer_end();

		glfwSwapBuffers(GLFWwin);

		glfwWaitEvents();

	}

	return 0;
}
/*
			else if (event.type == -5) {

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

			}

*/
