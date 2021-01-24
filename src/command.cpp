#include "command.h"
#include "cursor.h"
#include "buffer.h"
#include "window.h"
#include "editor.h"

static HashTable<Command> Commands;

static void
cmd_cursor_left(List<char>* args) {

	cursor_left();
}

static void
cmd_cursor_right(List<char>* args) {

	cursor_right();
}

static void
cmd_cursor_up(List<char>* args) {

	cursor_up();
}

static void
cmd_cursor_down(List<char>* args) {

	cursor_down();
}

static void 
cmd_window_split_vertical(List<char>* args) {

	window_split_vertical();
}

static void 
cmd_window_split_horizontal(List<char>* args) {


	window_split_horizontal();
}

static void 
cmd_window_switch_up(List<char>* args) {

	window_switch_up();
}

static void 
cmd_window_switch_down(List<char>* args) {

	window_switch_down();
}

static void 
cmd_window_switch_left(List<char>* args) {

	window_switch_left();
}

static void 
cmd_window_switch_right(List<char>* args) {

	window_switch_right();
}

static void 
cmd_window_close(List<char>* args) {

	window_close();
}

static void
cmd_enter_edit_mode(List<char>* args) {

	editor_change_mode(MODE_NORMAL);
}

static void
cmd_exit_edit_mode(List<char>* args) {

	editor_change_mode(MODE_NAVIGATION);
}

void
commands_init() {

	hash_table_init(&Commands);
	hash_table_put(&Commands, "cursor-left", {cmd_cursor_left, 0, 0});
	hash_table_put(&Commands, "cursor-right", {cmd_cursor_right, 0, 0});
	hash_table_put(&Commands, "cursor-up", {cmd_cursor_up, 0, 0});
	hash_table_put(&Commands, "cursor-down", {cmd_cursor_down, 0, 0});
	hash_table_put(&Commands, "window-split-vertical", {cmd_window_split_vertical, 0, 0});
	hash_table_put(&Commands, "window-split-horizontal", {cmd_window_split_horizontal, 0, 0});
	hash_table_put(&Commands, "window-switch-up", {cmd_window_switch_up, 0, 0});
	hash_table_put(&Commands, "window-switch-down", {cmd_window_switch_down, 0, 0});
	hash_table_put(&Commands, "window-switch-left", {cmd_window_switch_left, 0, 0});
	hash_table_put(&Commands, "window-switch-right", {cmd_window_switch_right, 0, 0});
	hash_table_put(&Commands, "window-close", {cmd_window_close, 0, 0});
	hash_table_put(&Commands, "enter-edit-mode", {cmd_enter_edit_mode, 0, 0});
	hash_table_put(&Commands, "exit-edit-mode", {cmd_exit_edit_mode, 0, 0});
}

Command* 
command_get(String& cmdname) {

	str_push(&cmdname, '\0');
	return &hash_table_get(&Commands, cmdname.data);
}

void
command_handle(String& cmdname) {

	Command command = hash_table_get(&Commands, cmdname.data);
	if (command.cmd) {
		command.cmd(NULL);
	}
}
