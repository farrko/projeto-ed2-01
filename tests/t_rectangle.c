#include "unity/unity.h"
#include "shapes/rectangle.h"

#define DELTA 0.001

void setUp(void) {}
void tearDown(void) {}

void test_rect_init_retorna_ponteiro_nao_nulo(void) {
  rectangle_t *r = rect_init(1, 10.0, 10.0, 50.0, 30.0, "#000", "#FFF", "2px");
  TEST_ASSERT_NOT_NULL(r);
  rect_destroy(r);
}

void test_rect_getters_retornam_valores_iniciais(void) {
  rectangle_t *r = rect_init(10, 5.5, 6.5, 100.0, 200.0, "blue", "red", "5px");

  TEST_ASSERT_EQUAL_size_t(10, rect_get_id(r));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 5.5, rect_get_x(r));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 6.5, rect_get_y(r));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 100.0, rect_get_width(r));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 200.0, rect_get_height(r));

  TEST_ASSERT_EQUAL_STRING("blue", rect_get_color(r));
  TEST_ASSERT_EQUAL_STRING("red", rect_get_border_color(r));
  TEST_ASSERT_EQUAL_STRING("5px", rect_get_border_width(r));

  rect_destroy(r);
}

void test_rect_setters_atualizam_valores(void) {
  rectangle_t *r = rect_init(1, 0.0, 0.0, 10.0, 10.0, "a", "b", "c");

  rect_set_x(r, 11.1);
  rect_set_y(r, 22.2);
  rect_set_width(r, 33.3);
  rect_set_height(r, 44.4);
  rect_set_color(r, "black");
  rect_set_border_color(r, "white");
  rect_set_border_width(r, "1px");

  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 11.1, rect_get_x(r));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 22.2, rect_get_y(r));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 33.3, rect_get_width(r));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 44.4, rect_get_height(r));

  TEST_ASSERT_EQUAL_STRING("black", rect_get_color(r));
  TEST_ASSERT_EQUAL_STRING("white", rect_get_border_color(r));
  TEST_ASSERT_EQUAL_STRING("1px", rect_get_border_width(r));

  rect_destroy(r);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_rect_init_retorna_ponteiro_nao_nulo);
  RUN_TEST(test_rect_getters_retornam_valores_iniciais);
  RUN_TEST(test_rect_setters_atualizam_valores);
  return UNITY_END();
}
