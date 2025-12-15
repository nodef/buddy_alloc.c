// This example shows how to use buddy_walk to iterate over the
// internal state of the allocator.
#define BUDDY_ALLOC_IMPLEMENTATION
#include <buddy_alloc.h>
#include <stdio.h>


// Callback function matching the signature required by buddy_walk.
// void *(fp)(void *ctx, void *addr, size_t slot_size, size_t allocated)
void *print_slot(void *ctx, void *addr, size_t slot_size, size_t allocated) {
  (void)ctx; // Unused
  printf("Slot at %p: size=%zu, allocated=%s\n", addr, slot_size,
         allocated ? "YES" : "NO");
  return NULL; // Return NULL to continue iteration
}


int main() {
  unsigned char arena[1024 * 16]; // 16KB
  struct buddy *buddy = buddy_embed(arena, sizeof(arena));

  if (!buddy) {
    return 1;
  }

  printf("--- Initial State ---\n");
  buddy_walk(buddy, print_slot, NULL);

  void *p1 = buddy_malloc(buddy, 2000);
  void *p2 = buddy_malloc(buddy, 100);

  printf("\n--- After Allocations ---\n");
  buddy_walk(buddy, print_slot, NULL);

  buddy_free(buddy, p1);

  printf("\n--- After Freeing p1 ---\n");
  buddy_walk(buddy, print_slot, NULL);

  return 0;
}
