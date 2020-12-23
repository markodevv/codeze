#include "window.h"
/* TODO:

   - file opening/closing
   - undo redo
   - switching buffers
 */

/* DONE:

   - closing windows
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


static GLFWwindow* GLFWwin;


static void
editor_enter_normal_state() {

	on_command_state_exit();
	CurState = STATE_NORMAL;
	CurTable = &NormalTable;
}

static void
editor_enter_command_state() {

	on_command_state_enter();
	CurState = STATE_COMMAND;
	CurTable = &CmdTable;
}



i32
main(int argc, char* argv[]) {

	CurState = STATE_NORMAL;
	Width = 1916;
	Height = 1041;
	GLFWwin = renderer_create_window();

	events_initialize(GLFWwin);
	renderer_initialize(Width, Height);
	renderer_load_font("assets/CONSOLA.TTF", 18);

	fileio_init();

	const char* testFileName = "/test.c";
	String filepath = str_create(10);

	for (sizet i = 0; i < WorkingDirectory.length; ++i) {
		str_push(&filepath, WorkingDirectory[i]);
	}
	for (sizet i = 0; i < strlen(testFileName); ++i) {

		str_push(&filepath, testFileName[i]);
	}
	str_push(&filepath, '\0');

	buffers_init();
	buffer_add(filepath.data);
	buffer_add_empthy("command_buffer");

	CurBuffer = buffer_get("test.c");
	CommandBuffer = buffer_get("command_buffer");


	Window window = window_create_empthy();
	window.buffId = 
	window.size.w = Width;
	window.size.h = Height;
	windows_init(buffer_hash_index_from_key("test.c"),
				 buffer_hash_index_from_key("command_buffer"));

	dt_init(&NormalTable);
	dt_init(&CmdTable);

	dt_bind(&NormalTable, cursor_down,             KEY_Down, 0);
	dt_bind(&NormalTable, cursor_up,               KEY_Up, 0);
	dt_bind(&NormalTable, cursor_left,             KEY_Left, 0);
	dt_bind(&NormalTable, cursor_right,            KEY_Right, 0);
	dt_bind(&NormalTable, buffer_insert_newline,   KEY_Enter, 0);
	dt_bind(&NormalTable, buffer_insert_tab,       KEY_Tab, 0);
	dt_bind(&NormalTable, buffer_backspace_delete, KEY_Backspace, 0);
	dt_bind(&NormalTable, window_split_horizontal, KEY_S, MOD_CONTROL);
	dt_bind(&NormalTable, window_split_vertical,   KEY_V, MOD_CONTROL);
	dt_bind(&NormalTable, window_close,            KEY_Q, MOD_CONTROL | MOD_ALT);
	dt_bind(&NormalTable, window_switch_up,        KEY_K, MOD_CONTROL);
	dt_bind(&NormalTable, window_switch_down,      KEY_J, MOD_CONTROL);
	dt_bind(&NormalTable, window_switch_left,      KEY_H, MOD_CONTROL);
	dt_bind(&NormalTable, window_switch_right,     KEY_L, MOD_CONTROL);
	dt_bind(&NormalTable, editor_enter_command_state, KEY_Enter, MOD_CONTROL);

	dt_bind(&CmdTable, cursor_left,             KEY_Left, 0);
	dt_bind(&CmdTable, cursor_right,            KEY_Right, 0);
	dt_bind(&CmdTable, buffer_insert_tab,       KEY_Tab, 0);
	dt_bind(&CmdTable, editor_enter_normal_state, KEY_Escape, 0);
	dt_bind(&CmdTable, buffer_backspace_delete, KEY_Backspace, 0);
	dt_bind(&CmdTable, command_execute, KEY_Enter, 0);

	CurTable = &NormalTable;

	while (!glfwWindowShouldClose(GLFWwin)) {
		
		Event event;
		while(event_queue_next(&event)) { 
			if (event.type == KEY_PRESSED) {

				dt_dispatch(CurTable, (KeyCode)event.key, event.mods);
			}
			else if (event.type == KEY_REPEAT) {

				dt_dispatch(CurTable, (KeyCode)event.key, event.mods);
			}
			else if (event.type == CHAR_INPUTED) {

				buffer_insert_char(event.character);
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

				Width = event.width;
				Height = event.height;
				renderer_on_window_resize(event.width, event.height);
			}
		} 


		renderer_begin();

		Array<Window> windows;
		windows_get_all(&windows);


		for (sizet i = 0; i < windows.length; ++i) {

			b8 focused = 0;
			if (windows[i].id == FocusedWindow->id) {
				focused = 1;
			}
			render_buffer(buffer_get(windows[i].buffId), &windows[i], NULL, focused);
			focused = 0;
		}

		if (CurState == STATE_COMMAND) {
			cmd_state_update();
		}
		render_cursor(CurBuffer, FocusedWindow);

		Vec2 pos;
		pos.x = Width - 200.0f;
		pos.y = Height - 333.0f;
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
		if (CurBuffer->preLen != 0) {
			
			DEBUG_TEXT(pos, "char before cursor %c", (i32)CurBuffer->text[CurBuffer->preLen - 1]); pos.y += 20.0f;
		}
		DEBUG_TEXT(pos, "char under cursor %c", (i32)CurBuffer->text[CurBuffer->preLen + CurBuffer->gapLen]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "Window count %zu", windows.length); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView start %i", FocusedWindow->renderView.start); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView  end %i", FocusedWindow->renderView.end); pos.y += 20.0f;

		renderer_end();

		array_free(&windows);

		glfwSwapBuffers(GLFWwin);

		glfwWaitEvents();

	}

	return 0;
}
