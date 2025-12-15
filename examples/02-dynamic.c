// This example demonstrates using buddy_alloc with a dynamically allocated
// arena. This is useful when the arena size is not known at compile time, or is
// too large for the stack.
#define BUDDY_ALLOC_IMPLEMENTATION
#include <buddy_alloc.h>
#include <stdio.h>
#include <stdlib.h>


int main() {
  size_t arena_size = 1024 * 1024; // 1MB

  // 1. Allocate the arena using system malloc
  unsigned char *arena_memory = (unsigned char *)malloc(arena_size);
  if (!arena_memory) {
    perror("System malloc failed");
    return 1;
  }

  // 2. Initialize buddy allocator
  struct buddy *buddy = buddy_embed(arena_memory, arena_size);
  if (!buddy) {
    fprintf(stderr, "Buddy init failed\n");
    free(arena_memory);
    return 1;
  }

  printf("Managed heap arena of size: %zu\n", buddy_arena_size(buddy));

  // 3. Perform a series of allocations
  void *pointers[10];
  for (int i = 0; i < 10; ++i) {
    pointers[i] =
        buddy_malloc(buddy, 100 * (i + 1)); // 100, 200, ... 1000 bytes
    if (pointers[i]) {
      printf("Allocated %d bytes\n", 100 * (i + 1));
    } else {
      printf("Failed allocation at index %d\n", i);
    }
  }

  // 4. Release them in reverse order
  for (int i = 9; i >= 0; --i) {
    if (pointers[i]) {
      buddy_free(buddy, pointers[i]);
    }
  }

  printf("All freed. Free space: %zu\n", buddy_arena_free_size(buddy));

  // 5. Clean up the main arena
  free(arena_memory);

  return 0;
}
