#include "keymap.h"
#include "debug.h"

void
keymap_init(KeyMap* keymap) {

	for (sizet i = 0; i < KEYMAP_SIZE; ++i) {

		keymap->commands[i] = NULL;
	}
}

static i32
hash_function(KeyCode k, i32 m) {

	u32 hashValue = 17 * k + 9 * m;
	return hashValue & (KEYMAP_SIZE - 1);
}

void
keymap_bind(KeyMap* keymap, Command* cmd, KeyCode key, i32 mods) {
  
	i32 index = hash_function(key, mods);
	ASSERT_MSG(keymap->commands[index] == NULL, "collision detected");
	keymap->commands[index] = cmd;
}

Command*
keymap_get_command(KeyMap* keymap, KeyCode key, i32 mods) {

	i32 index = hash_function(key, mods);
	ASSERT(index < KEYMAP_SIZE);

	return keymap->commands[index];
}
