#include "exhash.h"

typedef struct llist_t llist_t; /* placeholder */

typedef struct {
  size_t key;
  size_t nbytes;
  unsigned char data[];
} exh_entry_t;

typedef struct {
  size_t local_depth;
  llist_t *entries;
} exh_bucket_t;

struct exhash_t {
  size_t global_depth;
  size_t bucket_size;
  exh_bucket_t *buckets[];
};
