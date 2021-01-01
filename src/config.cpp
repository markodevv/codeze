#include "config.h"
#include "command.h"
#include "bind.h"

#include <unistd.h>

const char* BindIdentifier = "bind";

static void
config_parse_bind(String& line) {
	
	i32 cursor = 0;
	for (sizet i = 0; i < line.length; ++i) {
		if (line[i] == BindIdentifier[i]) {
			
			cursor++;
			continue;
		}
		else
			break;
	}

	while(line[cursor] == ' ')
		cursor++;

	String cmdName = str_create("");

	while(line[cursor] != ' ') {
		str_push(&cmdName, line[cursor]);
		cursor++;
	}

	Command* cmd = command_get(cmdName);

	if (cmd) {
		
		while(line[cursor] == ' ')
			cursor++;

		String keySequence = str_create("");
		while(line[cursor] != ' ' &&
			  line[cursor] != '\n' &&
			  line[cursor] != '\0') {
			str_push(&keySequence, line[cursor]);
			cursor++;
		}

		binding_add(cmd, keySequence);
	}
	
}

void 
config_read(const char* path) {
	
	FILE* fp = fopen(path, "r");

	char line[4096];

	while (fgets(line, sizeof(line), fp) != NULL) {

		if (line[0] == 'b') {
			String strline = str_create(line);
			config_parse_bind(strline);
		}
	}
}
