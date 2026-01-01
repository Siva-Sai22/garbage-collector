#include "stack.h"
#include "stdlib.h"

void stack_free(stack_t *stack) {
    if (stack == NULL) {
        return;
    }
    free(stack->data);
    free(stack);
}

void stack_free_nulls(stack_t *stack) {
    size_t new_size = 0;
    
    for(int i=0; i<stack->size; i++) {
        if(stack->data[i] != NULL) {
            stack->data[new_size++] = stack->data[i];
        }
    }
    
    stack->size = new_size;
    
    for(int i=new_size; i<stack->capacity; i++) {
        stack->data[i] = NULL;
    }
}

stack_t *stack_new(size_t capacity) {
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    if (stack == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    stack->size = 0;
    stack->capacity = capacity;

    stack->data = malloc(capacity * sizeof(void *));
    if (stack->data == NULL) {
        // Memory allocation failed;
        free(stack); // Important
        return NULL;
    }

    return stack;
}

void stack_push(stack_t *stack, void *obj) {
    if (stack == NULL) {
        return;
    }

    if (stack->size == stack->capacity) {
        stack->capacity *= 2;

        void **new_data =
            realloc(stack->data, sizeof(void *) * stack->capacity);
        if (new_data == NULL) {
            // Memory allocation failed;
            stack->capacity /= 2;
            exit(1);
        }
        stack->data = new_data;
    }

    stack->data[stack->size] = obj;
    stack->size++;
}

void *stack_pop(stack_t *stack) {
    if (stack == NULL) {
        return NULL;
    }

    stack->size--;
    return stack->data[stack->size];
}
