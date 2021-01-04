#include "config.h"
#include "command.h"
#include "editor.h"
#include "bind.h"

#ifdef LINUX_PLATFORM
	#include <unistd.h>
#elif WINDOWS_PLATFORM
	#include <io.h>
#endif


static b8
empthy_space(char c) {

	return c == ' ' || c == '\t';
}

static b8
valid_cmd(String cmdname) {

	Command* cmd = command_get(cmdname);
	if (cmd) 
		return true;

	return false;
}

static String
get_word_at(String str, i32 index) {

	String word = str_create(10);

	for (sizet i = index; i < str.length; ++i) {

		char c = str[i];
		if (c == '\n' || c == ' ')
			break;

		str_push(&word, c);
	}

	return word;
}

static void
config_handle_bind(String line, i32 cursor) {
	

	while(empthy_space(line[cursor]))
		cursor++;

	String cmdName = str_create("");

	while(!empthy_space(line[cursor])) {

		str_push(&cmdName, line[cursor]);
		cursor++;
	}


	if (valid_cmd(cmdName)) {
		
		while(empthy_space(line[cursor]))
			cursor++;

		String keySequence = str_create("");
		while(line[cursor] != ' ' &&
			line[cursor] != '\n' &&
			cursor < line.length) {

			str_push(&keySequence, line[cursor]);
			cursor++;
		}

		binding_add(cmdName, keySequence, INPUT_NORMAL);
	}
	else {

		str_push(&cmdName, '\0');
		printf("invalid command: %s \n", cmdName.data);
	}
	
}

static i32
index_of_first_char(const char* str) {

	i32 out = 0;
	while (str[out] == ' ' || str[out] == '\t') {
		out++;
	}

	return out;
}

void 
config_read(const char* path) {
	
	FILE* fp = fopen(path, "r");
	ASSERT(fp);
	char line[4096];

	while (fgets(line, sizeof(line), fp) != NULL) {

		i32 i = index_of_first_char(line);
		if (line[i] == 'b') {

			String strline = str_create(line);
			String word = get_word_at(strline, i);

			if (word == "bind") {
				config_handle_bind(strline, i + 4);
			}
		}
	}
}

