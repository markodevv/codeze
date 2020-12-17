#pragma once
#include "types.h"
#include "my_string.h"

typedef struct File {
  
	string buffer;
	sizet lineCount;

} File;

File* file_open(const char* path, const char* flags);
void file_close(File* file);
u8* image_load_png(const char* path, i32* x, i32* y, i32* bpp);
void image_free(u8* data);
void open_directory(const char* path);

  /*
struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("."); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
  
    closedir(d
  */
