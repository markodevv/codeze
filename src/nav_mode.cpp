#include "modes.h"
#include "buffer.h"
#include "window.h"
#include "cursor.h"
#include "bind.h"


static void
on_event(Event& event) {

	if (event.type == KEY_PRESSED ||
		event.type == KEY_REPEAT) {
		handle_key((KeyCode)event.key, event.mods);
	}
}

static void
update() {
	
}

const EditorModeOps NavigationModeOps = {
	on_event,
	update
};
