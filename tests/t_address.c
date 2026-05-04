#include "unity/unity.h"
#include "objects/address.h"

void setUp(void) {}
void tearDown(void) {}

void test_ads_sizeof_deve_ser_maior_que_zero(void) {
  TEST_ASSERT_GREATER_THAN(0, ads_sizeof());
}

void test_ads_init_retorna_ponteiro_nao_nulo(void) {
  address_t *ads = ads_init("11122233344", "86000000", 'N', 100, "Apto 1");
  TEST_ASSERT_NOT_NULL(ads);
  ads_destroy(ads);
}

void test_ads_getters_retornam_valores_iniciais_corretos(void) {
  address_t *ads = ads_init("123.456.789-00", "86000-000", 'S', 456, "Casa");

  TEST_ASSERT_EQUAL_STRING("123.456.789-00", ads_get_cpf(ads));
  TEST_ASSERT_EQUAL_STRING("86000-000", ads_get_cep(ads));
  TEST_ASSERT_EQUAL_INT('S', ads_get_face(ads));
  TEST_ASSERT_EQUAL_UINT16(456, ads_get_number(ads));
  TEST_ASSERT_EQUAL_STRING("Casa", ads_get_complement(ads));

  ads_destroy(ads);
}

void test_ads_setters_atualizam_valores_corretamente(void) {
  address_t *ads = ads_init("0000", "0000", 'N', 0, "");

  ads_set_cpf(ads, "111.111.111-11");
  ads_set_cep(ads, "11111-111");
  ads_set_face(ads, 'L');
  ads_set_number(ads, 999);

  TEST_ASSERT_EQUAL_STRING("111.111.111-11", ads_get_cpf(ads));
  TEST_ASSERT_EQUAL_STRING("11111-111", ads_get_cep(ads));
  TEST_ASSERT_EQUAL_INT('L', ads_get_face(ads));
  TEST_ASSERT_EQUAL_UINT16(999, ads_get_number(ads));

  ads_destroy(ads);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_ads_sizeof_deve_ser_maior_que_zero);
  RUN_TEST(test_ads_init_retorna_ponteiro_nao_nulo);
  RUN_TEST(test_ads_getters_retornam_valores_iniciais_corretos);
  RUN_TEST(test_ads_setters_atualizam_valores_corretamente);
  return UNITY_END();
}
