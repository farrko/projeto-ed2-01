#include <stdlib.h>
#include <string.h>

#include "objects/people.h"

struct people_t {
  char cpf[16];
  char name[32];
  char surname[32];
  char gender;
  char date_of_birth[11];
};

people_t *people_init(char cpf[16], char name[32], char surname[32], char gender, char date_of_birth[11]) {
  people_t *person = calloc(1, sizeof(people_t));

  strncpy(person->cpf, cpf, 15);
  strncpy(person->name, name, 31);
  strncpy(person->surname, surname, 31);
  strncpy(person->date_of_birth, date_of_birth, 10);

  return person;
}

const char *people_get_cpf(const people_t *person) {
  return person->cpf;
}

const char *people_get_name(const people_t *person) {
  return person->name;
}

const char *people_get_surname(const people_t *person) {
  return person->surname;
}

char people_get_gender(const people_t *person) {
  return person->gender;
}

const char *people_get_date_of_birth(const people_t *person) {
  return person->date_of_birth;
}

void people_set_name(people_t *person, const char *name) {
  strncpy(person->name, name, 31);
}

void people_set_surname(people_t *person, const char *surname) {
  strncpy(person->surname, surname, 31);
}

void people_set_gender(people_t *person, char gender) {
  person->gender = gender;
}

void people_set_date_of_birth(people_t *person, const char *date_of_birth) {
  strncpy(person->date_of_birth, date_of_birth, 10);
}

void people_destroy(people_t *person) {
  free(person);
}
