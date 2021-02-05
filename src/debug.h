#pragma once


	#ifdef WINDOWS_PLATFORM

		#include <stdio.h>


        #ifdef DEBUG

			#define ASSERT(condition) \
				if (!(condition)) { \
					*(int *)0 = 0; \
				} 
			#define ASSERT_MSG(condition, message) \
				if (!(condition)) { \
					printf("%s", message); \
					*(int *)0 = 0; \
				} 

        #else

			#define ASSERT(condition) 
			#define ASSERT_MSG(condition, message) 

        #endif

    #endif

	#ifdef LINUX_PLATFORM


        #ifdef DEBUG

			#include "assert.h"

			#include <stdio.h>

			#define ASSERT_MSG(condition, message) \
				assert(condition && message)
			#define ASSERT(condition) \
				assert(condition)

        #else

			#define ASSERT(condition) 
			#define ASSERT_MSG(condition) 

        #endif

    #endif

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef DEBUG

	#define ALERT_MSG(msg, ...) \
		printf(ANSI_COLOR_RED msg ANSI_COLOR_RESET, __VA_ARGS__)
	#define WARN_MSG(msg, ...) \
		printf(ANSI_COLOR_YELLOW msg ANSI_COLOR_RESET, __VA_ARGS__)
	#define NORMAL_MSG(msg, ...) \
		printf(ANSI_COLOR_GREEN msg ANSI_COLOR_RESET, __VA_ARGS__)

#else

	#define ALERT_MSG(msg, ...) 
	#define WARN_MSG(msg, ...)
	#define NORMAL_MSG(msg, ...)

#endif
