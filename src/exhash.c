#include "exhash.h"

/*

  -- ESTRUTURAÇÃO DO ARQUIVO BINÁRIO --

  GLOBAL DEPTH -> 16 BITS
  BUCKET SIZE -> 16 BITS
  DATA SIZE -> 16 BITS


  TAMANHO DA TABELA -> (2 ^ GLOBAL_DEPTH)
  CADA VALOR DA TABELA -> 32 BITS


  BUCKETS:
  LOCAL DEPTH -> 16 BITS
  OCCUPIED ENTRIES -> 16 BITS
  ENTRIES -> BUCKET SIZE * ENTRY SIZE


  ENTRIES:
  OCCUPIED -> 1 BIT
  KEY -> 32 BITS
  DATA -> DATA SIZE

*/

typedef struct {
  uint32_t key;
  unsigned char bytes[];
} exh_entry_t;

typedef struct {
  uint16_t local_depth;
  exh_entry_t entries[];
} exh_bucket_t;

struct exhash_t {
  uint16_t global_depth;
  uint16_t bucket_size;
  uint16_t entry_size;
  char path[255];
  uint32_t buckets[];
};
