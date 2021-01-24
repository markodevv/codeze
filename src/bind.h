#pragma once
#include "key.h"
#include "types.h"
#include "editor.h"

struct String;

void bindings_init();
void binding_add(String cmdName, String keySequence, InputMode mode);
void handle_key(KeyCode key, i32 mods);
