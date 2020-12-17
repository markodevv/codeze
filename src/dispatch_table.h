#pragma once
#include "event.h"



#define KEYMAP_SIZE 1024

typedef void(*Command)();

void dt_initialize();
void dt_bind(Command command, KeyCode key, i32 mods);
void dt_dispatch(KeyCode key, i32 mods);
