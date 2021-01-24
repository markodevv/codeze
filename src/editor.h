#pragma once

enum InputMode {

	MODE_NORMAL,
	MODE_COMMAND,
	MODE_NAVIGATION,
	MODES_TOTAL
	
};

void editor_change_mode(InputMode mode);
