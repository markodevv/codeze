#pragma once
#include "types.h"
#include "my_string.h"

typedef struct File {
  
	String buffer;
	sizet lineCount;

} File;

extern String WorkingDirectory;

void fileio_init();
File* file_open(const char* path, const char* flags);
void file_close(File* file);
u8* image_load_png(const char* path, i32* x, i32* y, i32* bpp);
void image_free(u8* data);
void open_directory(const char* path);
String directory_filename_next();
