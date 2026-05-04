#include "unity/unity.h"
#include "datast/exhash.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
  int id;
  char name[32];
  float value;
} record_t;

static void remove_files(const char *base) {
  char hf[300], hfc[300], hfd[300];
  snprintf(hf,  sizeof(hf),  "%s.hf",  base);
  snprintf(hfc, sizeof(hfc), "%s.hfc", base);
  snprintf(hfd, sizeof(hfd), "%s.hfd", base);
  remove(hf);
  remove(hfc);
  remove(hfd);
}

static record_t make_record(int id, const char *name, float value) {
  record_t r;
  r.id = id;
  r.value = value;
  memset(r.name, 0, sizeof(r.name));
  strncpy(r.name, name, sizeof(r.name) - 1);
  return r;
}

void setUp(void) {}
void tearDown(void) {}

void test_init_retorna_ponteiro_nao_nulo(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_init");
  TEST_ASSERT_NOT_NULL(exh);
  exh_destroy(exh);
  remove_files("./tmp_init");
}

void test_init_comeca_com_zero_entradas(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_empty");
  TEST_ASSERT_EQUAL_INT(0, exh_entries_amount(exh));
  exh_destroy(exh);
  remove_files("./tmp_empty");
}

void test_get_retorna_dado_inserido(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_get");

  record_t orig = make_record(42, "Alice", 3.14f);
  exh_insert(exh, "alice", &orig);

  record_t *got = exh_get(exh, "alice");
  TEST_ASSERT_NOT_NULL(got);
  TEST_ASSERT_EQUAL_INT(42, got->id);
  TEST_ASSERT_EQUAL_STRING("Alice", got->name);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.14f, got->value);

  free(got);
  exh_destroy(exh);
  remove_files("./tmp_get");
}

void test_get_chave_inexistente_retorna_null(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_miss");

  record_t r = make_record(1, "Bob", 0.0f);
  exh_insert(exh, "bob", &r);

  record_t *got = exh_get(exh, "nao_existe");
  TEST_ASSERT_NULL(got);

  exh_destroy(exh);
  remove_files("./tmp_miss");
}

void test_multiplas_insercoes_recuperadas_corretamente(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_multi");

  const int N = 10;
  char keys[10][16];
  record_t recs[10];

  for (int i = 0; i < N; i++) {
    snprintf(keys[i], sizeof(keys[i]), "chave_%d", i);
    recs[i] = make_record(i * 100, keys[i], (float)i);
    exh_insert(exh, keys[i], &recs[i]);
  }

  for (int i = 0; i < N; i++) {
    record_t *got = exh_get(exh, keys[i]);
    TEST_ASSERT_NOT_NULL_MESSAGE(got, keys[i]);
    TEST_ASSERT_EQUAL_INT(i * 100, got->id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)i, got->value);
    free(got);
  }

  exh_destroy(exh);
  remove_files("./tmp_multi");
}

void test_entries_amount_incrementa_apos_insercao(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_count");

  record_t r = make_record(1, "x", 0.0f);

  exh_insert(exh, "a", &r);
  TEST_ASSERT_EQUAL_INT(1, exh_entries_amount(exh));

  exh_insert(exh, "b", &r);
  TEST_ASSERT_EQUAL_INT(2, exh_entries_amount(exh));

  exh_insert(exh, "c", &r);
  TEST_ASSERT_EQUAL_INT(3, exh_entries_amount(exh));

  exh_destroy(exh);
  remove_files("./tmp_count");
}

void test_entries_amount_decrementa_apos_remocao(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_decr");

  record_t r = make_record(1, "x", 0.0f);
  exh_insert(exh, "a", &r);
  exh_insert(exh, "b", &r);
  exh_insert(exh, "c", &r);

  void *rem = exh_remove(exh, "b");
  free(rem);

  TEST_ASSERT_EQUAL_INT(2, exh_entries_amount(exh));

  exh_destroy(exh);
  remove_files("./tmp_decr");
}

void test_remove_retorna_dado_correto(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_rem");

  record_t orig = make_record(7, "Removal", 9.9f);
  exh_insert(exh, "rem_key", &orig);

  record_t *removed = exh_remove(exh, "rem_key");
  TEST_ASSERT_NOT_NULL(removed);
  TEST_ASSERT_EQUAL_INT(7, removed->id);
  TEST_ASSERT_EQUAL_STRING("Removal", removed->name);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 9.9f, removed->value);
  free(removed);

  exh_destroy(exh);
  remove_files("./tmp_rem");
}

