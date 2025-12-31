#include "oxy_object.h"
#include "vm.h"

int oxy_length(oxy_object_t *obj);
oxy_object_t *oxy_add(vm_t *vm, oxy_object_t *a, oxy_object_t *b);
oxy_object_t *_new_oxy_object(vm_t *vm);

oxy_object_t *new_oxy_integer(vm_t *vm, int value);
oxy_object_t *new_oxy_float(vm_t *vm, float value);
oxy_object_t *new_oxy_string(vm_t *vm, char *value);
oxy_object_t *new_oxy_vector3(vm_t *vm, oxy_object_t *x, oxy_object_t *y,
                              oxy_object_t *z);
oxy_object_t *new_oxy_array(vm_t *vm, size_t size);

void oxy_object_free(oxy_object_t *obj);

bool oxy_array_set(oxy_object_t *arr, size_t index, oxy_object_t *value);
oxy_object_t *oxy_array_get(oxy_object_t *arr, size_t index);
