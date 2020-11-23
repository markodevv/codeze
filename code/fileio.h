#pragma once
#include "types.h"

typedef struct File {
  
	char* buffer;

} File;

File* file_open(const char* path, const char* flags);
File* file_close(File* file);
u8* image_load_png(const char* path, i32* x, i32* y, i32* bpp);
void image_free(u8* data);
