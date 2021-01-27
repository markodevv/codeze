#include "modes.h"
#include "editor.h"
#include "globals.h"
#include "buffer.h"
#include "command.h"
#include "renderer.h"
#include "autocomplete.h"

static void
cmdmode_backspace_delete() {

	buffer_backspace_delete();
}

static void
cmdmode_insert_char(char c) {

	if (c > 32 && c < 126) {

		buffer_insert_char(c);
	}
}

static void
cmdmode_exit() {

	editor_change_mode(MODE_NAVIGATION);
	CurBuffer = PrevBuffer;
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

static TrieNode Root;

static void
on_init() {
	
	trie_init(&Root);
	Array<String> cmdNames = get_command_names();

	for (sizet i = 0; i < cmdNames.length; ++i) {

		trie_insert(&Root, cmdNames[i]);
	}
}

static void
on_event(Event& event) {

	if (event.type == KEY_PRESSED || event.type == KEY_REPEAT) 
		cmdmode_handle_key(event.key, event.mods);
	else if (event.type == CHAR_INPUTED) 
		cmdmode_insert_char(event.character);
}


static void
render_box_field(String& text, f32 posY) {

	render_quad({0.0f, posY}, {(f32)TheWidth, 25.0f}, {0.3f, 0.3f, 0.3f, 0.5f});
	render_text(text, {(f32)TheWidth / 2.0f, posY}, {0.9f, 0.9f, 0.9f, 1.0f});
}

static void
update() {
	
	render_quad({0.0f, 0.0f}, {(f32)TheWidth, (f32)TheHeight}, {0.1f, 0.1f, 0.1f, 0.5f});
	String text = buffer_get_text_copy(CurBuffer);
	render_text(text, {0.0f, 0.0f}, {0.9f, 0.9f, 0.9f, 1.0f});


	Array<String> matches = get_matching_names(&Root, text);
	f32 posY = 0;
	for (sizet i = 0; i < matches.length; ++i) {

		posY += 33.0f;
		render_box_field(matches[i], posY);
	}

	array_free(&matches);
}

const EditorModeOps CommandModeOps = {
	on_event,
	update,
	on_init
};
