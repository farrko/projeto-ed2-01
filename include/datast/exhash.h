#ifndef EXHASH_H
#define EXHASH_H

#include <stdint.h>

/*
 *  EXTENDIBLE HASHING
 *
 * A estrutura de extendible hashing (exhash) possui como objetivo armazenar informações em disco de forma na qual seja possível
 * realizar uma rápida leitura, inserção e remoção de dados. Para isso, ela se utiliza de dois arquivos: um header file,
 * que guarda uma hash table que associa chaves numéricas à posições de um hash file, e este hash file, que armazena os
 * dados em forma de buckets. A hash table indica onde começa o bucket onde está ou ficará guardada uma informação.
 * A estrutura, como o nome indica, expande e retrai seu tamanho automaticamente, para acomodar qualquer volume de dados.
 *
 * Uma estrutura de exhash pode ser ou inicializada do zero (criando seus arquivos) por meio do exh_init(), ou carregada
 * a partir de arquivos existentes por meio do exh_load(). São disponibilizadas operações de get, insert e delete.
 */

typedef struct exhash_t exhash_t;

/** @brief    Inicialização de uma estrutura de extendible hashing e seus arquivos associados.
 *
 * @param    bucket_size  Capacidade máxima de registros em cada bucket.
 * @param    data_size    Tamanho dos dados a serem armazenados em cada registro de um bucket.
 * @param    path         Caminho do arquivo para armazenamento de dados, sem extensão (serão criados dois arquivos: {path}.hf e {path}.hfc)
 *
 * @return   Um ponteiro para a nova estrutura de hashing inicializada.
 */
exhash_t *exh_init(uint16_t bucket_size, uint16_t data_size, char path[255]);

/** @brief    Lê os arquivos de extendible hashing e carrega suas informações.
 *
 * @param    path  Caminho dos arquivos de origem, sem extensão (serão lidos os arquivos {path}.hf e {path}.hfc)
 *
 * @return   Um ponteiro para a estrutura exhash_t carregada com os dados dos arquivos.
 * @warning  O comportamento é indefinido se o arquivo estiver corrompido ou em formato inválido.
 */
exhash_t *exh_load(char path[255]);

/** @brief    Insere um novo par key-value na tabela, escrevendo a informação no arquivo relacionado.
 *
 * @param    exh     Ponteiro para a tabela de hashing.
 * @param    key     Chave numérica para indexação.
 * @param    data    Ponteiro para os dados a serem armazenados.
 */
void exh_insert(exhash_t *exh, const char *key, void *data);

/** @brief    Recupera um dado associado a uma chave.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 * @param    key  Chave de busca.
 *
 * @return   Ponteiro para uma nova região de memória contendo a cópia dos dados.
 *           Retorna NULL caso a chave não seja encontrada.
 */
void *exh_get(exhash_t *exh, const char *key);

/** @brief    Retorna o número de entradas armazenadas na tabela.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 *
 * @return   Número de objetos atualmente armazenados na tabela.
 */
uint16_t exh_entries_amount(exhash_t *exh);

/** @brief    Recupera todos os dados armazenados na tabela, iterando sobre os buckets do hash file.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 *
 * @return   Ponteiro para uma nova região de memória contendo uma lista com cópias de todos os dados armazenados.
 *           Retorna NULL caso a tabela esteja vazia ou ocorra algum erro.
 */
void *exh_get_all(exhash_t *exh);

/** @brief    Remove uma chave e seu dado correspondente da tabela.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 * @param    key  Chave a ser removida.
 *
 * @return   Ponteiro para o objeto, agora removido da tabela.
 *           Retorna NULL caso o objeto não existisse na tabela ou ocorra algum erro.
 */
void *exh_remove(exhash_t *exh, const char *key);

/** @brief    Destrói a estrutura de extendible hashing (não destrói os arquivos associados).
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 */
void exh_destroy(exhash_t *exh);

#endif
