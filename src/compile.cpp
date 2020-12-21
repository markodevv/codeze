#include <stdint.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

#include "types.h"

#define DO_INIT
#include "globals.h"
#undef DO_INIT
#include "renderer.cpp"
#include "buffer.cpp"
#include "shader.cpp"
#include "cursor.cpp"
#include "event.cpp"
#include "dispatch_table.cpp"
#include "fileio.cpp"
#include "tokenizer.cpp"
#include "container.cpp"
#include "cmdstate.cpp"
#include "codeze.cpp"
#include "my_string.cpp"
#include "math.cpp"
#include "window.cpp"
