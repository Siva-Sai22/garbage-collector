#include "vm.h"
#include "../stack/stack.h"
#include "oxy_object.h"
#include "oxy_utils.h"
#include <stdlib.h>

void vm_collect_garbage(vm_t *vm) {
    mark(vm);
    trace(vm);
    sweep(vm);
}

void mark(vm_t *vm) {
    stack_t *frames = vm->frames;
    for (int i = 0; i < frames->size; i++) {
        frame_t *frame = frames->data[i];
        stack_t *references = frame->references;
        for (int j = 0; j < references->size; j++) {
            oxy_object_t *obj = references->data[i];
            obj->is_marked = true;
        }
    }
}

void trace(vm_t *vm) {
    stack_t *gray_objects = stack_new(8);
    if (gray_objects == NULL) {
        return;
    }

    for (int i = 0; i < vm->objects->size; i++) {
        oxy_object_t *obj = vm->objects->data[i];
        if (obj->is_marked) {
            stack_push(gray_objects, obj);
        }
    }

    while (gray_objects->size > 0) {
        oxy_object_t *obj = stack_pop(gray_objects);
        trace_blacken_object(gray_objects, obj);
    }

    stack_free(gray_objects);
}

void sweep(vm_t *vm) {
    for (int i = 0; i < vm->objects->size; i++) {
        oxy_object_t *obj = vm->objects->data[i];
        if (obj->is_marked) {
            obj->is_marked = false;
        } else {
            oxy_object_free(obj);
            vm->objects->data[i] = NULL;
        }
    }
    stack_remove_nulls(vm->objects);
}

void trace_blacken_object(stack_t *gray_objects, oxy_object_t *obj) {
    switch (obj->kind) {
    case INTEGER:
    case FLOAT:
    case STRING:
        break;
    case VECTOR3: {
        oxy_vector_t vec = obj->data.v_vector3;
        trace_mark_object(gray_objects, vec.x);
        trace_mark_object(gray_objects, vec.y);
        trace_mark_object(gray_objects, vec.z);
        break;
    }
    case ARRAY: {
        oxy_array_t arr = obj->data.v_array;
        for (int i = 0; i < arr.size; i++) {
            trace_mark_object(gray_objects, arr.elements[i]);
        }
        break;
    }
    default:
        return;
    }
}

void trace_mark_object(stack_t *gray_objects, oxy_object_t *obj) {
    if (obj == NULL) {
        return;
    }

    obj->is_marked = true;
    stack_push(gray_objects, obj);
}

vm_t *vm_new() {
    vm_t *vm = malloc(sizeof(vm_t));
    vm->frames = stack_new(8);
    vm->objects = stack_new(8);

    return vm;
}

void vm_free(vm_t *vm) {
    for (int i = 0; i < vm->frames->size; i++) {
        frame_free(vm->frames->data[i]);
    }

    for (int i = 0; i < vm->objects->size; i++) {
        oxy_object_free(vm->objects->data[i]);
    }

    stack_free(vm->frames);
    stack_free(vm->objects);

    free(vm);
}

void vm_track_object(vm_t *vm, oxy_object_t *obj) {
    stack_push(vm->objects, obj);
}

void vm_frame_push(vm_t *vm, frame_t *frame) { stack_push(vm->frames, frame); }

frame_t *vm_new_frame(vm_t *vm) {
    frame_t *frame = malloc(sizeof(frame_t));
    frame->references = stack_new(8);

    vm_frame_push(vm, frame);
    return frame;
}

void frame_free(frame_t *frame) {
    stack_free(frame->references);
    free(frame);
}

void frame_reference_object(frame_t *frame, oxy_object_t *obj) {
    stack_push(frame->references, obj);
}
