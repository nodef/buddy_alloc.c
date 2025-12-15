// This example demonstrates how to measure fragmentation within the allocator.
#define BUDDY_ALLOC_IMPLEMENTATION
#include <buddy_alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARENA_SIZE (64 * 1024)


int main() {
  srand((unsigned int)time(NULL));
  unsigned char arena[ARENA_SIZE];
  struct buddy *buddy = buddy_embed(arena, sizeof(arena));

  if (!buddy) {
    return 1;
  }

  printf("Initial fragmentation: %d\n", buddy_fragmentation(buddy));

  // Allocate many small blocks
  void *ptrs[50];
  for (int i = 0; i < 50; ++i) {
    ptrs[i] = buddy_malloc(buddy, 128 + (rand() % 128));
  }

  printf("Fragmentation after allocations: %d\n", buddy_fragmentation(buddy));

  // Free every other block to create "holes" (fragmentation)
  for (int i = 0; i < 50; i += 2) {
    if (ptrs[i]) {
      buddy_free(buddy, ptrs[i]);
      ptrs[i] = NULL;
    }
  }

  printf("Fragmentation after creating holes: %d\n",
         buddy_fragmentation(buddy));

  // Clean up remaining
  for (int i = 0; i < 50; ++i) {
    if (ptrs[i]) {
      buddy_free(buddy, ptrs[i]);
    }
  }

  printf("Final fragmentation (should be 0): %d\n", buddy_fragmentation(buddy));

  return 0;
}
