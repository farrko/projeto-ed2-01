#include "unity/unity.h"
#include "exhash.h"

#include <stdlib.h>

typedef struct {
  int number;
  char string[20];
} struct_t;

exhash_t *exh;

void setUp() {}

void tearDown() {}

void test_exhinit() {
  exh = exh_init(5, sizeof(struct_t), "./test_hash");
  TEST_ASSERT_NOT_NULL(exh);
}

void test_exhinsert_exhget() {
  struct_t i1 = { 1, "abc" };
  struct_t i2 = { 2, "def" };
  struct_t i3 = { 3, "ghi" };

  exh_insert(exh, 1, &i1);
  exh_insert(exh, 2, &i2);
  exh_insert(exh, 3, &i3);

  struct_t *g1 = exh_get(exh, 1);
  struct_t *g2 = exh_get(exh, 2);
  struct_t *g3 = exh_get(exh, 3);

  TEST_ASSERT_EQUAL_MEMORY(&i1, g1, sizeof(struct_t));
  TEST_ASSERT_EQUAL_MEMORY(&i2, g2, sizeof(struct_t));
  TEST_ASSERT_EQUAL_MEMORY(&i3, g3, sizeof(struct_t));

  free(g1);
  free(g2);
  free(g3);
}

void test_exhload() {
  exhash_t *loaded = exh_load(  "./test_hash");

  struct_t *l1 = exh_get(loaded, 1);
  struct_t *l2 = exh_get(loaded, 2);
  struct_t *l3 = exh_get(loaded, 3);

  struct_t *g1 = exh_get(exh, 1);
  struct_t *g2 = exh_get(exh, 2);
  struct_t *g3 = exh_get(exh, 3);

  TEST_ASSERT_EQUAL_MEMORY(l1, g1, sizeof(struct_t));
  TEST_ASSERT_EQUAL_MEMORY(l2, g2, sizeof(struct_t));
  TEST_ASSERT_EQUAL_MEMORY(l3, g3, sizeof(struct_t));


  free(l1);
  free(l2);
  free(l3);

  free(g1);
  free(g2);
  free(g3);

  exh_destroy(loaded);
}

void test_exhremove() {
  exh_remove(exh, 3);
  struct_t *g3 = exh_get(exh, 3);
  TEST_ASSERT_NULL(g3);
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_exhinit);
  RUN_TEST(test_exhinsert_exhget);
  RUN_TEST(test_exhload);
  RUN_TEST(test_exhremove);

  exh_destroy(exh);

  return UNITY_END();
}
