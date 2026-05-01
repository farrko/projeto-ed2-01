#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/*
 *  UTILITÁRIOS
 *
 * Este módulo reúne funções utilitárias de uso geral, sem vínculo com nenhum módulo específico
 * do sistema. As funções aqui presentes complementam a biblioteca padrão do C com operações
 * recorrentes que não são nativamente disponibilizadas por ela.
 */

/** @brief   Concatena duas strings em uma nova região de memória.
 *
 * @param   s1  Primeira string.
 * @param   s2  Segunda string.
 *
 * @return  Ponteiro para uma nova string contendo a concatenação de s1 e s2.
 */
char *r_strcat(char *s1, char *s2);

size_t strnlen(const char *str, size_t maxlen);

#endif
