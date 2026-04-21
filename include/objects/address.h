#ifndef ADDRESS_H
#define ADDRESS_H

#include <stddef.h>
#include <stdint.h>

typedef struct address_t address_t;

address_t *ads_init(const char cpf[16], const char cep[16], char face, uint16_t number, const char complement[32]);

size_t ads_sizeof();

const char *ads_get_cpf(const address_t *a);
const char *ads_get_cep(const address_t *a);
char ads_get_face(const address_t *a);
uint16_t ads_get_number(const address_t *a);
const char *ads_get_complement(const address_t *a);

void ads_set_cpf(address_t *a, const char *cpf);
void ads_set_cep(address_t *a, const char *cep);
void ads_set_face(address_t *a, char face);
void ads_set_number(address_t *a, uint16_t number);
void ads_set_complement(address_t *a, const char *complement);

void ads_destroy(address_t *ads);

#endif
