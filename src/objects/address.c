#include "objects/address.h"

#include <stdlib.h>
#include <string.h>

struct address_t {
  char cpf[16];
  char cep[16];
  char face;
  uint16_t number;
  char complement[32];
};

address_t *ads_init(const char cpf[16], const char cep[16], char face, uint16_t number, const char complement[32]) {
  address_t *a = calloc(1, sizeof(address_t));

  strncpy(a->cpf, cpf, 15);
  strncpy(a->cep, cep, 15);
  a->face = face;
  a->number = number;
  strncpy(a->complement, complement, 31);

  return a;
}

size_t ads_sizeof() {
  return sizeof(address_t);
}

const char *ads_get_cpf(const address_t *a) {
  return a->cpf;
}

const char *ads_get_cep(const address_t *a) {
  return a->cep;
}

char ads_get_face(const address_t *a) {
  return a->face;
}

uint16_t ads_get_number(const address_t *a) {
  return a->number;
}

const char *ads_get_complement(const address_t *a) {
  return a->complement;
}

void ads_set_cpf(address_t *a, const char *cpf) {
  strncpy(a->cpf, cpf, sizeof(a->cpf) - 1);
  a->cpf[sizeof(a->cpf) - 1] = '\0';
}

void ads_set_cep(address_t *a, const char *cep) {
  strncpy(a->cep, cep, sizeof(a->cep) - 1);
  a->cep[sizeof(a->cep) - 1] = '\0';
}

void ads_set_face(address_t *a, char face) {
  a->face = face;
}

void ads_set_number(address_t *a, uint16_t number) {
  a->number = number;
}

void ads_set_complement(address_t *a, const char *complement) {
  strncpy(a->complement, complement, sizeof(a->complement) - 1);
  a->complement[sizeof(a->complement) - 1] = '\0';
}

void ads_destroy(address_t *ads) {
  free(ads);
}