void test_get_apos_remocao_retorna_null(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_after_rem");

  record_t r = make_record(1, "Gone", 0.0f);
  exh_insert(exh, "gone", &r);

  void *rem = exh_remove(exh, "gone");
  free(rem);

  record_t *got = exh_get(exh, "gone");
  TEST_ASSERT_NULL(got);

  exh_destroy(exh);
  remove_files("./tmp_after_rem");
}

void test_remove_chave_inexistente_retorna_null(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_rem_miss");

  void *result = exh_remove(exh, "nao_existe");
  TEST_ASSERT_NULL(result);

  exh_destroy(exh);
  remove_files("./tmp_rem_miss");
}

void test_remove_parcial_preserva_demais_entradas(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_partial");

  record_t a = make_record(1, "Alpha", 1.0f);
  record_t b = make_record(2, "Beta",  2.0f);
  record_t c = make_record(3, "Gamma", 3.0f);

  exh_insert(exh, "alpha", &a);
  exh_insert(exh, "beta",  &b);
  exh_insert(exh, "gamma", &c);

  void *rem = exh_remove(exh, "beta");
  free(rem);

  record_t *ga = exh_get(exh, "alpha");
  record_t *gc = exh_get(exh, "gamma");
  record_t *gb = exh_get(exh, "beta");

  TEST_ASSERT_NOT_NULL(ga);
  TEST_ASSERT_EQUAL_INT(1, ga->id);

  TEST_ASSERT_NOT_NULL(gc);
  TEST_ASSERT_EQUAL_INT(3, gc->id);

  TEST_ASSERT_NULL(gb);

  free(ga);
  free(gc);
  exh_destroy(exh);
  remove_files("./tmp_partial");
}

void test_get_all_retorna_todas_as_entradas(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_getall");

  const int N = 5;
  record_t recs[5];
  char keys[5][16];

  for (int i = 0; i < N; i++) {
    snprintf(keys[i], sizeof(keys[i]), "k%d", i);
    recs[i] = make_record(i, keys[i], (float)i);
    exh_insert(exh, keys[i], &recs[i]);
  }

  record_t *all = exh_get_all(exh);
  TEST_ASSERT_NOT_NULL(all);

  int found[5] = {0};
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (all[j].id == recs[i].id) {
        found[i]++;
      }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found[i], recs[i].name);
  }

  free(all);
  exh_destroy(exh);
  remove_files("./tmp_getall");
}

void test_get_all_apos_remocao_nao_inclui_removido(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_getall_rem");

  record_t a = make_record(10, "A", 1.0f);
  record_t b = make_record(20, "B", 2.0f);
  record_t c = make_record(30, "C", 3.0f);

  exh_insert(exh, "ka", &a);
  exh_insert(exh, "kb", &b);
  exh_insert(exh, "kc", &c);

  void *rem = exh_remove(exh, "kb");
  free(rem);

  TEST_ASSERT_EQUAL_INT(2, exh_entries_amount(exh));

  record_t *all = exh_get_all(exh);
  TEST_ASSERT_NOT_NULL(all);

  int found_a = 0, found_b = 0, found_c = 0;
  for (int i = 0; i < 2; i++) {
    if (all[i].id == 10) found_a++;
    if (all[i].id == 20) found_b++;
    if (all[i].id == 30) found_c++;
  }

  TEST_ASSERT_EQUAL_INT(1, found_a);
  TEST_ASSERT_EQUAL_INT(0, found_b);
  TEST_ASSERT_EQUAL_INT(1, found_c);

  free(all);
  exh_destroy(exh);
  remove_files("./tmp_getall_rem");
}

void test_load_recupera_dados_apos_destroy(void) {
  const char *path = "./tmp_persist";

  exhash_t *exh = exh_init(4, sizeof(record_t), path);
  record_t r1 = make_record(1, "Persisted1", 1.1f);
  record_t r2 = make_record(2, "Persisted2", 2.2f);
  exh_insert(exh, "p1", &r1);
  exh_insert(exh, "p2", &r2);
  exh_destroy(exh);

  exhash_t *loaded = exh_load(path);
  TEST_ASSERT_NOT_NULL(loaded);

  record_t *got1 = exh_get(loaded, "p1");
  record_t *got2 = exh_get(loaded, "p2");

  TEST_ASSERT_NOT_NULL(got1);
  TEST_ASSERT_EQUAL_INT(1, got1->id);
  TEST_ASSERT_EQUAL_STRING("Persisted1", got1->name);

  TEST_ASSERT_NOT_NULL(got2);
  TEST_ASSERT_EQUAL_INT(2, got2->id);
  TEST_ASSERT_EQUAL_STRING("Persisted2", got2->name);

  free(got1);
  free(got2);
  exh_destroy(loaded);
  remove_files(path);
}

