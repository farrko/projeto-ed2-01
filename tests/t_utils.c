/*
 * Testes Unitários — Módulo utils (r_strcat / strnlen)
 *
 * Cobertura:
 *   r_strcat
 *     - concatenação básica
 *     - string vazia à esquerda
 *     - string vazia à direita
 *     - ambas vazias
 *     - resultado alocado em memória nova (ponteiros distintos)
 *     - strings longas
 *
 *   strnlen
 *     - string normal dentro do limite
 *     - string exatamente no limite
 *     - limite menor que o comprimento real → retorna maxlen
 *     - limite zero → retorna 0
 *     - string vazia → retorna 0
 *     - string com um único caractere
 *     - maxlen muito maior que a string
 */

#include "unity/unity.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* setUp / tearDown                                                     */
/* ------------------------------------------------------------------ */

void setUp(void) {}
void tearDown(void) {}

/* ================================================================== */
/* r_strcat                                                            */
/* ================================================================== */

void test_rstrcat_concatena_duas_strings_simples(void) {
  char *result = r_strcat("hello", " world");
  TEST_ASSERT_EQUAL_STRING("hello world", result);
  free(result);
}

void test_rstrcat_string_vazia_esquerda(void) {
  char *result = r_strcat("", "suffix");
  TEST_ASSERT_EQUAL_STRING("suffix", result);
  free(result);
}

void test_rstrcat_string_vazia_direita(void) {
  char *result = r_strcat("prefix", "");
  TEST_ASSERT_EQUAL_STRING("prefix", result);
  free(result);
}

void test_rstrcat_ambas_vazias(void) {
  char *result = r_strcat("", "");
  TEST_ASSERT_EQUAL_STRING("", result);
  free(result);
}

void test_rstrcat_retorna_ponteiro_diferente_dos_originais(void) {
  char s1[] = "abc";
  char s2[] = "def";
  char *result = r_strcat(s1, s2);

  /* A memória deve ser nova — não pode ser igual a nenhum dos originais */
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_TRUE(result != s1);
  TEST_ASSERT_TRUE(result != s2);

  free(result);
}

void test_rstrcat_comprimento_correto_do_resultado(void) {
  char *result = r_strcat("abcd", "efgh");

  TEST_ASSERT_EQUAL_INT(8, (int)strlen(result));
  TEST_ASSERT_EQUAL_STRING("abcdefgh", result);

  free(result);
}

void test_rstrcat_strings_longas(void) {
  /* 64 caracteres cada */
  const char *s1 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  const char *s2 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";

  char *result = r_strcat((char *)s1, (char *)s2);

  TEST_ASSERT_EQUAL_INT(128, (int)strlen(result));

  /* Primeiros 64 devem ser 'A', últimos 64 devem ser 'B' */
  for (int i = 0;  i < 64;  i++) TEST_ASSERT_EQUAL_CHAR('A', result[i]);
  for (int i = 64; i < 128; i++) TEST_ASSERT_EQUAL_CHAR('B', result[i]);

  free(result);
}

void test_rstrcat_preserva_conteudo_das_strings_originais(void) {
  char s1[] = "foo";
  char s2[] = "bar";

  char *result = r_strcat(s1, s2);

  /* As strings originais não devem ser modificadas */
  TEST_ASSERT_EQUAL_STRING("foo", s1);
  TEST_ASSERT_EQUAL_STRING("bar", s2);

  free(result);
}

void test_rstrcat_com_extensao_de_arquivo(void) {
  /* Caso de uso real: montar caminhos como "path/arquivo" + ".hf" */
  char *result = r_strcat("./saida/dados", ".hf");
  TEST_ASSERT_EQUAL_STRING("./saida/dados.hf", result);
  free(result);
}

/* ================================================================== */
/* strnlen                                                             */
/* ================================================================== */

void test_strnlen_string_normal_dentro_do_limite(void) {
  /* "hello" tem 5 caracteres; maxlen > 5 → deve retornar 5 */
  size_t len = strnlen("hello", 10);
  TEST_ASSERT_EQUAL_size_t(5, len);
}

