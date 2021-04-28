#include "modes.h"
#include "buffer.h"
#include "window.h"
#include "cursor.h"
#include "bind.h"
#include "globals.h"
#include "renderer.h"


static void
on_event(Event& event) {

	if (event.type == KEY_PRESSED ||
		event.type == KEY_REPEAT) {
		handle_key((KeyCode)event.key, event.mods);

		if (event.key == KEY_Enter) 
			buffer_insert_newline();
	}
	else if (event.type == CHAR_INPUTED) {
        if (just_entered_edit_mode)
        {
            just_entered_edit_mode = false;
        }
        else
        {
            buffer_insert_char(event.character);
        }
		
	}
}

static void
on_start() {
	
}

static void
on_end() {
	
}

static void
update() {
	
	render_cursor(CurBuffer, FocusedWindow, CURSOR_LINE);
}

static void
on_init() {
	
}

const EditorModeOps NormalModeOps = {
	on_event,
	update,
	on_init,
	on_start,
	on_end
};
