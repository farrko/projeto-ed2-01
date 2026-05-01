#include "datast/exhash.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


#define HASHTABLE_START 8

/*

  -- ESTRUTURAÇÃO DO ARQUIVO BINÁRIO --

  GLOBAL DEPTH -> 2 BYTES
  BUCKET SIZE -> 2 BYTES
  DATA SIZE -> 2 BYTES


  TAMANHO DA TABELA -> (2 ^ GLOBAL_DEPTH)
  CADA VALOR DA TABELA -> 4 BYTES


  BUCKETS:
  LOCAL DEPTH -> 2 BYTES
  OCCUPIED ENTRIES -> 2 BYTES
  ENTRIES -> BUCKET SIZE * ENTRY SIZE


  ENTRIES:
  OCCUPIED -> 1 BYTE
  KEY -> 8 BYTES
  DATA -> DATA SIZE

*/

typedef struct {
  bool occupied;
  uint64_t key;
  unsigned char *bytes;
} exh_entry_t;

typedef struct {
  uint32_t bucket_pos;
  uint16_t local_depth;
  uint16_t occupied_entries;
  exh_entry_t **entries;
} exh_bucket_t;

struct exhash_t {
  uint16_t global_depth;
  uint16_t bucket_size;
  uint16_t data_size;
  uint16_t entries_amount;
  char path[255];
  FILE *headerfile;
  FILE *hashfile;
  uint32_t *buckets;
};

uint64_t calculate_numerical_key(const char *key) {
  // Algoritmo de hash: djb2
  // http://www.cse.yorku.ca/~oz/hash.html

  uint64_t nkey = 5381;
  int c;
  
  while((c = *key++)) nkey = ((nkey << 5) + nkey) + c;
  
  return nkey;
}

int cmp_uint32(const void *a, const void *b) {
    return (*(uint32_t*)a > *(uint32_t*)b) - (*(uint32_t*)a < *(uint32_t*)b);
}

exh_bucket_t *exh_load_bucket(exhash_t *exh, uint32_t bucket_pos) {
  fseek(exh->hashfile, bucket_pos, SEEK_SET);

  exh_bucket_t *bucket = calloc(1, sizeof(exh_bucket_t));
  bucket->bucket_pos = bucket_pos;
  fread(&bucket->local_depth, sizeof(uint16_t), 1, exh->hashfile);
  fread(&bucket->occupied_entries, sizeof(uint16_t), 1, exh->hashfile);

  bucket->entries = calloc(exh->bucket_size, sizeof(exh_entry_t *));

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    exh_entry_t *entry = calloc(1, sizeof(exh_entry_t));
    entry->bytes = calloc(exh->data_size, sizeof(unsigned char));
    bucket->entries[i] = entry;

    fread(&entry->occupied, sizeof(bool), 1, exh->hashfile);
    fread(&entry->key, sizeof(uint64_t), 1, exh->hashfile);
    fread(entry->bytes, sizeof(unsigned char), exh->data_size, exh->hashfile);
  }

  return bucket;
}

void exh_update_bucket(exhash_t *exh, exh_bucket_t *bucket) {
  fseek(exh->hashfile, bucket->bucket_pos, SEEK_SET);

  fwrite(&bucket->local_depth, sizeof(uint16_t), 1, exh->hashfile);
  fwrite(&bucket->occupied_entries, sizeof(uint16_t), 1, exh->hashfile);

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    fwrite(&bucket->entries[i]->occupied, sizeof(bool), 1, exh->hashfile);
    fwrite(&bucket->entries[i]->key, sizeof(uint64_t), 1, exh->hashfile);
    fwrite(bucket->entries[i]->bytes, exh->data_size, 1, exh->hashfile);
  }

  fflush(exh->hashfile);
}

exh_bucket_t *exh_create_bucket(exhash_t *exh, uint16_t local_depth) {
  exh_bucket_t *bucket = calloc(1, sizeof(exh_bucket_t));

  fseek(exh->hashfile, 0, SEEK_END);
  bucket->bucket_pos = ftell(exh->hashfile);
  bucket->local_depth = local_depth;
  bucket->occupied_entries = 0;
  bucket->entries = calloc(exh->bucket_size, sizeof(exh_entry_t *));

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    exh_entry_t *entry = calloc(1, sizeof(exh_entry_t));
    entry->bytes = calloc(exh->data_size, sizeof(unsigned char));
    bucket->entries[i] = entry;
  }

  exh_update_bucket(exh, bucket);

  return bucket;
}

void exh_destroy_bucket(exhash_t *exh, exh_bucket_t *bucket) {
  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    free(bucket->entries[i]->bytes);
    free(bucket->entries[i]);
  }

  free(bucket->entries);
  free(bucket);
}

