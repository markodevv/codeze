#include "bind.h"
#include "event.h"
#include "command.h"
#include "keymap.h"
#include "editor.h"
#include "globals.h"
#include "my_string.h"
#include "key.h"

#include <ctype.h>

KeyMap Bindings[MODES_TOTAL];


void
bindings_init() {
	
	for (sizet i = 0; i < MODES_TOTAL; ++i) 
		keymap_init(&Bindings[i]);
}

static i32
get_mod_from_string(String str) {

	if (str == "C") 
		return MOD_CONTROL;
	if (str == "M") 
		return MOD_SUPER;
	if (str == "A") 
		return MOD_ALT;
	if (str == "S") 
		return MOD_SHIFT;
}

static b8
is_modifier(String str) {

	if (str == "C" || str == "S" || str == "M" || str == "A") {

		return true;
	}
	return false;
}

static String
next_key(String keys, i32* cursor) {

	String out = str_create("");

	for (sizet i = *cursor; i < keys.length; ++i) {

		if (keys[i] == '-') {
			(*cursor)++;
			break;
		}
		str_push(&out, keys[i]);
		(*cursor)++;
	}

	return out;
}

void
binding_add(String cmdname, String keys, InputMode mode) {

	KeyCode key;
	i32 mods = 0;
	String keystr = str_create("");
	for (i32 cursor = 0; cursor < keys.length;) {

		keystr = next_key(keys, &cursor);
		if (is_modifier(keystr)) {

			mods = mods | get_mod_from_string(keystr);
		}
		else {

			key = keycode_from_string(keystr);
			if (cursor + 1 < keys.length || key == KEY_Unknown) {

				WARN_MSG("Invalid key sequence: %s \n", keys.as_cstr());
				break;
			}

		}
	}

	keymap_bind(&Bindings[mode], cmdname, key, mods);
}

void
handle_key(KeyCode key, i32 mods) {
	
	String cmdname = keymap_get_command_name(&Bindings[InputMod], key, mods);
	if (cmdname.data) {
		command_handle(cmdname);
	}
}
