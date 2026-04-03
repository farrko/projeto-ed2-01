#ifndef EXHASH_H
#define EXHASH_H

#include <stdint.h>

typedef struct exhash_t exhash_t;

/** @brief    Inicialização de um arquivo e estrutura de extendible hashing.
 *
 * @param    bucket_size  Capacidade máxima de registros em cada bucket.
 * @param    data_size    Tamanho dos dados a serem armazenados em cada registro de um bucket.
 * @param    path         Caminho do arquivo para armazenamento de dados.
 *
 * @return   Um ponteiro para a nova estrutura de hashing inicializada.
 */
exhash_t *exh_init(uint16_t bucket_size, uint16_t entry_size, char path[255]);

/** @brief    Lê um arquivo de extendible hashing e carrega suas informações.
 *
 * @param    path  Caminho do arquivo de origem.
 *
 * @return   Um ponteiro para a estrutura exhash_t carregada com os dados do arquivo.
 * @warning  O comportamento é indefinido se o arquivo estiver corrompido ou em formato inválido.
 */
exhash_t *exh_load(char path[255]);

/** @brief    Insere um novo par key-value na tabela, escrevendo a informação no arquivo relacionado.
 *
 * @param    exh     Ponteiro para a tabela de hashing.
 * @param    key     Chave numérica para indexação.
 * @param    data    Ponteiro para os dados a serem armazenados.
 */
void exh_insert(exhash_t *exh, uint32_t key, void *data);

/** @brief    Recupera um dado associado a uma chave.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 * @param    key  Chave de busca.
 *
 * @return   Ponteiro para uma nova região de memória contendo a cópia dos dados.
 *           Retorna NULL caso a chave não seja encontrada.
 */
void *exh_get(exhash_t *exh, uint32_t key);

/** @brief    Remove uma chave e seu dado correspondente da tabela.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 * @param    key  Chave a ser removida.
 */
void exh_remove(exhash_t *exh, uint32_t key);

/** @brief    Destrói a estrutura de extendible hashing, sem deletar o seu arquivo.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 */
void exh_destroy(exhash_t *exh);

#endif
