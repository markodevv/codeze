#pragma once
#include "types.h"
#include "container.h"

struct String;
void completion_init();
void completion_add(String& word);
void completion_reset();
void completion_complete();
Array<String> completion_get_matching(String& word);
