#ifndef PEOPLE_H
#define PEOPLE_H

#include <stddef.h>

/*
 *  PESSOA
 *
 * O módulo de pessoa (people) define a estrutura people_t, responsável por representar
 * os dados cadastrais de um indivíduo. Cada pessoa é identificada unicamente pelo seu CPF
 * e possui informações de nome, sobrenome, gênero e data de nascimento.
 *
 * A estrutura pode ser criada por meio de people_init() e destruída com people_destroy().
 * São disponibilizadas operações de leitura (get) para todos os campos e de escrita (set)
 * para os campos mutáveis. O CPF, por ser o identificador único da pessoa, não possui setter.
 */

typedef struct people_t people_t;

/** @brief   Inicializa uma nova estrutura de pessoa com os dados fornecidos.
 *
 * @param   cpf            CPF identificador da pessoa.
 * @param   name           Nome da pessoa.
 * @param   surname        Sobrenome da pessoa.
 * @param   gender         Gênero da pessoa, representado por um caractere.
 * @param   date_of_birth  Data de nascimento da pessoa no formato DD/MM/AAAA.
 *
 * @return  Ponteiro para a nova estrutura people_t inicializada.
 */
people_t *people_init(char cpf[16], char name[32], char surname[32], char gender, char date_of_birth[11]);

/** @brief   Retorna o tamanho em bytes da estrutura people_t.
 *
 * @return  Tamanho em bytes de people_t.
 */
size_t people_sizeof();

/** @brief   Recupera o CPF identificador da pessoa.
 *
 * @param   person  Ponteiro para a pessoa.
 *
 * @return  Ponteiro para a string contendo o CPF.
 */
const char *people_get_cpf(const people_t *person);

/** @brief   Recupera o nome da pessoa.
 *
 * @param   person  Ponteiro para a pessoa.
 *
 * @return  Ponteiro para a string contendo o nome.
 */
const char *people_get_name(const people_t *person);

/** @brief   Recupera o sobrenome da pessoa.
 *
 * @param   person  Ponteiro para a pessoa.
 *
 * @return  Ponteiro para a string contendo o sobrenome.
 */
const char *people_get_surname(const people_t *person);

/** @brief   Recupera o gênero da pessoa.
 *
 * @param   person  Ponteiro para a pessoa.
 *
 * @return  Caractere representando o gênero da pessoa.
 */
char people_get_gender(const people_t *person);

/** @brief   Recupera a data de nascimento da pessoa.
 *
 * @param   person  Ponteiro para a pessoa.
 *
 * @return  Ponteiro para a string contendo a data de nascimento no formato DD/MM/AAAA.
 */
const char *people_get_date_of_birth(const people_t *person);

/** @brief   Define o nome da pessoa.
 *
 * @param   person  Ponteiro para a pessoa.
 * @param   name    Novo nome a ser atribuído.
 */
void people_set_name(people_t *person, const char *name);

/** @brief   Define o sobrenome da pessoa.
 *
 * @param   person   Ponteiro para a pessoa.
 * @param   surname  Novo sobrenome a ser atribuído.
 */
void people_set_surname(people_t *person, const char *surname);

/** @brief   Define o gênero da pessoa.
 *
 * @param   person  Ponteiro para a pessoa.
 * @param   gender  Novo caractere de gênero a ser atribuído.
 */
void people_set_gender(people_t *person, char gender);

/** @brief   Define a data de nascimento da pessoa.
 *
 * @param   person         Ponteiro para a pessoa.
 * @param   date_of_birth  Nova data de nascimento a ser atribuída, no formato DD/MM/AAAA.
 */
void people_set_date_of_birth(people_t *person, const char *date_of_birth);

/** @brief   Destrói a estrutura de pessoa, liberando a memória alocada.
 *
 * @param   person  Ponteiro para a pessoa a ser destruída.
 */
void people_destroy(people_t *person);

#endif
