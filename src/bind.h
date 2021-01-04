#pragma once
#include "key.h"
#include "types.h"

struct String;
enum InputMode;

void bindings_init();
void binding_add(String cmdName, String keySequence, InputMode mode);
void handle_key(KeyCode key, i32 mods);
