#include "stdbool.h"
#include "stddef.h"

// Forward declaration
typedef struct oxy_object oxy_object_t;

int oxy_length(oxy_object_t *obj);
oxy_object_t *oxy_add(oxy_object_t *a, oxy_object_t *b);
oxy_object_t *_new_oxy_objec();
void refcount_inc(oxy_object_t *obj);
void refcount_dec(oxy_object_t *obj);
void refcount_free(oxy_object_t *obj);

// Datatypes in oxy
typedef enum oxy_object_kind {
    INTEGER,
    FLOAT,
    STRING,
    VECTOR3,
    ARRAY
} oxy_object_kind_t;

// Dynamic array
typedef struct oxy_array {
    size_t size;
    oxy_object_t **elements;
} oxy_array_t;

// Kind of tuple
typedef struct oxy_vector {
    oxy_object_t *x;
    oxy_object_t *y;
    oxy_object_t *z;
} oxy_vector_t;

typedef union oxy_object_data {
    int v_int;
    float v_float;
    char *v_string;
    oxy_vector_t v_vector3;
    oxy_array_t v_array;
} oxy_object_data_t;

// Basic object of the oxy
typedef struct oxy_object {
    oxy_object_kind_t kind;
    oxy_object_data_t data;
    int refcount;
} oxy_object_t;

oxy_object_t *new_oxy_integer(int value);
oxy_object_t *new_oxy_float(float value);
oxy_object_t *new_oxy_string(char *value);
oxy_object_t *new_oxy_vector3(oxy_object_t *x, oxy_object_t *y,
                              oxy_object_t *z);
oxy_object_t *new_oxy_array(size_t size);
bool oxy_array_set(oxy_object_t *arr, size_t index, oxy_object_t *value);
oxy_object_t *oxy_array_get(oxy_object_t *arr, size_t index);
