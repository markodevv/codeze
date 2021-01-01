#pragma once
#include "window.h"
#include "buffer.h"

extern Buffer* CurBuffer;
extern Buffer* CmdBuffer;

extern Window* FocusedWindow;
extern Window* CommandWindow;

extern Window* LastWindow;
extern Buffer* LastBuffer;

extern i32 TheWidth;
extern i32 TheHeight;

#ifdef DO_INIT

Buffer* CurBuffer;
Buffer* CmdBuffer;

Window* FocusedWindow;
Window* CommandWindow;

Window* LastWindow;
Buffer* LastBuffer;

i32 TheWidth;
i32 TheHeight;

#endif
