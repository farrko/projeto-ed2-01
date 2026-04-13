#ifndef PEOPLE_H

typedef struct people_t people_t;

people_t *people_init(char cpf[16], char name[32], char surname[32], char gender, char date_of_birth[11]);

const char *people_get_cpf(const people_t *person);
const char *people_get_name(const people_t *person);
const char *people_get_surname(const people_t *person);
char people_get_gender(const people_t *person);
const char *people_get_date_of_birth(const people_t *person);

void people_set_name(people_t *person, const char *name);
void people_set_surname(people_t *person, const char *surname);
void people_set_gender(people_t *person, char gender);
void people_set_date_of_birth(people_t *person, const char *date_of_birth);

void people_destroy(people_t *person);

#endif
