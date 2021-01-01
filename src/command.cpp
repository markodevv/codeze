#include "command.h"
#include "cursor.h"
#include "buffer.h"

HashTable<Command> Commands;

void
cmd_cursor_left(List<char>* args) {

	cursor_left();
}

void
cmd_cursor_right(List<char>* args) {

	cursor_right();
}

void
cmd_cursor_up(List<char>* args) {

	cursor_up();
}

void
cmd_cursor_down(List<char>* args) {

	cursor_down();
}

void
commands_init() {

	hash_table_init(&Commands);
	hash_table_put(&Commands, "cursor-left", {cmd_cursor_left, 0, 0});
	hash_table_put(&Commands, "cursor-right", {cmd_cursor_right, 0, 0});
	hash_table_put(&Commands, "cursor-up", {cmd_cursor_up, 0, 0});
	hash_table_put(&Commands, "cursor-down", {cmd_cursor_down, 0, 0});
}

Command*
command_get(String& name) {

	str_push(&name, '\0');
	if (hash_table_value_exists(&Commands, name.data)) 
		return &hash_table_get(&Commands, name.data);
	else
		return NULL;
}




