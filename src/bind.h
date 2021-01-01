#pragma once
#include "my_string.h"
#include "key.h"

struct Command;

void bindings_init();
void binding_add(Command* cmd, String& keySequence);
void handle_key(KeyCode key, i32 mods);
