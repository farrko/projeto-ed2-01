#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "objects/people.h"

#define CPF_LEN 16
#define NAME_SN_LEN 64
#define DOB_LEN 11

struct people_t {
  char cpf[CPF_LEN];
  char name[NAME_SN_LEN];
  char surname[NAME_SN_LEN];
  char gender;
  char date_of_birth[DOB_LEN];
};

people_t *people_init(const char *cpf, const char *name, const char *surname, char gender, const char *date_of_birth) {
  people_t *person = calloc(1, sizeof(people_t));

  strncpy(person->cpf, cpf, CPF_LEN - 1);
  strncpy(person->name, name, NAME_SN_LEN - 1);
  strncpy(person->surname, surname, NAME_SN_LEN - 1);
  person->gender = gender;
  strncpy(person->date_of_birth, date_of_birth, DOB_LEN - 1);

  return person;
}

size_t people_sizeof() {
  return sizeof(people_t);
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
  strncpy(person->name, name, NAME_SN_LEN - 1);
  person->name[strnlen(name, NAME_SN_LEN - 1)] = '\0';
}

void people_set_surname(people_t *person, const char *surname) {
  strncpy(person->surname, surname, NAME_SN_LEN - 1);
  person->surname[strnlen(surname, NAME_SN_LEN - 1)] = '\0';
}

void people_set_gender(people_t *person, char gender) {
  person->gender = gender;
}

void people_set_date_of_birth(people_t *person, const char *date_of_birth) {
  strncpy(person->date_of_birth, date_of_birth, DOB_LEN - 1);
  person->date_of_birth[strnlen(date_of_birth, DOB_LEN - 1)] = '\0';
}

void people_destroy(people_t *person) {
  free(person);
}
