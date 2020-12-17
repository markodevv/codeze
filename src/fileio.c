#include "fileio.h"
#include "types.h"
#include "debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>


File*
file_open(const char* path, const char* flags) {

	FILE* fp = fopen(path, flags);
	ASSERT_MSG(fp, "Failed to load file ");

	fseek(fp, 0, SEEK_END);
	sizet size = ftell(fp);
	rewind(fp);

	File* file = malloc(sizeof(File));
	ASSERT(file);

	file->buffer = str_create_s(size);
	file->lineCount = 0;

	char line[1024];
	sizet cursor = 0;

	while (fgets(line, sizeof(line), fp) != NULL) {

		sizet i = 0;
		while (line[i] != '\0') {

			file->buffer = str_push(file->buffer, line[i]);
			i++;
		}
		file->lineCount++;
	}

	fclose(fp);

	return file;
  
}

void
file_close(File* file) {

	str_release(file->buffer);
	free(file);
	
}

u8*
image_load_png(const char* path, i32* x, i32* y, i32* bpp) {

	return stbi_load(path, x, y, bpp, 0);
	
}

void
image_free(u8* data) {
	
	stbi_image_free(data);

}

static DIR* dir;
static struct dirent* entry;

void
open_directory(const char* path) {

	dir = opendir(path);

	if (!dir) {
		
		printf("Cound't load %s directory \n", path);
	} else {
		printf("File loaded %s \n", path);
	}

}

void
open_new_file(const char* path) {

	
}

