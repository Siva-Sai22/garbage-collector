#include <stdlib.h>

#include "munit/munit.h"
#include "oxyobject.h"

/* ========================================================================= */
/* TEST CASES                                                                */
/* ========================================================================= */

static MunitResult
test_integer_add(const MunitParameter params[], void* user_data) {
  (void)params; (void)user_data;

  oxy_object_t *one = new_oxy_integer(1);
  oxy_object_t *three = new_oxy_integer(3);
  oxy_object_t *four = oxy_add(one, three);

  munit_assert_not_null(four);
  munit_assert_int(four->kind, ==, INTEGER);
  munit_assert_int(four->data.v_int, ==, 4);

  free(one);
  free(three);
  free(four);
  
  return MUNIT_OK;
}

static MunitResult
test_float_add(const MunitParameter params[], void* user_data) {
  (void)params; (void)user_data;

  oxy_object_t *one = new_oxy_float(1.5);
  oxy_object_t *three = new_oxy_float(3.5);
  oxy_object_t *five = oxy_add(one, three);

  munit_assert_not_null(five);
  munit_assert_int(five->kind, ==, FLOAT);
  munit_assert_float(five->data.v_float, ==, 5.0);

  free(one);
  free(three);
  free(five);
  

  return MUNIT_OK;
}

static MunitResult
test_string_add(const MunitParameter params[], void* user_data) {
  (void)params; (void)user_data;

  oxy_object_t *hello = new_oxy_string("hello");
  oxy_object_t *world = new_oxy_string(", world");
  oxy_object_t *greeting = oxy_add(hello, world);

  munit_assert_not_null(greeting);
  munit_assert_int(greeting->kind, ==, STRING);
  munit_assert_string_equal(greeting->data.v_string, "hello, world");

  free(hello->data.v_string);
  free(hello);
  free(world->data.v_string);
  free(world);
  free(greeting->data.v_string);
  free(greeting);

  return MUNIT_OK;
}

static MunitResult
test_string_add_self(const MunitParameter params[], void* user_data) {
  (void)params; (void)user_data;

  oxy_object_t *repeated = new_oxy_string("(repeated)");
  oxy_object_t *result = oxy_add(repeated, repeated);

  munit_assert_not_null(result);
  munit_assert_int(result->kind, ==, STRING);
  munit_assert_string_equal(result->data.v_string, "(repeated)(repeated)");

  free(repeated->data.v_string);
  free(repeated);
  free(result->data.v_string);
  free(result);

  return MUNIT_OK;
}

static MunitResult
test_vector3_add(const MunitParameter params[], void* user_data) {
  (void)params; (void)user_data;

  oxy_object_t *one = new_oxy_float(1.0);
  oxy_object_t *two = new_oxy_float(2.0);
  oxy_object_t *three = new_oxy_float(3.0);
  oxy_object_t *four = new_oxy_float(4.0);
  oxy_object_t *five = new_oxy_float(5.0);
  oxy_object_t *six = new_oxy_float(6.0);

  oxy_object_t *v1 = new_oxy_vector3(one, two, three);
  oxy_object_t *v2 = new_oxy_vector3(four, five, six);
  oxy_object_t *result = oxy_add(v1, v2);

  munit_assert_not_null(result);
  munit_assert_int(result->kind, ==, VECTOR3);

  munit_assert_float(result->data.v_vector3.x->data.v_float, ==, 5.0);
  munit_assert_float(result->data.v_vector3.y->data.v_float, ==, 7.0);
  munit_assert_float(result->data.v_vector3.z->data.v_float, ==, 9.0);

  /* Note: In your original code, you free components manually. 
     Ensure your oxy_object implementation doesn't double-free these 
     if new_oxy_vector3 takes ownership. Assuming manual free is correct: */

  free(v1->data.v_vector3.x);
  free(v1->data.v_vector3.y);
  free(v1->data.v_vector3.z);
  free(v1);

  free(v2->data.v_vector3.x);
  free(v2->data.v_vector3.y);
  free(v2->data.v_vector3.z);
  free(v2);

  free(result->data.v_vector3.x);
  free(result->data.v_vector3.y);
  free(result->data.v_vector3.z);
  free(result);

  return MUNIT_OK;
}

static MunitResult
test_array_add(const MunitParameter params[], void* user_data) {
  (void)params; (void)user_data;

  oxy_object_t *one = new_oxy_integer(1);
  oxy_object_t *ones = new_oxy_array(2);
  munit_assert_true(oxy_array_set(ones, 0, one));
  munit_assert_true(oxy_array_set(ones, 1, one));

  oxy_object_t *hi = new_oxy_string("hi");
  oxy_object_t *hellos = new_oxy_array(3);
  munit_assert_true(oxy_array_set(hellos, 0, hi));
  munit_assert_true(oxy_array_set(hellos, 1, hi));
  munit_assert_true(oxy_array_set(hellos, 2, hi));

  oxy_object_t *result = oxy_add(ones, hellos);

  munit_assert_not_null(result);
  munit_assert_int(result->kind, ==, ARRAY);

  oxy_object_t *first = oxy_array_get(result, 0);
  munit_assert_not_null(first);
  munit_assert_int(first->data.v_int, ==, 1);

  oxy_object_t *third = oxy_array_get(result, 2);
  munit_assert_not_null(third);
  munit_assert_string_equal(third->data.v_string, "hi");

  free(one);
  free(ones->data.v_array.elements);
  free(ones);

  free(hi->data.v_string);
  free(hi);
  free(hellos->data.v_array.elements);
  free(hellos);
  free(result->data.v_array.elements);
  free(result);

  return MUNIT_OK;
}

/* ========================================================================= */
/* SUITE CONFIGURATION                                                       */
/* ========================================================================= */

static MunitTest test_suite_tests[] = {
  { (char*) "/integer", test_integer_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/float", test_float_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/string", test_string_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/string-repeated", test_string_add_self, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/vector3", test_vector3_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/array", test_array_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  
  /* NULL terminator is required by munit */
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
  (char*) "object-add",       /* Suite name */
  test_suite_tests,           /* Array of tests */
  NULL,                       /* Child suites */
  1,                          /* Iterations */
  MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
  /* Passing NULL for user_data as it is not used in the tests */
  return munit_suite_main(&test_suite, NULL, argc, argv);
}