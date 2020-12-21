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
}

static void
editor_enter_command_state() {

	on_command_state_enter();
	CurState = STATE_COMMAND;
}


static Array<Buffer> buffers;

i32
main(int argc, char* argv[]) {

	List<Buffer> bufs;
	list_init(&bufs, 1);

	list_add(&bufs, buffer_create_empthy());
	list_add(&bufs, buffer_create_empthy());


	CurState = STATE_NORMAL;
	Width = 1916;
	Height = 1041;
	GLFWwin = renderer_create_window();

	events_initialize(GLFWwin);
	renderer_initialize(Width, Height);
	renderer_load_font("assets/CONSOLA.TTF", 18);


	fileio_init();
	File file = file_open("test.c", "r");

	array_init(&buffers, 2);
	Buffer b = buffer_create(file);
	array_push(&buffers, buffer_create(file));
	array_push(&buffers, buffer_create_empthy());

	CurBuffer = &buffers[0];
	CommandBuffer = &buffers[1];


	Window window = window_create_empthy();
	window.buffId = 0;
	window.size.w = Width;
	window.size.h = Height;
	window_tree_create(window);

	DispatchTable *dtNormalMode = new DispatchTable;
	DispatchTable *dtCmdState = new DispatchTable;
	dt_init(dtNormalMode);
	dt_init(dtCmdState);

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

	dt_bind(dtCmdState, cursor_left,             KEY_Left, 0);
	dt_bind(dtCmdState, cursor_right,            KEY_Right, 0);
	dt_bind(dtCmdState, buffer_insert_tab,       KEY_Tab, 0);
	dt_bind(dtCmdState, editor_enter_normal_state, KEY_Escape, 0);
	dt_bind(dtCmdState, buffer_backspace_delete, KEY_Backspace, 0);
	dt_bind(dtCmdState, check_open_file, KEY_Enter, 0);

	CurTable = dtNormalMode;

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


		//String bufferText = buffer_get_text(&buffers[0]);
		Array<Token> tokens;// = tokens_make(bufferText);

		Array<Window> windows;
		array_init(&windows, 32);
		tree_get_windows(WinTree, &windows);


		for (sizet i = 0; i < windows.length; ++i) {

			b8 focused = 0;
			if (windows[i].id == FocusedWindow->id) {
				focused = 1;
			}
			render_buffer(&buffers[windows[i].buffId], &windows[i], tokens, focused);
			focused = 0;
		}

		if (CurState == STATE_COMMAND) {


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
			String text = buffer_get_text_copy(CurBuffer);
			render_text(text, winPos, textColor);

			Array<String> fileNames;
			fileio_cwd_file_names(&fileNames);

			winPos.y += renderer_font_size();
			f32 startPos = winPos.y;

			for (sizet i = 0; i < fileNames.length; ++i) {

				if (winPos.y >= Height - 2 * renderer_font_size()) {

					winPos.y = startPos;
					winPos.x += 300.0f;
				}
												
				render_text(fileNames[i], winPos, {1.0f, 1.0f, 1.0f, 1.0f});
				winPos.y += renderer_font_size();

			}
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
		DEBUG_TEXT(pos, "char under cursor %c", (i32)CurBuffer->text[CurBuffer->preLen + CurBuffer->gapLen]); pos.y += 20.0f;
		DEBUG_TEXT(pos, "Window count %zu", windows.length); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView start %i", FocusedWindow->renderView.start); pos.y += 20.0f;
		DEBUG_TEXT(pos, "RenderView  end %i", FocusedWindow->renderView.end); pos.y += 20.0f;

		renderer_end();

		array_free(&windows);
		array_free(&tokens);

		glfwSwapBuffers(GLFWwin);

		glfwWaitEvents();

	}

	return 0;
}
