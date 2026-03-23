#ifndef EXHASH_H
#define EXHASH_H

#include <stddef.h>

typedef struct exhash_t exhash_t;

/** @brief    Inicialização da estrutura de Extendible Hashing.
 *
 * @param    bucket_size  Capacidade máxima de itens em cada bucket.
 *
 * @return   Um ponteiro para a nova estrutura de hashing inicializada.
 * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
 */
exhash_t *exh_init(size_t bucket_size);

/** @brief    Serializa e grava o estado atual da tabela de hashing em um arquivo.
 *
 * @param    exh     Ponteiro para a estrutura a ser persistida.
 * @param    path    Caminho do arquivo (máx. 255 caracteres) onde os dados serão escritos.
 */
void exh_write(exhash_t *exh, char path[255]);

/** @brief    Lê e reconstrói uma tabela de hashing a partir de um arquivo.
 *
 * @param    path  Caminho do arquivo de origem.
 *
 * @return   Um ponteiro para a estrutura exhash_t carregada com os dados do arquivo.
 * @warning  O comportamento é indefinido se o arquivo estiver corrompido ou em formato inválido.
 */
exhash_t *exh_read(char path[255]);

/** @brief    Insere um novo par key-value na tabela.
 *
 * @param    exh     Ponteiro para a tabela de hashing.
 * @param    key     Chave numérica para indexação.
 * @param    data    Ponteiro para os dados a serem armazenados.
 * @param    nbytes  Tamanho em bytes dos dados apontados por data.
 *
 * @note     Esta função realiza uma cópia total dos bytes de data.
 */
void exh_insert(exhash_t *exh, size_t key, void *data, size_t nbytes);

/** @brief    Recupera um dado associado a uma chave.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 * @param    key  Chave de busca.
 *
 * @return   Ponteiro para uma nova região de memória contendo a cópia dos dados.
 *           Retorna NULL caso a chave não seja encontrada.
 *
 * @note     O usuário é responsável por liberar a memória retornada por esta função.
 */
void *exh_get(exhash_t *exh, size_t key);

/** @brief    Remove uma chave e seu dado correspondente da tabela.
 *
 * @param    exh  Ponteiro para a tabela de hashing.
 * @param    key  Chave a ser removida.
 */
void exh_delete(exhash_t *exh, size_t key);

#endif