void test_load_preserva_entries_amount(void) {
  const char *path = "./tmp_persist_count";

  exhash_t *exh = exh_init(4, sizeof(record_t), path);
  record_t r = make_record(0, "x", 0.0f);
  exh_insert(exh, "x1", &r);
  exh_insert(exh, "x2", &r);
  exh_insert(exh, "x3", &r);
  exh_destroy(exh);

  exhash_t *loaded = exh_load(path);
  TEST_ASSERT_EQUAL_INT(3, exh_entries_amount(loaded));

  exh_destroy(loaded);
  remove_files(path);
}

void test_load_chave_inexistente_apos_reload(void) {
  const char *path = "./tmp_persist_miss";

  exhash_t *exh = exh_init(4, sizeof(record_t), path);
  record_t r = make_record(1, "OnlyOne", 0.0f);
  exh_insert(exh, "only", &r);
  exh_destroy(exh);

  exhash_t *loaded = exh_load(path);
  record_t *got = exh_get(loaded, "nao_esta");
  TEST_ASSERT_NULL(got);

  exh_destroy(loaded);
  remove_files(path);
}

void test_estresse_100_insercoes_e_buscas(void) {
  exhash_t *exh = exh_init(3, sizeof(record_t), "./tmp_stress");

  const int N = 100;
  char keys[100][16];
  record_t recs[100];

  for (int i = 0; i < N; i++) {
    snprintf(keys[i], sizeof(keys[i]), "stress_%d", i);
    recs[i] = make_record(i, keys[i], (float)i * 0.5f);
    exh_insert(exh, keys[i], &recs[i]);
  }

  TEST_ASSERT_EQUAL_INT(N, exh_entries_amount(exh));

  for (int i = 0; i < N; i++) {
    record_t *got = exh_get(exh, keys[i]);
    TEST_ASSERT_NOT_NULL_MESSAGE(got, keys[i]);
    TEST_ASSERT_EQUAL_INT(i, got->id);
    free(got);
  }

  exh_destroy(exh);
  remove_files("./tmp_stress");
}

void test_estresse_insercao_remocao_alternada(void) {
  exhash_t *exh = exh_init(3, sizeof(record_t), "./tmp_stress_alt");

  record_t r = make_record(99, "Alternado", 0.0f);
  char key[16];

  for (int i = 0; i < 50; i++) {
    snprintf(key, sizeof(key), "alt_%d", i);
    exh_insert(exh, key, &r);
  }
  TEST_ASSERT_EQUAL_INT(50, exh_entries_amount(exh));

  for (int i = 0; i < 25; i++) {
    snprintf(key, sizeof(key), "alt_%d", i);
    void *rem = exh_remove(exh, key);
    TEST_ASSERT_NOT_NULL_MESSAGE(rem, key);
    free(rem);
  }
  TEST_ASSERT_EQUAL_INT(25, exh_entries_amount(exh));

  for (int i = 25; i < 50; i++) {
    snprintf(key, sizeof(key), "alt_%d", i);
    record_t *got = exh_get(exh, key);
    TEST_ASSERT_NOT_NULL_MESSAGE(got, key);
    free(got);
  }

  exh_destroy(exh);
  remove_files("./tmp_stress_alt");
}

void test_estresse_bucket_size_1_forca_muitos_splits(void) {
  exhash_t *exh = exh_init(1, sizeof(record_t), "./tmp_stress_split");

  const int N = 30;
  char keys[30][16];
  record_t recs[30];

  for (int i = 0; i < N; i++) {
    snprintf(keys[i], sizeof(keys[i]), "sp_%d", i);
    recs[i] = make_record(i, keys[i], (float)i);
    exh_insert(exh, keys[i], &recs[i]);
  }

  TEST_ASSERT_EQUAL_INT(N, exh_entries_amount(exh));

  for (int i = 0; i < N; i++) {
    record_t *got = exh_get(exh, keys[i]);
    TEST_ASSERT_NOT_NULL_MESSAGE(got, keys[i]);
    TEST_ASSERT_EQUAL_INT(i, got->id);
    free(got);
  }

  exh_destroy(exh);
  remove_files("./tmp_stress_split");
}

