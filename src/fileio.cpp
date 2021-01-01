#include "fileio.h"
#include "types.h"
#include "debug.h"

#include <dirent.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static DIR* Dir;
static struct dirent* entry;
static String WorkingDirectory;


void
fileio_init() {
  
	char* cwd = (char*)malloc(sizeof(char) * 512);
	cwd = getcwd(cwd, 512);
	WorkingDirectory = str_create(cwd);
	Dir = opendir(cwd);

	free(cwd);
}

b8
fileio_try_change_cwd(String& newcwd) {

	str_push(&newcwd, '\0');
	DIR* dir = opendir(newcwd.data);

	if (dir) {

		closedir(Dir);
		Dir = dir;
		WorkingDirectory = newcwd;

		return true;
	}
	return false;
}

void
fileio_cwd_file_names(Array<String>* arr) {

	array_init(arr, 2);

	while ((entry = readdir(Dir)) != NULL) {
		
		String filename = str_create(entry->d_name);
		array_push(arr, filename);
	}

	rewinddir(Dir);
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

String
fileio_get_cwd() {
	return WorkingDirectory;
}


u8*
image_load_png(const char* path, i32* x, i32* y, i32* bpp) {

	return stbi_load(path, x, y, bpp, 0);
	
}

void
image_free(u8* data) {
	
	stbi_image_free(data);

}



