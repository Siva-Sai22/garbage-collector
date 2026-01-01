#include "../munit/munit.h"
#include "oxy_object.h"

// AAA: Arrage Act Assert

MunitResult test_array_set() {
    oxy_object_t *ele = new_oxy_integer(5);
    oxy_object_t *arr = new_oxy_array(3);
    int index = 0;

    oxy_array_set(arr, index, ele);

    munit_assert(arr->data.v_array.elements[index] == ele);
    return MUNIT_OK;
}

MunitTest tests[] = {{"/test_array_set", test_array_set, NULL, NULL,
                      MUNIT_TEST_OPTION_NONE, NULL},
                     {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

const MunitSuite suite = {"/ref_gc_tests", tests, NULL, 1,
                          MUNIT_SUITE_OPTION_NONE};

int main(int argc, char *argv[]) { return munit_suite_main(&suite, NULL, argc, argv); }
