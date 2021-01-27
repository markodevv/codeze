#pragma once
#include "window.h"
#include "buffer.h"
#include "editor.h"

extern Buffer* CurBuffer;
extern Buffer* CmdBuffer;

extern Window* FocusedWindow;
extern Window* CommandWindow;

extern Window* PrevWindow;
extern Buffer* PrevBuffer;

extern i32 TheWidth;
extern i32 TheHeight;

enum InputMode;
extern InputMode InputMod;

#ifdef DO_INIT

Buffer* CurBuffer;
Buffer* CmdBuffer;

Window* FocusedWindow;
Window* CommandWindow;

Window* PrevWindow;
Buffer* PrevBuffer;

i32 TheWidth;
i32 TheHeight;

InputMode InputMod;

#endif
