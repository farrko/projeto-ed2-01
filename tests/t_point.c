#include "unity/unity.h"
#include "shapes/point.h"

#define DELTA 0.001

void setUp(void) {}
void tearDown(void) {}

void test_point_init_e_getters_cartesianos(void) {
  point_t *p = point_init(10.5, -5.5);
  TEST_ASSERT_NOT_NULL(p);
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10.5, point_get_x(p));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, -5.5, point_get_y(p));
  point_destroy(p);
}

void test_point_setters_cartesianos(void) {
  point_t *p = point_init(0.0, 0.0);
  point_set_x(p, 100.0);
  point_set_y(p, 200.0);
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 100.0, point_get_x(p));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 200.0, point_get_y(p));
  point_destroy(p);
}

void test_polar_init_e_getters(void) {
  point_t *origin = point_init(0.0, 0.0);
  polar_coords_t *polar = polar_init(3.1415, 10.0, origin);

  TEST_ASSERT_NOT_NULL(polar);
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 3.1415, polar_get_angle(polar));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10.0, polar_get_distance(polar));

  point_t *rec_origin = polar_get_origin(polar);
  TEST_ASSERT_NOT_NULL(rec_origin);

  polar_destroy(polar);
  point_destroy(origin);
}

void test_conversao_cartesian_para_polar_e_vice_versa(void) {
  point_t *origin = point_init(0.0, 0.0);
  point_t *final = point_init(10.0, 10.0);

  polar_coords_t *polar = polar_from_cartesian(origin, final);
  TEST_ASSERT_NOT_NULL(polar);

  point_t *converted_back = cartesian_from_polar(polar);
  TEST_ASSERT_NOT_NULL(converted_back);

  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10.0, point_get_x(converted_back));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10.0, point_get_y(converted_back));

  point_destroy(origin);
  point_destroy(final);
  polar_destroy(polar);
  point_destroy(converted_back);
}

int main(void) {
  UNITY_BEGIN();

  // point_init e getters
  RUN_TEST(test_point_init_e_getters_cartesianos);

  // point setters
  RUN_TEST(test_point_setters_cartesianos);

  // polar_init e getters
  RUN_TEST(test_polar_init_e_getters);

  // troca entre cartesiano e polar
  RUN_TEST(test_conversao_cartesian_para_polar_e_vice_versa);

  return UNITY_END();
}
