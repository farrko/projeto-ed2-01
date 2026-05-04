#include "unity/unity.h"
#include "shapes/text.h"

#define DELTA 0.001

void setUp(void) {}
void tearDown(void) {}

void test_text_init_e_getters(void) {
  text_t *t = text_init(42, 10.0, 20.0, "start", "black", "none", "Arial", "bold", "12px", "Hello World");

  TEST_ASSERT_NOT_NULL(t);
  TEST_ASSERT_EQUAL_size_t(42, text_get_id(t));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10.0, text_get_x(t));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 20.0, text_get_y(t));

  TEST_ASSERT_EQUAL_STRING("start", text_get_anchor(t));
  TEST_ASSERT_EQUAL_STRING("black", text_get_color(t));
  TEST_ASSERT_EQUAL_STRING("none", text_get_border_color(t));
  TEST_ASSERT_EQUAL_STRING("Arial", text_get_ffam(t));
  TEST_ASSERT_EQUAL_STRING("bold", text_get_fweight(t));
  TEST_ASSERT_EQUAL_STRING("12px", text_get_fsize(t));
  TEST_ASSERT_EQUAL_STRING("Hello World", text_get_content(t));

  text_destroy(t);
}

void test_text_setters(void) {
  text_t *t = text_init(1, 0, 0, "", "", "", "", "", "", "");

  text_set_x(t, 55.5);
  text_set_y(t, 66.6);
  text_set_anchor(t, "middle");
  text_set_color(t, "red");
  text_set_border_color(t, "blue");
  text_set_ffam(t, "Times New Roman");
  text_set_fweight(t, "normal");
  text_set_fsize(t, "16px");
  text_set_content(t, "Novo Texto");

  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 55.5, text_get_x(t));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 66.6, text_get_y(t));
  TEST_ASSERT_EQUAL_STRING("middle", text_get_anchor(t));
  TEST_ASSERT_EQUAL_STRING("red", text_get_color(t));
  TEST_ASSERT_EQUAL_STRING("blue", text_get_border_color(t));
  TEST_ASSERT_EQUAL_STRING("Times New Roman", text_get_ffam(t));
  TEST_ASSERT_EQUAL_STRING("normal", text_get_fweight(t));
  TEST_ASSERT_EQUAL_STRING("16px", text_get_fsize(t));
  TEST_ASSERT_EQUAL_STRING("Novo Texto", text_get_content(t));

  text_destroy(t);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_text_init_e_getters);
  RUN_TEST(test_text_setters);
  return UNITY_END();
}