typedef struct {
  char buf[128];
} large_record_t;

void test_data_size_grande_insercao_e_recuperacao(void) {
  exhash_t *exh = exh_init(4, sizeof(large_record_t), "./tmp_large");

  large_record_t orig;
  memset(orig.buf, 'Z', sizeof(orig.buf));
  orig.buf[127] = '\0';

  exh_insert(exh, "big_key", &orig);

  large_record_t *got = exh_get(exh, "big_key");
  TEST_ASSERT_NOT_NULL(got);
  TEST_ASSERT_EQUAL_MEMORY(orig.buf, got->buf, sizeof(orig.buf));

  free(got);
  exh_destroy(exh);
  remove_files("./tmp_large");
}

typedef struct {
  uint8_t byte;
} tiny_record_t;

void test_data_size_pequeno_insercao_e_recuperacao(void) {
  exhash_t *exh = exh_init(4, sizeof(tiny_record_t), "./tmp_tiny");

  tiny_record_t orig = { .byte = 0xAB };
  exh_insert(exh, "tiny_key", &orig);

  tiny_record_t *got = exh_get(exh, "tiny_key");
  TEST_ASSERT_NOT_NULL(got);
  TEST_ASSERT_EQUAL_UINT8(0xAB, got->byte);

  free(got);
  exh_destroy(exh);
  remove_files("./tmp_tiny");
}

void test_insert_duplicado_nao_adiciona_segunda_entrada(void) {
  exhash_t *exh = exh_init(4, sizeof(record_t), "./tmp_dup");

  record_t r1 = make_record(1, "First",  1.0f);
  record_t r2 = make_record(2, "Second", 2.0f);

  bool ins_res1 = exh_insert(exh, "dup_key", &r1);
  bool ins_res2 = exh_insert(exh, "dup_key", &r2);

  TEST_ASSERT_TRUE(ins_res1);
  TEST_ASSERT_FALSE(ins_res2);
  TEST_ASSERT_EQUAL_INT(1, exh_entries_amount(exh));

  record_t *got = exh_get(exh, "dup_key");
  TEST_ASSERT_NOT_NULL(got);
  TEST_ASSERT_EQUAL_MEMORY(got, &r1, sizeof(record_t));
  free(got);

  exh_destroy(exh);
  remove_files("./tmp_dup");
}

int main(void) {
  UNITY_BEGIN();

  // criação
  RUN_TEST(test_init_retorna_ponteiro_nao_nulo);
  RUN_TEST(test_init_comeca_com_zero_entradas);

  // exh_get e exh_insert
  RUN_TEST(test_get_retorna_dado_inserido);
  RUN_TEST(test_get_chave_inexistente_retorna_null);
  RUN_TEST(test_multiplas_insercoes_recuperadas_corretamente);

  // exh_entries_amount
  RUN_TEST(test_entries_amount_incrementa_apos_insercao);
  RUN_TEST(test_entries_amount_decrementa_apos_remocao);

  // exh_remove
  RUN_TEST(test_remove_retorna_dado_correto);
  RUN_TEST(test_get_apos_remocao_retorna_null);
  RUN_TEST(test_remove_chave_inexistente_retorna_null);
  RUN_TEST(test_remove_parcial_preserva_demais_entradas);

  // exh_get_all
  RUN_TEST(test_get_all_retorna_todas_as_entradas);
  RUN_TEST(test_get_all_apos_remocao_nao_inclui_removido);

  // exh_load
  RUN_TEST(test_load_recupera_dados_apos_destroy);
  RUN_TEST(test_load_preserva_entries_amount);
  RUN_TEST(test_load_chave_inexistente_apos_reload);

  // inserção e remoção múltipla
  RUN_TEST(test_estresse_100_insercoes_e_buscas);
  RUN_TEST(test_estresse_insercao_remocao_alternada);
  RUN_TEST(test_estresse_bucket_size_1_forca_muitos_splits);

  // data sizes diferentes
  RUN_TEST(test_data_size_grande_insercao_e_recuperacao);
  RUN_TEST(test_data_size_pequeno_insercao_e_recuperacao);

  // key duplicada
  RUN_TEST(test_insert_duplicado_nao_adiciona_segunda_entrada);

  return UNITY_END();
}
