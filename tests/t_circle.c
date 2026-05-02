/*
 * Testes Unitários — Módulo circle (Círculo)
 */

#include "unity/unity.h"
#include "shapes/circle.h"
#include "shapes/point.h" /* Conforme include no header circle.h */

#define DELTA 0.001

void setUp(void) {}
void tearDown(void) {}

void test_circle_init_retorna_ponteiro_nao_nulo(void) {
  circle_t *c = circle_init(1, 10.5, 20.5, 5.0, "#FFFFFF", "#000000");
  TEST_ASSERT_NOT_NULL(c);
  circle_destroy(c);
}

void test_circle_getters_retornam_valores_iniciais(void) {
  circle_t *c = circle_init(2, 15.0, 30.0, 7.5, "red", "blue");
  
  TEST_ASSERT_EQUAL_size_t(2, circle_get_id(c));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 15.0, circle_get_x(c));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 30.0, circle_get_y(c));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 7.5, circle_get_radius(c));
  TEST_ASSERT_EQUAL_STRING("red", circle_get_color(c));
  TEST_ASSERT_EQUAL_STRING("blue", circle_get_border_color(c));
  
  /* Verifica se o point_t da origem foi instanciado corretamente */
  point_t *origin = circle_get_origin(c);
  TEST_ASSERT_NOT_NULL(origin);
  
  circle_destroy(c);
}

void test_circle_setters_atualizam_valores(void) {
  circle_t *c = circle_init(3, 0.0, 0.0, 1.0, "white", "black");
  
  circle_set_x(c, 50.5);
  circle_set_y(c, 60.5);
  circle_set_radius(c, 10.0);
  circle_set_color(c, "green");
  circle_set_border_color(c, "yellow");
  
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 50.5, circle_get_x(c));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 60.5, circle_get_y(c));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10.0, circle_get_radius(c));
  TEST_ASSERT_EQUAL_STRING("green", circle_get_color(c));
  TEST_ASSERT_EQUAL_STRING("yellow", circle_get_border_color(c));
  
  circle_destroy(c);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_circle_init_retorna_ponteiro_nao_nulo);
  RUN_TEST(test_circle_getters_retornam_valores_iniciais); RUN_TEST(test_circle_setters_atualizam_valores);
  return UNITY_END();
}
