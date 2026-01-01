#ifndef vm_h
#define vm_h

#include "../stack/stack.h"
#include "oxy_object.h"

// Tracks the state of oxy program if it is a fully funtional intepreted
// language
typedef struct VirtualMachine {
    stack_t *frames;
    stack_t *objects;
} vm_t;

// Each object in frames is a stack of these frame_t (object references)
typedef struct StackFrame {
    // Object references in a frame
    stack_t *references;
} frame_t;

// Main functions for garbage collection
void mark(vm_t *vm);
void trace(vm_t *vm);
void sweep(vm_t *vm);

// Helper functions for trace
void trace_blacken_object(stack_t *gray_objects, oxy_object_t *ref);
void trace_mark_object(stack_t *gray_objects, oxy_object_t *ref);

// Actual function called to do the garbage collection
void vm_collect_garbage(vm_t *vm);

vm_t *vm_new();
void vm_free(vm_t *vm);
void vm_track_object(vm_t *vm, oxy_object_t *obj);

void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t *vm_new_frame(vm_t *vm);
void frame_free(frame_t *frame);

// Mark the object as referenced in the current stack frame
void frame_reference_object(frame_t *frame, oxy_object_t *obj);

#endif
