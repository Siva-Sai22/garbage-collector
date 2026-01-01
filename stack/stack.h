#ifndef stack_h
#define stack_h

#include <stddef.h>

typedef struct Stack {
    size_t size;
    size_t capacity;
    void **data;
} stack_t;

stack_t *stack_new(size_t capacity);

void stack_push(stack_t *stack, void *obj);
void *stack_pop(stack_t *stack);

void stack_free(stack_t *stack);
void stack_remove_nulls(stack_t *stack);

#endif
