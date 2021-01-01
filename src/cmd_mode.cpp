#include "modes.h"
#include "editor.h"
#include "globals.h"
#include "buffer.h"
#include "command.h"

static void
cmdmode_backspace_delete() {

	buffer_backspace_delete();
}

static void
cmdmode_insert_char(char c) {

	if (c != '\t' && c != '\n') {

		buffer_insert_char(c);
	}
}

static void
cmdmode_exit() {

	editor_set_input_mode(INPUT_NORMAL);
}

static void
cmdmode_handle_command() {

	String name = buffer_get_text_copy(CurBuffer);
	Command* cmd = command_get(name);

	if (cmd) {
		if (cmd->minArgs == 0) {
			
			cmd->cmd(NULL);
		}
	}
}

static void 
cmdmode_handle_key(i32 key, i32 mods) {
	
	switch(key) {
	case KEY_Backspace:
		cmdmode_backspace_delete();
		break;
	case KEY_Escape:
		cmdmode_exit();
		break;
	case KEY_Enter:
		cmdmode_handle_command();
		break;
	}
}

void
cmdmode_on_event(Event& event) {

	if (event.type == KEY_PRESSED || event.type == KEY_REPEAT) {

		cmdmode_handle_key(event.key, event.mods);
	}
	else if (event.type == CHAR_INPUTED) {
		cmdmode_insert_char(event.character);
	}
}

void
cmdmode_update() {
	
}

const EditorModeOps CommandModeOps = {
	cmdmode_on_event,
	cmdmode_update
};
