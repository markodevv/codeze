#include "cmdstate.h"
#include "buffer.h"


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
}

void command_execute() {

	String command = buffer_get_text_copy(CurBuffer);

	if (command == "open-file") {

		
		for (sizet i = 0; i < WorkingDirectory.length; ++i) {

			buffer_insert_char(WorkingDirectory.data[i]);
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

	// Array<String> fileNames;
	// fileio_cwd_file_names(&fileNames);

	// String bufferstr = buffer_get_text_copy(CurBuffer);
	// i32 slashIndex = find_last_slash_index(bufferstr);

	// String filestr = str_substring(&bufferstr, slashIndex + 1, bufferstr.length);

	// for (sizet i = 0; i < fileNames.length; ++i) {

	// 	if (fileNames[i] == filestr) {
	// 		str_push(&bufferstr, '\0');
	// 		editor_enter_normal_state();

	// 		File file = file_open(bufferstr.data, "r");
	// 		array_push(&buffers, buffer_create(file));
	// 		CurBuffer = &buffers[buffers.length - 1];
	// 		CommandBuffer = &buffers[1];
	// 		FocusedWindow->buffId = buffers.length - 1;
	// 		break;
	// 	}
	// }

}

