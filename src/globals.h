#pragma once
#include "window.h"
#include "buffer.h"
#include "editor.h"
#include "tokenizer.h"

extern Buffer* CurBuffer;
extern Window* FocusedWindow;
extern Buffer* PrevBuffer;
extern i32 TheWidth;
extern i32 TheHeight;
extern InputMode InputMod;
extern bool just_entered_edit_mode;
extern Node* WinTree;
extern Array<Token> Tokens;
