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

	return c == ' ' || c == '\t' || c == '\n';
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
		if (empthy_space(c))
			break;

		str_push(&word, c);
	}

	return word;
}

static String
next_word(String str, i32 &index) {

	while(!empthy_space(str[index]) && index < str.length) {

		index += 1;
	}
	while(empthy_space(str[index]) && index < str.length) {

		index += 1;
	}

	String word = str_create(10);
	for (sizet i = index; i < str.length; ++i) {

		char c = str[i];
		if (empthy_space(c))
			break;

		str_push(&word, c);
	}

	return word;
}

static void
config_handle_bind(String line, i32 &cursor, InputMode mode) {
	
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

		binding_add(cmdName, keySequence, mode);
	}
	else {

		str_push(&cmdName, '\0');
		WARN_MSG("invalid command: %s \n", cmdName.data);
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
	i32 lineNum = 0;
	InputMode currentMode = MODE_NORMAL;

	while (fgets(line, sizeof(line), fp) != NULL) {

		i32 i = index_of_first_char(line);
		String strline = str_create(line);
		String word = get_word_at(strline, i);

		if (word == "bind") {
			i += 4;
			config_handle_bind(strline, i, currentMode);
		}
		else if (word == "mode") {

			String mode = next_word(strline, i);
			if (mode == "edit") 
				currentMode = MODE_NORMAL;
			else if (mode == "navigation") 
				currentMode = MODE_NAVIGATION;
		}
		lineNum++;
	}
}

