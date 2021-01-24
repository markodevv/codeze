#include "modes.h"
#include "my_string.h"

const EditorModeOps* const Modes[] = {
	&NormalModeOps,
	&CommandModeOps,
	&NavigationModeOps
};

String
ModeToString(InputMode mode) {
	
	if (mode == MODE_COMMAND) 
		return "Command mode";
	if (mode == MODE_NAVIGATION) 
		return "Navigation mode";
	if (mode == MODE_NORMAL) 
		return "Normal mode";
}
