#pragma once
#include <stddef.h>
 
void* my_malloc(size_t size);
void* my_realloc(void* ptr, size_t size);
void* my_calloc(size_t nb, size_t size);
void my_free(void* ptr);
void showDebugInfo();