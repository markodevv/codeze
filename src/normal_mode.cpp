#include "modes.h"
#include "buffer.h"
#include "window.h"
#include "cursor.h"
#include "bind.h"


void
normal_mode_on_event(Event& event) {

	if (event.type == KEY_PRESSED ||
			 event.type == KEY_REPEAT) {
		handle_key((KeyCode)event.key, event.mods);
	}
	// else if (event.type == CHAR_INPUTED) {
		
	// 	buffer_insert_char(event.character);
	// }
}

void
normal_mode_update() {
	
}

const EditorModeOps NormalModeOps = {
	normal_mode_on_event,
	normal_mode_update
};
