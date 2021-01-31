#include "fileio.h"
#include "types.h"
#include "debug.h"


#ifdef LINUX_PLATFORM

#include <dirent.h>
#include <unistd.h>

#elif WINDOWS_PLATFORM

#include <io.h>
#include <../third_party/dirent/dirent.h>

#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define LONGEST_PATH_LENGTH 512

static DIR* Dir;
static String WorkingDirectory;


void
fileio_update_cwd() {
  
	char* cwd = (char*)malloc(sizeof(char) * LONGEST_PATH_LENGTH);

#ifdef LINUX_PLATFORM
	cwd = getcwd(cwd, LONGEST_PATH_LENGTH);
#elif WINDOWS_PLATFORM
	cwd = _getcwd(cwd, LONGEST_PATH_LENGTH);
#endif

	WorkingDirectory = str_create(cwd);
	Dir = opendir(cwd);

	free(cwd);
}

b8
fileio_change_dir(String& cd) {

	
	str_push(&cd, '\0');
	i32 err = chdir(cd.data);

	if (err == -1) {

		printf("Can't change dir with: %s \n", cd.data);
		return false;
	}
	char* cwd = (char*)malloc(sizeof(char) * LONGEST_PATH_LENGTH);
	cwd = getcwd(cwd, LONGEST_PATH_LENGTH);
	printf("new cwd: %s \n", cwd);
	fileio_update_cwd();
	free(cwd);

	return true;
}

Array<String>
fileio_cwd_file_names() {

	static struct dirent* entry;
	Array<String> arr;
	array_init(&arr, 10);

	while ((entry = readdir(Dir)) != NULL) {
		
		String filename = str_create(entry->d_name);
		array_push(&arr, filename);
	}

	rewinddir(Dir);

	return arr;
}

Array<String>
fileio_path_file_names(String& path) {

	static DIR* dir;
	dirent* entry;
	Array<String> arr;
	array_init(&arr, 10);

	dir = opendir(path.as_cstr());

	if (dir) {
		

		while ((entry = readdir(dir)) != NULL) {

			String filename = str_create(entry->d_name);
			array_push(&arr, filename);
		}

		closedir(dir);
		return arr;
	}

	return arr;
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

String&
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



