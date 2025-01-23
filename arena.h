#ifndef ARENA_H
#define ARENA_H

typedef struct arena* Arena;

#include <stdio.h>
Arena create_arena(size_t default_size);
void* alloc_on_arena(Arena a, size_t size);
void  destroy_arena(Arena);
void  debug_arena(Arena);

#endif
