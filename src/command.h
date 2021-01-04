#pragma once
#include "my_string.h"
#include "types.h"
#include "container.h"

struct Command {

	void (*cmd)(List<char>* args);
	i8 minArgs;
	i8 maxArgs;

};

void commands_init();
void command_handle(String& cmdname);
Command* command_get(String& cmdname);
