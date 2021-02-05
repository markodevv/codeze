#pragma once
#include "types.h"
#include "my_string.h"
#include "container.h"

typedef struct File {
  
	char* buffer;
	sizet lineCount;
	sizet size;
	String path;

} File;


void fileio_update_cwd();
File file_open(const char* path);
void file_save();
b8 file_exists(const char* path);
u8* image_load_png(const char* path, i32* x, i32* y, i32* bpp);
void image_free(u8* data);
Array<String> fileio_cwd_file_names();
Array<String> fileio_path_file_names(String& path);
b8 fileio_change_dir(String& cd);
String& fileio_get_cwd();
