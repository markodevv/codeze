#include "cmdstate.h"
#include "buffer.h"


static b8 OpenFile = false;

void
on_command_state_enter() {

	buffer_clear(CommandBuffer);
	LastWindow = FocusedWindow;
	FocusedWindow = CommandWindow;
	LastBuffer = CurBuffer;
	CurBuffer = CommandBuffer;


}

void
on_command_state_exit() {

	FocusedWindow = LastWindow;
	CurBuffer = LastBuffer;
	OpenFile = false;
}

void
command_execute() {

	String command = buffer_get_text_copy(CurBuffer);

	if (command == "open-file") {

		
		buffer_clear(CurBuffer);
		for (sizet i = 0; i < WorkingDirectory.length; ++i) {

			buffer_insert_char(WorkingDirectory.data[i]);
		}
		OpenFile = true;
	}
	else {

		printf("invalid command");
	}
}

void
cmd_state_update() {
		
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

	if (OpenFile) {
		
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
}

static i32
find_last_slash_index(String& text) {

	for (sizet i = text.length - 1; i >= 0; --i) {

		if (text[i] == '/') {
			return i;
		}
	}
	return 0;
}

void
check_open_file() {

	Array<String> fileNames;
	fileio_cwd_file_names(&fileNames);

	String filepath = buffer_get_text_copy(CurBuffer);
	i32 slashIndex = find_last_slash_index(filepath);

	String filestr = get_filestr_from_path(filepath);
	str_push(&filestr, '\0');

	for (sizet i = 0; i < fileNames.length; ++i) {

		if (fileNames[i] == filestr) {
			str_push(&filepath, '\0');

			buffer_add(filepath.data);
			CurBuffer = buffer_get(filestr.data);
			i32 buffId = buffer_hash_index_from_key(filestr.data);
			LastWindow->buffId = buffId;
			break;
		}
	}

}

