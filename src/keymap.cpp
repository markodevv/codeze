#include "keymap.h"
#include "debug.h"

void
keymap_init(KeyMap* keymap) {

	for (sizet i = 0; i < KEYMAP_SIZE; ++i) {

		keymap->data[i].data = NULL;
	}
}

static i32
hash_function(i32 x, i32 y) {

	i32 hashValue = 17 * x + 9 * y;
	return hashValue & (HASH_TABLE_SIZE - 1);

}

void
keymap_bind(KeyMap* keymap, String cmdname, KeyCode key, i32 mods) {
  
	i32 index = hash_function((i32)key, mods);

	ASSERT(index < KEYMAP_SIZE && keymap->data[index].data == NULL);
	keymap->data[index] = cmdname;
}

String
keymap_get_command_name(KeyMap* keymap, KeyCode key, i32 mods) {

	i32 index = hash_function(key, mods);
	ASSERT(index < KEYMAP_SIZE);

	return keymap->data[index];
}
