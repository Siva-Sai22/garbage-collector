#include "oxyobject.h"
#include "stdlib.h"
#include "string.h"

oxy_object_t *new_oxy_interger(int value) {
    oxy_object_t *obj = (oxy_object_t *)malloc(sizeof(oxy_object_t));
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    obj->kind = INTEGER;
    obj->data.v_int = value;

    return obj;
}

oxy_object_t *new_oxy_float(float value) {
    oxy_object_t *obj = (oxy_object_t *)malloc(sizeof(oxy_object_t));
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    obj->kind = FLOAT;
    obj->data.v_float = value;

    return obj;
}

oxy_object_t *new_oxy_string(char *value) {
    oxy_object_t *obj = (oxy_object_t *)malloc(sizeof(oxy_object_t));
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

oxy_object_t *new_oxy_vector3(oxy_object_t *x, oxy_object_t *y,
                              oxy_object_t *z) {
    if (x == NULL || y == NULL || z == NULL) {
        return NULL;
    }
    oxy_object_t *obj = (oxy_object_t *)malloc(sizeof(oxy_object_t));
    if (obj == NULL) {
        // Memory allocation failed;
        return NULL;
    }

    obj->kind = VECTOR3;
    oxy_vector_t vec = {.x = x, .y = y, .z = z};
    obj->data.v_vector3 = vec;

    return obj;
}
