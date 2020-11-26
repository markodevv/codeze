#include "fileio.h"
#include "types.h"
#include "my_string.h"
#include "debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>


File* file_open(const char* path, const char* flags) {

	FILE* fp = fopen(path, flags);
	ASSERT_MSG(fp, "Failed to load file ");

	fseek(fp, 0, SEEK_END);
	sizet size = ftell(fp);
	rewind(fp);

	File* file = malloc(sizeof(File));
	file->buffer = malloc(sizeof(char) * size);

	char line[1024];

	i32 cursor = 0;

	while (fgets(line, sizeof(line), fp) != NULL) {
		i32 i = 0;
		while (line[i] != '\0') {
			file->buffer[cursor] = line[i];
			cursor++;
			i++;
		}
	}
    file->buffer[cursor] = '\0';

	fclose(fp);

	return file;
  
}

File* file_close(File* file) {

	free(file->buffer);
	free(file);
	
}

u8* image_load_png(const char* path, i32* x, i32* y, i32* bpp) {

	return stbi_load(path, x, y, bpp, 0);
	
}

void image_free(u8* data) {
	
	stbi_image_free(data);

}
