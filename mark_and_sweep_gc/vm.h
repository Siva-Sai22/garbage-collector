#include "../stack/stack.h"
#include "oxy_object.h"

// Tracks the state of oxy program if it is a fully funtional intepreted
// language
typedef struct VirtualMachine {
    stack_t *frames;
    stack_t *objects;
} vm_t;

typedef struct StackFrame {
    // Object references in a frame
    stack_t *references;
} frame_t;

vm_t *vm_new();
void vm_free(vm_t *vm);
void vm_track_object(vm_t *vm, oxy_object_t *obj);

void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t *vm_new_frame(vm_t *vm);

void frame_free(frame_t *frame);
