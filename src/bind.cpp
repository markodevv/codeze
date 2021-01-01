#include "bind.h"
#include "event.h"
#include "command.h"
#include "keymap.h"

KeyMap Bindings;

void
bindings_init() {
	
	keymap_init(&Bindings);
}


void
binding_add(Command* cmd, String& keySequence) {
	
	KeyCode key = keycode_from_string(keySequence);
	keymap_bind(&Bindings, cmd, key, 0);
}

void
handle_key(KeyCode key, i32 mods) {
	
	Command* cmd = keymap_get_command(&Bindings, key, mods);

	if (cmd) {
		
		if (cmd->minArgs == 0) {
			cmd->cmd(NULL);
		}
	}
}
