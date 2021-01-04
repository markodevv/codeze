#pragma once

enum InputMode {

	INPUT_NORMAL,
	INPUT_COMMAND,
	INPUT_NAVIGATION,
	MODES_TOTAL
	
};

void editor_change_mode(InputMode mode);
