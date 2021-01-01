#pragma once
#include "event.h"



#define KEYMAP_SIZE 1024

struct Command;

typedef struct KeyMap {
  
	Command* commands[KEYMAP_SIZE];

} KeyMpa;

void keymap_init(KeyMap* keymap);
void keymap_bind(KeyMap* keymap, Command* cmd, KeyCode key, i32 mods);
Command* keymap_get_command(KeyMap* keymap, KeyCode key, i32 mods);
