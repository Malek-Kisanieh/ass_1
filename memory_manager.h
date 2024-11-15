// This file is a **header file** for a custom memory manager in C. 
// Header files contain declarations of functions, types, and macros, allowing other parts of a program to access them by including this file with `#include`. Below is the code with comments explaining each part:

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h> // Includes the standard library for size_t, which represents sizes in bytes

// Function prototypes for the memory manager

// Initializes a memory pool of a given size
// Parameters:
// - size: the total size to allocate for the memory pool in bytes
void mem_init(size_t size);

// Allocates a block of memory from the pool of the specified size
// Parameters:
// - size: the size of memory to allocate in bytes
// Returns:
// - A pointer to the allocated memory block if successful, or NULL if allocation fails
void* mem_alloc(size_t size);

// Frees a previously allocated memory block within the pool
// Parameters:
// - block: pointer to the memory block to free
void mem_free(void* block);

// Resizes a previously allocated memory block within the pool
// Parameters:
// - block: pointer to the memory block to resize
// - size: the new size to allocate in bytes
// Returns:
// - A pointer to the resized memory block if successful, or NULL if resizing fails
void* mem_resize(void* block, size_t size);

// Deinitializes the memory pool and releases all resources
// This should be called to clean up after using the memory pool
void mem_deinit(void);

#endif // MEMORY_MANAGER_H

// ## Explanation of Each Section

// 1. **Include Guards (`#ifndef`, `#define`, `#endif`)**:
//    - `#ifndef MEMORY_MANAGER_H` and `#define MEMORY_MANAGER_H` ensure that this file is included only once in each compilation, preventing duplicate definitions.
//    - `#endif` closes this conditional compilation block.

// 2. **Library Inclusion (`#include <stddef.h>`)**:
//    - Includes the `stddef.h` library, which provides the `size_t` type. `size_t` is an unsigned integer type used for representing the sizes of objects and is widely used in memory management functions.

// 3. **Function Prototypes**:
//    - **`void mem_init(size_t size);`**: Initializes a memory pool with a specified size. This should be called first to allocate memory for the pool.
//    - **`void* mem_alloc(size_t size);`**: Allocates a block of memory within the pool. If successful, it returns a pointer to the allocated block; otherwise, it returns `NULL`.
//    - **`void mem_free(void* block);`**: Frees a previously allocated memory block, making it available for future allocations.
//    - **`void* mem_resize(void* block, size_t size);`**: Changes the size of an allocated memory block. Returns a pointer to the resized memory block or `NULL` if resizing fails.
//    - **`void mem_deinit(void);`**: Cleans up the memory pool by releasing all allocated resources. This should be called when the memory pool is no longer needed to prevent memory leaks.

// This header file provides the declarations needed to use a custom memory manager in a program, with each function handling a specific aspect of memory allocation and management.