#pragma once

typedef enum InputMode {

	INPUT_NORMAL,
	INPUT_COMMAND,
	
} InputMode;

void editor_set_input_mode(InputMode im);
