#ifndef GEO_H

/*
 *  GEO
 *
 * O módulo GEO tem como função o processamento de um arquivo .geo, que possui informações de
 * quadras em uma cidade. Ele instancia objetos block_t com os dados do arquivo e os registra
 * em um hashfile.
 */


/** @brief    Processa um arquivo .geo e carrega suas informações em um hashfile.
  *
  * @param    geopath  Caminho do arquivo geo.
  * @param    hfpath   Caminho para o hashfile que conterá os registros de quadras.
  */
void geo_processing(char *geopath, char *hfpath);

#endif