void exh_update_hashtable(exhash_t *exh) {
  fseek(exh->headerfile, HASHTABLE_START, SEEK_SET);
  fwrite(exh->buckets, sizeof(uint32_t), (size_t) 1 << exh->global_depth, exh->headerfile);
  fflush(exh->headerfile);
}

void exh_extend_hashtable(exhash_t *exh) {
  exh->global_depth++;
  fseek(exh->headerfile, 0, SEEK_SET);
  fwrite(&exh->global_depth, sizeof(uint16_t), 1, exh->headerfile);

  exh->buckets = realloc(exh->buckets, ((size_t) 1 << exh->global_depth) * sizeof(uint32_t));

  for (uint16_t i = 0; i < (1 << (exh->global_depth - 1)); i++) {
    exh->buckets[(1 << (exh->global_depth - 1)) + i] = exh->buckets[i];
  }

  exh_update_hashtable(exh);
}

exhash_t *exh_init(uint16_t bucket_size, uint16_t data_size, const char *path) {
  exhash_t *exh = malloc(sizeof(exhash_t));

  exh->global_depth = 0;
  exh->bucket_size = bucket_size;
  exh->data_size = data_size;
  exh->entries_amount = 0;
  strcpy(exh->path, path);

  char *hfcpath = r_strcat(exh->path, ".hfc");
  exh->headerfile = fopen(hfcpath, "w+b");
  free(hfcpath);

  fwrite(&exh->global_depth, sizeof(uint16_t), 1, exh->headerfile);
  fwrite(&exh->bucket_size, sizeof(uint16_t), 1, exh->headerfile);
  fwrite(&exh->data_size, sizeof(uint16_t), 1, exh->headerfile);
  fwrite(&exh->entries_amount, sizeof(uint16_t), 1, exh->headerfile);

  char *hfpath = r_strcat(exh->path, ".hf");
  exh->hashfile = fopen(hfpath, "w+b");
  free(hfpath);

  exh->buckets = calloc(1, sizeof(uint32_t));
  exh_bucket_t *initial = exh_create_bucket(exh, 0);
  exh_destroy_bucket(exh, initial);

  exh_update_hashtable(exh);

  return exh;
}

exhash_t *exh_load(const char *path) {
  exhash_t *exh = malloc(sizeof(exhash_t));

  strcpy(exh->path, path);

  char *hfcpath = r_strcat(exh->path, ".hfc");
  exh->headerfile = fopen(hfcpath, "r+b");
  free(hfcpath);

  char *hfpath = r_strcat(exh->path, ".hf");
  exh->hashfile = fopen(hfpath, "r+b");
  free(hfpath);
  
  fseek(exh->headerfile, 0, SEEK_END);
  fseek(exh->headerfile, 0, SEEK_SET);

  fread(&exh->global_depth, sizeof(uint16_t), 1, exh->headerfile);
  fread(&exh->bucket_size, sizeof(uint16_t), 1, exh->headerfile);
  fread(&exh->data_size, sizeof(uint16_t), 1, exh->headerfile);
  fread(&exh->entries_amount, sizeof(uint16_t), 1, exh->headerfile);

  exh->buckets = calloc((size_t) 1 << exh->global_depth, sizeof(uint32_t));
  fread(exh->buckets, sizeof(uint32_t), (size_t) 1 << exh->global_depth, exh->headerfile);

  return exh;
}

