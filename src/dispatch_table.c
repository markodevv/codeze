#include "dispatch_table.h"
#include "debug.h"

static Command Commands[KEYMAP_SIZE];

void dt_initialize() {

	for (sizet i = 0; i < KEYMAP_SIZE; ++i) {

		Commands[i] = NULL;
	}
}

static i32
hash_function(KeyCode k, i32 m) {

	u32 hashValue = 17 * k + 9 * m;
	return hashValue & (KEYMAP_SIZE - 1);
}

void
dt_bind(Command command, KeyCode key, i32 mods) {
  
	i32 index = hash_function(key, mods);
	ASSERT_MSG(Commands[index] == NULL, "collision detected");
	Commands[index] = command;
}

void
dt_dispatch(KeyCode key, i32 mods) {

	i32 index = hash_function(key, mods);
	if (Commands[index]) 
		Commands[index]();
	else 
		printf("No command for key \n");
}
