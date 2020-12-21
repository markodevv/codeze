#include "fileio.h"
#include "types.h"
#include "debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static DIR* dir;
static struct dirent* entry;


void
fileio_init() {
  
	char* cwd = (char*)malloc(sizeof(char) * 256);
	cwd = getcwd(cwd, 256);
	WorkingDirectory = str_create(cwd);
	dir = opendir(cwd);

	free(cwd);
}

void
fileio_cwd_file_names(Array<String>* arr) {

	array_init(arr, 2);

	while ((entry = readdir(dir)) != NULL) {
		
		String filename = str_create(entry->d_name);
		array_push(arr, filename);
	}

	rewinddir(dir);
}

File
file_open(const char* path, const char* flags) {

	FILE* fp = fopen(path, flags);
	ASSERT_MSG(fp, "Failed to load file ");

	fseek(fp, 0, SEEK_END);
	sizet size = ftell(fp);
	rewind(fp);

	File file;

	file.buffer = str_create(size);
	file.lineCount = 0;

	char line[4096];

	while (fgets(line, sizeof(line), fp) != NULL) {

		sizet i = 0;
		while (line[i] != '\0') {

			str_push(&file.buffer, line[i]);
			i++;
		}
		file.lineCount++;
	}

	fclose(fp);

	return file;
  
}


u8*
image_load_png(const char* path, i32* x, i32* y, i32* bpp) {

	return stbi_load(path, x, y, bpp, 0);
	
}

void
image_free(u8* data) {
	
	stbi_image_free(data);

}



