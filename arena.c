#include "arena.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 32768              //32KB Arena Default

struct arena{
    size_t            capacity;
    size_t            used;
    struct arena*     next;
    unsigned char*    buffer;
};

#define handle_error(trigger, x, expr)                  \
    do{                                                 \
        if ( trigger ){                                 \
            fprintf(stderr, "\e[31m%s\e[m\n", (x));     \
            expr;                                       \
        }                                               \
    }while(0);


Arena create_arena(size_t default_size){
    if ( default_size == 0 ){
        default_size = DEFAULT_SIZE;
    }
    Arena a = (Arena)malloc(sizeof(struct arena));
    handle_error(
            a == NULL,
            "Error allocating space for the Arena ...",
            return NULL;
    );
    a -> used = 0;
    a -> next = NULL;
    a -> capacity = default_size;
    a -> buffer = malloc(a -> capacity); 
    handle_error(
        a -> buffer == NULL,
        "Error allocating space for the Arena ...",
        free(a);
        return NULL;
    );
    return a;
}

void *alloc_on_arena(Arena a, size_t size){
    handle_error(
        size > a -> capacity,
        "Arena Capacity is inferior to pointer size ...",
        return NULL;
    )
    if ( a -> used + size > a -> capacity ){
        a -> next = create_arena(a -> capacity);
        return alloc_on_arena(a -> next, size);
    }
    void* output = a -> buffer + a -> used;
    a -> used += size;
    return output;
}

void destroy_arena(Arena a){
    Arena x = a;
    Arena next;
    while( x != NULL ){
        next = x -> next;
        free(x->buffer);
        free(x);
        x = next;
    }
} 

void debug_arena(Arena a){
    printf("[");
    for(unsigned i = 0; i < a -> capacity; i++){
        printf("%x", a -> buffer[i]);
    }
    printf("]");
    if ( a -> next != NULL ){
        printf("->");
        debug_arena(a -> next);
    }
    printf("\n");
}
