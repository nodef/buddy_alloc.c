// This example demonstrates the most basic usage of buddy_alloc:
// 1. Defining a memory arena (a static buffer in this case).
// 2. Embedding the buddy allocator metadata into that arena.
// 3. allocating and freeing memory.
#define BUDDY_ALLOC_IMPLEMENTATION
#include <buddy_alloc.h>
#include <stdio.h>

// 256KB arena
#define ARENA_SIZE (256 * 1024)


int main() {
    // 1. Prepare the memory arena
    unsigned char arena[ARENA_SIZE];

    // 2. Initialize the buddy allocator inside the arena
    // buddy_embed will place the allocator data structures at the beginning of 'arena'
    // and return a handle to it.
    struct buddy *buddy = buddy_embed(arena, ARENA_SIZE);

    if (!buddy) {
        fprintf(stderr, "Failed to initialize buddy allocator\n");
        return 1;
    }

    printf("Buddy allocator initialized. Total arena size: %zu bytes\n", buddy_arena_size(buddy));
    printf("Free space: %zu bytes\n", buddy_arena_free_size(buddy));

    // 3. Allocate some memory
    void *ptr1 = buddy_malloc(buddy, 1024);
    if (ptr1) {
        printf("Allocated 1024 bytes at %p\n", ptr1);
    } else {
        printf("Failed to allocate 1024 bytes\n");
    }

    void *ptr2 = buddy_malloc(buddy, 500);
    if (ptr2) {
        printf("Allocated 500 bytes at %p\n", ptr2);
    } else {
        printf("Failed to allocate 500 bytes\n");
    }

    // 4. Free memory
    if (ptr1) {
        buddy_free(buddy, ptr1);
        printf("Freed ptr1\n");
    }
    if (ptr2) {
        buddy_free(buddy, ptr2);
        printf("Freed ptr2\n");
    }

    printf("Final free space: %zu bytes\n", buddy_arena_free_size(buddy));

    return 0;
}
