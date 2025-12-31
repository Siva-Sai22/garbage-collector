#include "oxy_object.h"
#include "oxy_utils.h"
#include "stdlib.h"
#include "string.h"

oxy_object_t *_new_oxy_object(vm_t *vm) {
    oxy_object_t *obj = malloc(sizeof(oxy_object_t));
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    vm_track_object(vm, obj);

    return obj;
}

void oxy_object_free(oxy_object_t *obj) {
    switch (obj->kind) {
    case INTEGER:
    case FLOAT:
        break;
    case STRING:
        free(obj->data.v_string);
        break;
    case VECTOR3:
        break;
    case ARRAY:
        free(obj->data.v_array.elements);
        break;
    }

    free(obj);
}

int oxy_length(oxy_object_t *obj) {
    switch (obj->kind) {
    case INTEGER:
    case FLOAT:
        return 1;
    case STRING:
        return strlen(obj->data.v_string);
    case VECTOR3:
        return 3;
    case ARRAY:
        return obj->data.v_array.size;
    default:
        return -1;
    }
}

oxy_object_t *oxy_add(vm_t *vm, oxy_object_t *a, oxy_object_t *b) {
    if (a == NULL || b == NULL)
        return NULL;

    if (a->kind == INTEGER) {
        if (b->kind == INTEGER) {
            return new_oxy_integer(vm, a->data.v_int + b->data.v_int);
        } else if (b->kind == FLOAT) {
            return new_oxy_float(vm, a->data.v_int + b->data.v_float);
        } else
            return NULL;
    }

    if (a->kind == FLOAT) {
        if (b->kind == FLOAT) {
            return new_oxy_float(vm, a->data.v_float + b->data.v_float);
        } else if (b->kind == INTEGER) {
            return new_oxy_float(vm, a->data.v_float + b->data.v_int);
        } else
            return NULL;
    }

    if (a->kind == STRING) {
        if (b->kind != STRING)
            return NULL;

        int tot_len = oxy_length(a) + oxy_length(b);
        char *new_str = calloc(tot_len + 1, sizeof(char));
        strcat(new_str, a->data.v_string);
        strcat(new_str, b->data.v_string);

        oxy_object_t *obj = new_oxy_string(vm, new_str);
        free(new_str);
        return obj;
    }

    if (a->kind == VECTOR3) {
        if (b->kind != VECTOR3)
            return NULL;

        oxy_object_t *obj = new_oxy_vector3(
            vm, oxy_add(vm, a->data.v_vector3.x, b->data.v_vector3.x),
            oxy_add(vm, a->data.v_vector3.y, b->data.v_vector3.y),
            oxy_add(vm, a->data.v_vector3.z, b->data.v_vector3.z));

        return obj;
    }

    if (a->kind == ARRAY) {
        if (b->kind != ARRAY)
            return NULL;
        int len_a = oxy_length(a), len_b = oxy_length(b);
        int tot_len = len_a + len_b;
        oxy_object_t *obj = new_oxy_array(vm, tot_len);

        for (int i = 0; i < len_a; i++) {
            oxy_array_set(obj, i, oxy_array_get(a, i));
        }
        for (int i = 0; i < len_b; i++) {
            oxy_array_set(obj, i + len_a, oxy_array_get(b, i));
        }

        return obj;
    }

    return NULL;
}

oxy_object_t *new_oxy_integer(vm_t *vm, int value) {
    oxy_object_t *obj = _new_oxy_object(vm);
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    obj->kind = INTEGER;
    obj->data.v_int = value;

    return obj;
}

oxy_object_t *new_oxy_float(vm_t *vm, float value) {
    oxy_object_t *obj = _new_oxy_object(vm);
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    obj->kind = FLOAT;
    obj->data.v_float = value;

    return obj;
}

oxy_object_t *new_oxy_string(vm_t *vm, char *value) {
    oxy_object_t *obj = _new_oxy_object(vm);
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    int len = strlen(value);
    char *str = (char *)malloc(len + 1);
    if (str == NULL) {
        // Memory allocation failed;
        free(obj);
        return NULL;
    }

    strcpy(str, value);
    obj->kind = STRING;
    obj->data.v_string = str;

    return obj;
}

oxy_object_t *new_oxy_vector3(vm_t *vm, oxy_object_t *x, oxy_object_t *y,
                              oxy_object_t *z) {
    if (x == NULL || y == NULL || z == NULL) {
        return NULL;
    }
    oxy_object_t *obj = _new_oxy_object(vm);
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    obj->kind = VECTOR3;
    oxy_vector_t vec = {.x = x, .y = y, .z = z};
    obj->data.v_vector3 = vec;

    return obj;
}

oxy_object_t *new_oxy_array(vm_t *vm, size_t size) {
    oxy_object_t *obj = _new_oxy_object(vm);
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    oxy_object_t **arr_ptr = calloc(size, sizeof(oxy_object_t *));
    if (arr_ptr == NULL) {
        // Memory allocation failed;
        free(obj);
        return NULL;
    }

    obj->kind = ARRAY;
    oxy_array_t arr = {.size = size, .elements = arr_ptr};
    obj->data.v_array = arr;

    return obj;
}

bool oxy_array_set(oxy_object_t *obj, size_t index, oxy_object_t *value) {
    if (obj == NULL || value == NULL) {
        return false;
    }
    if (obj->kind != ARRAY) {
        return false;
    }
    if (index >= obj->data.v_array.size) {
        return false;
    }

    oxy_object_t **arr = obj->data.v_array.elements;
    arr[index] = value;

    return true;
}

oxy_object_t *oxy_array_get(oxy_object_t *obj, size_t index) {
    if (obj == NULL || obj->kind != ARRAY) {
        return NULL;
    }
    if (index >= obj->data.v_array.size) {
        return NULL;
    }
    return obj->data.v_array.elements[index];
}
