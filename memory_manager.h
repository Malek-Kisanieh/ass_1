// This file is a **header file** for a custom memory manager in C. 
// Header files contain declarations of functions, types, and macros, allowing other parts of a program to access them by including this file with `#include`. Below is the code with comments explaining each part:

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h> // Includes the standard library for size_t, which represents sizes in bytes
void mem_init(size_t size);
void* mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit(void);

#endif // MEMORY_MANAGER_H

