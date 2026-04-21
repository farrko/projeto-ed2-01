#ifndef PM_H
#define PM_H

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
  * @param    pm_hfpath   Caminho para o hashfile que conterá os registros de pessoas.
  * @param    ads_hfpath  Caminho para o hashfile que conterá os registros de endereços.
  */
void pm_processing(char *pmpath, char *pm_hfpath, char *ads_hfpath);

#endif
