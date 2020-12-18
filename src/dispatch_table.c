#include "dispatch_table.h"
#include "debug.h"

void
dt_init(DispatchTable* dt) {

	for (sizet i = 0; i < KEYMAP_SIZE; ++i) {

		dt->commands[i] = NULL;
	}
}

static i32
hash_function(KeyCode k, i32 m) {

	u32 hashValue = 17 * k + 9 * m;
	return hashValue & (KEYMAP_SIZE - 1);
}

void
dt_bind(DispatchTable* dt, Command command, KeyCode key, i32 mods) {
  
	i32 index = hash_function(key, mods);
	ASSERT_MSG(dt->commands[index] == NULL, "collision detected");
	dt->commands[index] = command;
}

void
dt_dispatch(DispatchTable* dt, KeyCode key, i32 mods) {

	i32 index = hash_function(key, mods);
	ASSERT(index < KEYMAP_SIZE);
	if (dt->commands[index]) 
		dt->commands[index]();
	else 
		printf("No command for key %i \n", key);
}
