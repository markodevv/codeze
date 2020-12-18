#pragma once
#include "event.h"



#define KEYMAP_SIZE 1024

typedef void(*Command)();

typedef struct DispatchTable {
  
	Command commands[KEYMAP_SIZE];

} DispatchTable;

void dt_init(DispatchTable* dt);
void dt_bind(DispatchTable* dt, Command command, KeyCode key, i32 mods);
void dt_dispatch(DispatchTable* dt, KeyCode key, i32 mods);
