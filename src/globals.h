#pragma once
#include "window.h"
#include "buffer.h"
#include "editor.h"

extern Buffer* CurBuffer;

extern Window* FocusedWindow;

extern Buffer* PrevBuffer;

extern i32 TheWidth;
extern i32 TheHeight;

enum InputMode;
extern InputMode InputMod;

#ifdef DO_INIT

Buffer* CurBuffer;

Window* FocusedWindow;

Buffer* PrevBuffer;

i32 TheWidth;
i32 TheHeight;

InputMode InputMod;

#endif
