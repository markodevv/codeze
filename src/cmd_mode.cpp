#include "modes.h"
#include "editor.h"
#include "globals.h"
#include "buffer.h"
#include "command.h"
#include "renderer.h"
#include "complete.h"
#include "container.h"
#include "fileio.h"

static Array<String> FieldNames;
static String Text;
static i32 SelectedFieldId;
static Buffer* CmdBuffer;

static enum CMD_STATE {
					   
					   NORMAL_STATE,
					   FIND_FILE_STATE,
}CmdState;

static inline void
update_fields() {

	Text = buffer_get_text_copy(CurBuffer);
	array_free(&FieldNames);
	FieldNames = completion_get_matching(Text);
}

static void
backspace_delete() {

	buffer_backspace_delete();
	update_fields();
}

static void
insert_char(char c) {

	if (c > 32 && c < 126) {

		buffer_insert_char(c);
	}
}

static void
normal_state_insert(char c) {
	
	insert_char(c);
	update_fields();
}

static void
find_file_state_insert(char c) {
	
	insert_char(c);
}

static void
exit() {

	editor_change_mode(MODE_NAVIGATION);
	CurBuffer = PrevBuffer;
}


static void
handle_command() {

	Text = buffer_get_text_copy(CurBuffer);
	Command* cmd = command_get(Text);
	if (cmd->cmd) {
		printf(" %i test \n", Text.length);
		if (Text == "find-file") {

			CmdState = FIND_FILE_STATE;
			String cwd = fileio_get_cwd();
			buffer_clear(CurBuffer);

			for (sizet i = 0; i < cwd.length; ++i) {

				insert_char(cwd[i]);
			}
			Text = buffer_get_text_copy(CurBuffer);

		}
		else{
			
			exit();
			if (cmd->minArgs == 0) {

				cmd->cmd(NULL);
			}
		}
	}
}


static void 
handle_key(i32 key, i32 mods) {
	
	switch(key) {
	case KEY_Backspace:
		backspace_delete();
		break;
	case KEY_Escape:
		exit();
		break;
	case KEY_Enter:
			handle_command();
		break;
	case KEY_Tab:
		if (CmdState == NORMAL_STATE) {
			
		}
		else if (CmdState == FIND_FILE_STATE) {

			Text = buffer_get_text_copy(CurBuffer);
			Array<String> filenames = fileio_path_file_names(Text);
			if (filenames.length) {
				
				array_reset(&FieldNames);
				for(sizet i = 0; i < filenames.length; ++i) {
					array_push(&FieldNames, filenames[i]);
				}
			}
		}
		break;
	case KEY_Down:
		if (SelectedFieldId >= FieldNames.length - 1) 
			break;
	    SelectedFieldId += 1;
		break;
	case KEY_Up:
		if (SelectedFieldId == 0) 
			break;
	    SelectedFieldId -= 1;
		break;
	}
}


static void
on_init() {
	
	CmdState = NORMAL_STATE;
	array_init(&FieldNames, 10);
	completion_init();

	Text = str_create("");
	SelectedFieldId = 0;
	CmdBuffer = buffer_get("command_buffer");
}

static void
on_event(Event& event) {

	if (event.type == KEY_PRESSED || event.type == KEY_REPEAT) 
		handle_key(event.key, event.mods);
	else if (event.type == CHAR_INPUTED) 
		if (CmdState == NORMAL_STATE) {
			
			normal_state_insert(event.character);
		}
		else if (CmdState == FIND_FILE_STATE) {
			
			find_file_state_insert(event.character);
		}
}

static void
on_start() {

	CurBuffer = CmdBuffer;
	buffer_clear(CurBuffer);
	SelectedFieldId = 0;

	Array<String> cmdNames = get_command_names();

	completion_reset();
	for (sizet i = 0; i < cmdNames.length; ++i) {

		completion_add(cmdNames[i]);
	}
	update_fields();
}

static void
on_end() {
	
}

static void
render_box_field(String& text, f32 posY, Vec4 color) {

	render_quad({0.0f, posY}, {(f32)TheWidth, 25.0f}, color);
	render_text(text, {(f32)TheWidth / 2.0f, posY}, {0.9f, 0.9f, 0.9f, 1.0f});
}

static void
update() {
	
	render_quad({0.0f, 0.0f}, {(f32)TheWidth, (f32)TheHeight}, {0.1f, 0.1f, 0.1f, 0.8f});
	render_text(Text, {0.0f, 0.0f}, {0.9f, 0.9f, 0.9f, 1.0f});


	f32 posY = 0;
	for (sizet i = 0; i < FieldNames.length; ++i) {

		posY += 33.0f;
		if (i == SelectedFieldId) 
			render_box_field(FieldNames[i], posY, {0.4f, 0.4f, 0.4f, 0.5f});
		else 
			render_box_field(FieldNames[i], posY, {0.2f, 0.2f, 0.2f, 0.5f});
	}

}

const EditorModeOps CommandModeOps = {
	on_event,
	update,
	on_init,
	on_start,
	on_end
};