void exh_insert(exhash_t *exh, const char *key, void *data) {
  uint64_t nkey = calculate_numerical_key(key);

  uint32_t hash = nkey % ((uint32_t) 1 << exh->global_depth);
  uint32_t bucket_pos = exh->buckets[hash];

  exh_bucket_t *bucket = exh_load_bucket(exh, bucket_pos);

  if (bucket->occupied_entries < exh->bucket_size) {
    for (uint16_t i = 0; i < exh->bucket_size; i++) {
      if (bucket->entries[i]->occupied) continue;
      bucket->occupied_entries++;
      exh->entries_amount++;

      bucket->entries[i]->occupied = true;
      bucket->entries[i]->key = nkey;
      memcpy(bucket->entries[i]->bytes, data, exh->data_size);

      exh_update_bucket(exh, bucket);
      exh_destroy_bucket(exh, bucket);

      return;
    }
  } else {
    if (bucket->local_depth < exh->global_depth) {
      uint16_t new_local_depth = bucket->local_depth + 1;
      uint32_t discriminant_bit = (uint32_t)1 << bucket->local_depth;

      exh_bucket_t *split_bucket = exh_create_bucket(exh, new_local_depth);
      bucket->local_depth = new_local_depth;

      // redistribui as entradas com base no bit discriminante
      for (uint16_t i = 0; i < exh->bucket_size; i++) {
        if (!bucket->entries[i]->occupied) continue;

        if (bucket->entries[i]->key & discriminant_bit) {
          split_bucket->occupied_entries++;
          split_bucket->entries[i]->occupied = true;
          split_bucket->entries[i]->key = bucket->entries[i]->key;
          memcpy(split_bucket->entries[i]->bytes, bucket->entries[i]->bytes, exh->data_size);

          bucket->occupied_entries--;
          bucket->entries[i]->occupied = false;
          memset(bucket->entries[i]->bytes, 0, exh->data_size);
        }
      }

      // atualiza TODOS os índices da tabela que apontavam para o bucket original
      size_t table_size = (size_t)1 << exh->global_depth;
      for (size_t i = 0; i < table_size; i++) {
        if (exh->buckets[i] == bucket->bucket_pos && (i & discriminant_bit)) {
          exh->buckets[i] = split_bucket->bucket_pos;
        }
      }

      exh_update_hashtable(exh);
      exh_update_bucket(exh, bucket);
      exh_update_bucket(exh, split_bucket);

      exh_destroy_bucket(exh, bucket);
      exh_destroy_bucket(exh, split_bucket);

      return exh_insert(exh, key, data);
    } else {
      exh_destroy_bucket(exh, bucket);
      exh_extend_hashtable(exh);
      return exh_insert(exh, key, data);
    }
  }
}

void *exh_get(exhash_t *exh, const char *key) {
  uint64_t nkey = calculate_numerical_key(key);

  uint32_t hash = nkey % ((uint32_t) 1 << exh->global_depth);
  uint32_t bucket_pos = exh->buckets[hash];

  exh_bucket_t *bucket = exh_load_bucket(exh, bucket_pos);
  void *data = NULL;

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    if (!bucket->entries[i]->occupied) continue;
    if (bucket->entries[i]->key != nkey) continue;

    data = calloc(1, exh->data_size);
    memcpy(data, bucket->entries[i]->bytes, exh->data_size);
  }

  exh_destroy_bucket(exh, bucket);
  return data;
}

uint16_t exh_entries_amount(exhash_t *exh) {
  return exh->entries_amount;
}

void *exh_get_all(exhash_t *exh) {
  void *objects = calloc(exh->entries_amount, exh->data_size);
  size_t objects_amount = 0;

  size_t table_size = (size_t)1 << exh->global_depth;

  uint32_t *sorted_bucket_pos = malloc((1 << exh->global_depth) * sizeof(uint32_t));
  memcpy(sorted_bucket_pos, exh->buckets, (1 << exh->global_depth) * sizeof(uint32_t));
  qsort(sorted_bucket_pos, 1 << exh->global_depth, sizeof(uint32_t), cmp_uint32);

  for (size_t i = 0; i < table_size; i++) {
    if (objects_amount >= exh->entries_amount) break;

    if (i == 0 || sorted_bucket_pos[i] != sorted_bucket_pos[i - 1]) {
      exh_bucket_t *bucket = exh_load_bucket(exh, sorted_bucket_pos[i]);

      uint16_t visited = 0;
      for (size_t j = 0; j < exh->bucket_size; j++) {
        if (visited >= bucket->occupied_entries) break;
        if (!bucket->entries[j]->occupied) continue;

        visited++;
        memcpy((uint8_t *) objects + objects_amount * exh->data_size, bucket->entries[j]->bytes, exh->data_size);
        objects_amount++;
      }

      exh_destroy_bucket(exh, bucket);
    }
  }

  free(sorted_bucket_pos);

  return objects;
}

void *exh_remove(exhash_t *exh, const char *key) {
  uint64_t nkey = calculate_numerical_key(key);

  uint32_t hash = nkey % ((uint32_t) 1 << exh->global_depth);
  uint32_t bucket_pos = exh->buckets[hash];

  exh_bucket_t *bucket = exh_load_bucket(exh, bucket_pos);

  void *buf = NULL;

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    if (!bucket->entries[i]->occupied) continue;
    if (bucket->entries[i]->key != nkey) continue;

    buf = malloc(exh->data_size);
    memcpy(buf, bucket->entries[i]->bytes, exh->data_size);

    bucket->entries[i]->occupied = false;
    bucket->entries[i]->key = 0;
    memset(bucket->entries[i]->bytes, 0, exh->data_size);

    exh->entries_amount--;

    exh_update_bucket(exh, bucket);
    break;
  }

  exh_destroy_bucket(exh, bucket);

  return buf;
}

void exh_destroy(exhash_t *exh) {
  fclose(exh->headerfile);
  fclose(exh->hashfile);
  free(exh->buckets);
  free(exh);
}
