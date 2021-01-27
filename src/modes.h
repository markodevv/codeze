#pragma once
#include "types.h"
#include "event.h"
#include "editor.h"

struct EditorModeOps {

	void (*on_event)(Event& event);
	void (*update)();
	void (*on_init)();
};

struct String;
String ModeToString(InputMode mode);

extern const EditorModeOps NormalModeOps;
extern const EditorModeOps CommandModeOps;
extern const EditorModeOps NavigationModeOps;

extern const EditorModeOps* const Modes[];
