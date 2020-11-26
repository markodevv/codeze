#pragma once

#ifdef WINDOWS_PLATFORM

#include <stdio.h>

#ifdef DEBUG

#define ASSERT(condition) \
	if (!(condition)) { \
		*(int *)0 = 0; \
	} 

#else

#define ASSERT(condition) 

#endif

#elif LINUX_PLATFORM


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