void test_strnlen_limite_exatamente_igual_ao_comprimento(void) {
  /* "abc" tem 3 caracteres; maxlen = 3 → deve retornar 3 */
  size_t len = strnlen("abc", 3);
  TEST_ASSERT_EQUAL_size_t(3, len);
}

void test_strnlen_limite_menor_que_comprimento_real(void) {
  /* "abcdef" tem 6 caracteres; maxlen = 4 → deve retornar 4 (truncado) */
  size_t len = strnlen("abcdef", 4);
  TEST_ASSERT_EQUAL_size_t(4, len);
}

void test_strnlen_limite_zero_retorna_zero(void) {
  /* Qualquer string com maxlen = 0 → deve retornar 0 */
  size_t len = strnlen("qualquer", 0);
  TEST_ASSERT_EQUAL_size_t(0, len);
}

void test_strnlen_string_vazia_retorna_zero(void) {
  size_t len = strnlen("", 10);
  TEST_ASSERT_EQUAL_size_t(0, len);
}

void test_strnlen_string_vazia_maxlen_zero(void) {
  size_t len = strnlen("", 0);
  TEST_ASSERT_EQUAL_size_t(0, len);
}

void test_strnlen_um_unico_caractere(void) {
  size_t len = strnlen("x", 5);
  TEST_ASSERT_EQUAL_size_t(1, len);
}

void test_strnlen_maxlen_muito_maior_que_string(void) {
  /* O comprimento real deve ser retornado, não maxlen */
  size_t len = strnlen("hi", 1000);
  TEST_ASSERT_EQUAL_size_t(2, len);
}

void test_strnlen_limite_1_string_de_tamanho_1(void) {
  /* Exactamente na fronteira: "a" com maxlen=1 → deve retornar 1 */
  size_t len = strnlen("a", 1);
  TEST_ASSERT_EQUAL_size_t(1, len);
}

void test_strnlen_limite_1_string_maior(void) {
  /* "abc" com maxlen=1 →  deve retornar 1 (truncado) */
  size_t len = strnlen("abc", 1);
  TEST_ASSERT_EQUAL_size_t(1, len);
}

void test_strnlen_string_com_espacos(void) {
  /* Espaços são caracteres normais */
  size_t len = strnlen("a b c", 10);
  TEST_ASSERT_EQUAL_size_t(5, len);
}

/* ================================================================== */
/* main                                                                 */
/* ================================================================== */

int main(void) {
  UNITY_BEGIN();

  /* r_strcat */
  RUN_TEST(test_rstrcat_concatena_duas_strings_simples);
  RUN_TEST(test_rstrcat_string_vazia_esquerda);
  RUN_TEST(test_rstrcat_string_vazia_direita);
  RUN_TEST(test_rstrcat_ambas_vazias);
  RUN_TEST(test_rstrcat_retorna_ponteiro_diferente_dos_originais);
  RUN_TEST(test_rstrcat_comprimento_correto_do_resultado);
  RUN_TEST(test_rstrcat_strings_longas);
  RUN_TEST(test_rstrcat_preserva_conteudo_das_strings_originais);
  RUN_TEST(test_rstrcat_com_extensao_de_arquivo);

  /* strnlen */
  RUN_TEST(test_strnlen_string_normal_dentro_do_limite);
  RUN_TEST(test_strnlen_limite_exatamente_igual_ao_comprimento);
  RUN_TEST(test_strnlen_limite_menor_que_comprimento_real);
  RUN_TEST(test_strnlen_limite_zero_retorna_zero);
  RUN_TEST(test_strnlen_string_vazia_retorna_zero);
  RUN_TEST(test_strnlen_string_vazia_maxlen_zero);
  RUN_TEST(test_strnlen_um_unico_caractere);
  RUN_TEST(test_strnlen_maxlen_muito_maior_que_string);
  RUN_TEST(test_strnlen_limite_1_string_de_tamanho_1);
  RUN_TEST(test_strnlen_limite_1_string_maior);
  RUN_TEST(test_strnlen_string_com_espacos);

  return UNITY_END();
}
