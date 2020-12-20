#pragma once
#include "window.h"
#include "buffer.h"

typedef enum EditorState {

	STATE_NORMAL,
	STATE_COMMAND,
	
} EditorState;

extern Buffer* CurBuffer;
extern Buffer* CmdBuffer;

extern Window* FocusedWindow;
extern WindowTree* WinTree;
extern Window* CommandWindow;

extern Window* LastWindow;
extern Buffer* LastBuffer;

extern i32 Width;
extern i32 Height;

extern EditorState CurState;

#ifdef DO_INIT

Buffer* CurBuffer;
Buffer* CommandBuffer;

Window* FocusedWindow;
WindowTree* WinTree;
Window* CommandWindow;

Window* LastWindow;
Buffer* LastBuffer;

i32 Width;
i32 Height;

EditorState CurState;

#endif
