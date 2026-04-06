#include "exhash.h"
#include <stdio.h>
#include <stdlib.h>

struct test_t {
  char c;
  int n;
};

int main() {
  exhash_t *exh = exh_init(10, sizeof(struct test_t), "./testfile");
  
  struct test_t t1 = { 'a', 1 };
  struct test_t t2 = { 'b', 2 };
  struct test_t t3 = { 'c', 3 };
  struct test_t t4 = { 'd', 4 };
  struct test_t t5 = { 'e', 5};

  exh_insert(exh, 1, &t1);
  exh_insert(exh, 2, &t2);
  exh_insert(exh, 3, &t3);
  exh_insert(exh, 4, &t4);
  exh_insert(exh, 5, &t5);

  exhash_t *exhloaded = exh_load("./testfile");

  exh_remove(exh, 4);

  struct test_t *r1 = exh_get(exhloaded, 1);
  struct test_t *r2 = exh_get(exhloaded, 2);
  struct test_t *r3 = exh_get(exhloaded, 3);
  struct test_t *r5 = exh_get(exhloaded, 5);

  if (r1 == NULL) printf("r1 is null\n");
  if (r2 == NULL) printf("r2 is null\n");
  if (r3 == NULL) printf("r3 is null\n");
  if (r5 == NULL) printf("r5 is null\n");

  printf("- R1\n\t- c -> %c\n\tn -> %d\n", r1->c, r1->n);
  printf("- R2\n\t- c -> %c\n\tn -> %d\n", r2->c, r2->n);
  printf("- R3\n\t- c -> %c\n\tn -> %d\n", r3->c, r3->n);
  printf("- R5\n\t- c -> %c\n\tn -> %d\n", r5->c, r5->n);

  /*
  struct test_t t1 = { 10, "vai tomar no cu" };
  struct test_t t2 = { 15, "seu filho duma puta" };
  struct test_t t3 = { 20, "arrombado do caralho" };
  struct test_t t4 = { 25, "vai se foder" };
  struct test_t t5 = { 30, "corno" };

  exh_insert(exh, 1, &t1);
  exh_insert(exh, 2, &t2);
  exh_insert(exh, 3, &t3);
  exh_insert(exh, 4, &t4);
  exh_insert(exh, 5, &t5);

  struct test_t *r1 = exh_get(exh, 1);
  struct test_t *r2 = exh_get(exh, 2);
    struct test_t *r3 = exh_get(exh, 3);
  struct test_t *r4 = exh_get(exh, 4);
  struct test_t *r5 = exh_get(exh, 5);

  if (r1 == NULL) printf("r1 is null\n");
  if (r2 == NULL) printf("r2 is null\n");
  if (r3 == NULL) printf("r3 is null\n");
  if (r4 == NULL) printf("r4 is null\n");
  if (r5 == NULL) printf("r5 is null\n");

  printf("- R1\n\t- Number = %d\n\t- String = %s\n\n", r1->number, r1->string);
  printf("- R2\n\t- Number = %d\n\t- String = %s\n\n", r2->number, r2->string);
  printf("- R3\n\t- Number = %d\n\t- String = %s\n\n", r3->number, r3->string);
  printf("- R4\n\t- Number = %d\n\t- String = %s\n\n", r4->number, r4->string);
  printf("- R5\n\t- Number = %d\n\t- String = %s\n\n", r5->number, r5->string);
  */

  free(r1);
  free(r2);
  free(r3);
  free(r5);

  exh_destroy(exh);
  exh_destroy(exhloaded);
}
