// Forward declaration
typedef struct oxy_object oxy_object_t;

typedef enum oxy_object_kind {
    INTEGER,
    FLOAT,
    STRING,
    VECTOR3
} oxy_object_kind_t;

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
} oxy_object_data_t;

typedef struct oxy_object {
    oxy_object_kind_t kind;
    oxy_object_data_t data;
} oxy_object_t;

oxy_object_t *new_oxy_interger(int value);
oxy_object_t *new_oxy_float(float value);
oxy_object_t *new_oxy_string(char *value);
oxy_object_t *new_oxy_vector3(oxy_object_t *x, oxy_object_t *y, oxy_object_t *z);