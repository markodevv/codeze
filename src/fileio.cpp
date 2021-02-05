#include "fileio.h"
#include "types.h"
#include "debug.h"
#include "globals.h"


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

		ALERT_MSG("Can't change dir with: %s \n", cd.data);
		return false;
	}
	char* cwd = (char*)malloc(sizeof(char) * LONGEST_PATH_LENGTH);
	cwd = getcwd(cwd, LONGEST_PATH_LENGTH);
	NORMAL_MSG("Changed cwd to : %s \n", cwd);
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
file_open(const char* path) {

	FILE* fp = fopen(path, "r");

	fseek(fp, 0, SEEK_END);
	sizet size = ftell(fp);
	rewind(fp);

	if (!fp) {
		
		WARN_MSG("Invalid file path %s \n", path);
	}

	File file;
	file.size = size;
	file.buffer = (char*)malloc(sizeof(char) * (size + 1));
	file.lineCount = 0;
	file.path = str_create(path);

	char line[4096];
	sizet cursor = 0;

	while (fgets(line, sizeof(line), fp) != NULL) {

		sizet i = 0;
		while (line[i] != '\0') {

			file.buffer[cursor] = line[i];
			i++;
			cursor++;
		}
		file.lineCount++;
	}

	file.buffer[size] = '\0';

	fclose(fp);

	return file;
  
}

void
file_save() {
	
	String data = buffer_get_text_copy(CurBuffer);
	String path = CurBuffer->path;
	FILE* fp = fopen(path.as_cstr(), "w");

	if (fp) {
		NORMAL_MSG("File contents: \n%s \n", data.as_cstr());
		fprintf(fp, "%s", data.as_cstr());
		NORMAL_MSG("File saved: %s \n", path.as_cstr());
	}
	else {
		
		ALERT_MSG("Failed to save: %s \n", path.as_cstr());
	}

	fclose(fp);
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



