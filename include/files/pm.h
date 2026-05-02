#ifndef PM_H
#define PM_H

#include "datast/exhash.h"

/*
 *  PM
 *
 * O módulo de PM (pessoa/morador) processa um arquivo de extensão .pm que possui dados
 * de criação de pessoas e endereços (objetos people_t e address_t). O único método deste
 * módulo processa o arquivo e guarda suas informações em hashfiles.
 */

/** @brief    Processa um arquivo .pm e carrega suas informações em hashfiles.
  *
  * @param    pmpath      Caminho do arquivo PM.
  * @param    people      Estrutura de hashfile que conterá os registros de pessoas.
  * @param    addresses   Estrutura de hashfile que conterá os registros de endereços.
  */
void pm_processing(char *pmpath, exhash_t *people, exhash_t *addresses);

#endif
