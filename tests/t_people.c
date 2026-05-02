/*
* Testes Unitários — Módulo people (Pessoa)
*/

#include "unity/unity.h"
#include "objects/people.h"

/* ------------------------------------------------------------------ */
/* setUp / tearDown                                                   */
/* ------------------------------------------------------------------ */
void setUp(void) {}
void tearDown(void) {}

/* ================================================================== */
/* Testes                                                             */
/* ================================================================== */

void test_people_sizeof_deve_ser_maior_que_zero(void) {
  TEST_ASSERT_GREATER_THAN(0, people_sizeof());
}

void test_people_init_retorna_ponteiro_nao_nulo(void) {
  people_t *p = people_init("11122233344", "João", "Silva", 'M', "01/01/1990");
  TEST_ASSERT_NOT_NULL(p);
  people_destroy(p);
}

void test_people_getters_retornam_valores_iniciais_corretos(void) {
  people_t *p = people_init("123.456.789-00", "Maria", "Souza", 'F', "15/05/1985");
  
  TEST_ASSERT_EQUAL_STRING("123.456.789-00", people_get_cpf(p));
  TEST_ASSERT_EQUAL_INT('F', people_get_gender(p));
  TEST_ASSERT_EQUAL_STRING("15/05/1985", people_get_date_of_birth(p));
  
  people_destroy(p);
}

void test_people_setters_atualizam_valores_corretamente(void) {
  people_t *p = people_init("000", "Nome", "Sobrenome", 'N', "00/00/0000");
  
  people_set_name(p, "Carlos");
  people_set_surname(p, "Andrade");
  people_set_gender(p, 'M');
  
  TEST_ASSERT_EQUAL_INT('M', people_get_gender(p));
  /* O CPF não deve possuir setter de acordo com a documentação do .h, logo, atestamos que a mudança nos demais não o corrompe */
  TEST_ASSERT_EQUAL_STRING("000", people_get_cpf(p));
  
  people_destroy(p);
}

/* ================================================================== */
/* main                                                               */
/* ================================================================== */
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_people_sizeof_deve_ser_maior_que_zero);
  RUN_TEST(test_people_init_retorna_ponteiro_nao_nulo);
  RUN_TEST(test_people_getters_retornam_valores_iniciais_corretos);
  RUN_TEST(test_people_setters_atualizam_valores_corretamente);
  return UNITY_END();
}
