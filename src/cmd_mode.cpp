#include "modes.h"
#include "editor.h"
#include "globals.h"
#include "buffer.h"
#include "command.h"
#include "renderer.h"
#include "complete.h"
#include "cursor.h"
#include "container.h"
#include "fileio.h"

enum CmdMode {
					   
					   MODE_CMD,
					   MODE_FIND_FILE,
					   MODE_MAX
};

struct MinorModeOps {
	
	void (*on_start)();
	void (*handle_key)(i32 key, i32 mods);
}; 

static Array<String> FieldNames;
static String Text;
static i32 SelectedFieldId;
static Buffer* CmdBuffer;
static CmdMode CmdCurMode;


static MinorModeOps MinorModes[MODE_MAX];


static void
empthy() {
	
}

static inline void
sort_completion(String& word) {

	str_array_free(FieldNames);
	FieldNames = completion_get_matching(word);
	SelectedFieldId = 0;
}

static void
cmdmode_backspace() {

	buffer_backspace_delete();
	Text = buffer_get_text_copy(CurBuffer);
	sort_completion(Text);
}

static void
insert_char(char c) {

	if (c > 32 && c < 126) {

		buffer_insert_char(c);
	}
}

static void
cmdmode_insert(char c) {
	
	insert_char(c);
	Text = buffer_get_text_copy(CurBuffer);
	sort_completion(Text);
}

static String
last_word_from_path() {

	Text = buffer_get_text_copy(CurBuffer);
	String out = str_create("");

	sizet index = Text.length - 1;

	while (Text[index] != '/' && index > 0) {
		str_push(&out, Text[index]);
		index--;
	}
	str_reverse(&out);

	return out;
}

static void
findfile_update_completion() {
	
	Array<String> filenames = fileio_cwd_file_names();

	completion_reset();

	if (filenames.length) {

		str_array_free(FieldNames);
		array_init(&FieldNames, 10);

		for(sizet i = 0; i < filenames.length; ++i) {
			array_push(&FieldNames, filenames[i]);
			completion_add(filenames[i]);
		}
	}

}


static void
findfile_insert(char c) {
	String word;
	
	if (c == '/') {
		word = last_word_from_path();
		if (fileio_change_dir(word)) {

			findfile_update_completion();
			word = str_create("");
			insert_char(c);
		}
	}
	else {
		insert_char(c);
		word = last_word_from_path();
	}

	sort_completion(word);
}


static void
findfile_backspace() {
	
	char c = char_under_cursor();
	buffer_backspace_delete();

	if (c == '/') {
		String cd = str_create("..");
		fileio_change_dir(cd);
		findfile_update_completion();
	}
	String word = last_word_from_path();
	sort_completion(word);

}

static void
exit() {

	editor_change_mode(MODE_NAVIGATION);
	CurBuffer = PrevBuffer;
}

static inline void
change_minor_mode_to(CmdMode mode) {

	MinorModes[mode].on_start();
	CmdCurMode = mode;
}

static void
handle_command() {

	Text = buffer_get_text_copy(CurBuffer);
	Command* cmd = command_get(Text);
	if (cmd->cmd) {
		if (Text == "find-file") {

			change_minor_mode_to(MODE_FIND_FILE);
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
cmdmode_complete() {

	if (FieldNames.length) {

		Text = buffer_get_text_copy(CurBuffer);

		for (sizet i = Text.length; i < FieldNames[SelectedFieldId].length; ++i) {

			cmdmode_insert(FieldNames[SelectedFieldId][i]);
		}
	}
}

static void
findfile_complete() {
	
	if (FieldNames.length) {

		String word = last_word_from_path();

		for (sizet i = word.length; i < FieldNames[SelectedFieldId].length; ++i) {

			findfile_insert(FieldNames[SelectedFieldId][i]);
		}
	}
}

static void 
cmdmode_handle_key(i32 key, i32 mods) {
	
	switch(key) {
	case KEY_Backspace:
		cmdmode_backspace();
		break;
	case KEY_Escape:
		exit();
		break;
	case KEY_Enter:
		handle_command();
		break;
	case KEY_Tab:
		cmdmode_complete();
		break;
	case KEY_Down:
		if (SelectedFieldId < FieldNames.length)
			SelectedFieldId++;
		break;
	case KEY_Up:
		if (SelectedFieldId > 0) 
			SelectedFieldId--;
		break;
	}
}

static void
findfile_open_file() {

	
}

static void
findfile_handle_key(i32 key, i32 mods) {
	
	switch(key) {
	case KEY_Enter:
		break;
	case KEY_Tab: 
		findfile_complete();
		break;
	case KEY_Backspace:
		findfile_backspace();
		break;
	case KEY_Down:
		if (SelectedFieldId < FieldNames.length)
			SelectedFieldId++;
		break;
	case KEY_Up:
		if (SelectedFieldId > 0) 
			SelectedFieldId--;
		break;
	}
}


static void
findfile_on_start() {
	
	String cwd = fileio_get_cwd();
	buffer_clear(CurBuffer);

	for (sizet i = 0; i < cwd.length; ++i) 
		insert_char(cwd[i]);

	findfile_update_completion();
	insert_char('/');
}


static void
on_init() {
	
	MinorModes[MODE_CMD].on_start = empthy;
	MinorModes[MODE_CMD].handle_key = cmdmode_handle_key;

	MinorModes[MODE_FIND_FILE].on_start = findfile_on_start;
	MinorModes[MODE_FIND_FILE].handle_key = findfile_handle_key;

	CmdCurMode = MODE_CMD;
	array_init(&FieldNames, 10);
	completion_init();

	Text = str_create("");
	SelectedFieldId = 0;
	CmdBuffer = buffer_get("command_buffer");
}

static void
on_event(Event& event) {

	if (event.type == KEY_PRESSED || event.type == KEY_REPEAT) {
		MinorModes[CmdCurMode].handle_key(event.key, event.mods);
	}
	else if (event.type == CHAR_INPUTED)  {

		if (CmdCurMode == MODE_CMD) {
			
			cmdmode_insert(event.character);
		}
		else if (CmdCurMode == MODE_FIND_FILE) {
			
			findfile_insert(event.character);
		}
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
	Text = buffer_get_text_copy(CurBuffer);
	sort_completion(Text);
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
	Text = buffer_get_text_copy(CurBuffer);
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

