#pragma once
#include "types.h"
#include "event.h"

struct EditorModeOps {

	void (*on_event)(Event& event);
	void (*update)();
};


extern const EditorModeOps NormalModeOps;
extern const EditorModeOps CommandModeOps;
extern const EditorModeOps NavigationModeOps;

extern const EditorModeOps* const Modes[];
