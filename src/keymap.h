#pragma once
#include "event.h"
#include "container.h"
#include "my_string.h"

#define KEYMAP_SIZE 1024

struct KeyMap {

	String data[KEYMAP_SIZE];
};

void keymap_init(KeyMap* keymap);
void keymap_bind(KeyMap* keymap, String cmdname, KeyCode key, i32 mods);
String keymap_get_command_name(KeyMap* keymap, KeyCode key, i32 mods);
