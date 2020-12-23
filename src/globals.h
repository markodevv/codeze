#pragma once
#include "window.h"
#include "buffer.h"
#include "dispatch_table.h"

typedef enum EditorState {

	STATE_NORMAL,
	STATE_COMMAND,
	
} EditorState;

extern Buffer* CurBuffer;
extern Buffer* CmdBuffer;

extern Window* FocusedWindow;
extern Window* CommandWindow;

extern Window* LastWindow;
extern Buffer* LastBuffer;

extern i32 Width;
extern i32 Height;

extern EditorState CurState;

extern DispatchTable* CurTable;

extern DispatchTable dtNormalMode;
extern DispatchTable dtCmdState;


#ifdef DO_INIT

Buffer* CurBuffer;
Buffer* CommandBuffer;

Window* FocusedWindow;
Window* CommandWindow;

Window* LastWindow;
Buffer* LastBuffer;

i32 Width;
i32 Height;

EditorState CurState;

DispatchTable* CurTable;
DispatchTable NormalTable;
DispatchTable CmdTable;

#endif
