#include "exhash.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


#define HASHTABLE_START 6

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
  KEY -> 4 BYTES
  DATA -> DATA SIZE

*/

typedef struct {
  bool occupied;
  uint32_t key;
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
  uint16_t entry_size;
  char path[255];
  FILE *headerfile;
  FILE *hashfile;
  uint32_t *buckets;
};

exh_bucket_t *exh_load_bucket(exhash_t *exh, uint32_t bucket_pos) {
  fseek(exh->hashfile, bucket_pos, SEEK_SET);

  exh_bucket_t *bucket = calloc(1, sizeof(exh_bucket_t));
  bucket->bucket_pos = bucket_pos;
  fread(&bucket->local_depth, sizeof(uint16_t), 1, exh->hashfile);
  fread(&bucket->occupied_entries, sizeof(uint16_t), 1, exh->hashfile);

  bucket->entries = calloc(exh->bucket_size, sizeof(exh_entry_t *));

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    exh_entry_t *entry = calloc(1, sizeof(exh_entry_t));
    entry->bytes = calloc(exh->entry_size, sizeof(unsigned char));
    bucket->entries[i] = entry;

    fread(&entry->occupied, sizeof(bool), 1, exh->hashfile);
    fread(&entry->key, sizeof(uint32_t), 1, exh->hashfile);
    fread(entry->bytes, sizeof(unsigned char), exh->entry_size, exh->hashfile);
  }

  return bucket;
}

void exh_update_bucket(exhash_t *exh, exh_bucket_t *bucket) {
  fseek(exh->hashfile, bucket->bucket_pos, SEEK_SET);

  fwrite(&bucket->local_depth, sizeof(uint16_t), 1, exh->hashfile);
  fwrite(&bucket->occupied_entries, sizeof(uint16_t), 1, exh->hashfile);

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    fwrite(&bucket->entries[i]->occupied, sizeof(bool), 1, exh->hashfile);
    fwrite(&bucket->entries[i]->key, sizeof(uint32_t), 1, exh->hashfile);
    fwrite(bucket->entries[i]->bytes, exh->entry_size, 1, exh->hashfile);
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
    entry->bytes = calloc(exh->entry_size, sizeof(unsigned char));
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
  fwrite(exh->buckets, sizeof(uint32_t), (1 << exh->global_depth), exh->headerfile);
  fflush(exh->headerfile);
}

void exh_extend_hashtable(exhash_t *exh) {
  exh->global_depth++;
  fseek(exh->headerfile, 0, SEEK_SET);
  fwrite(&exh->global_depth, sizeof(uint16_t), 1, exh->headerfile);

  exh->buckets = realloc(exh->buckets, 1 << exh->global_depth);

  for (uint16_t i = 0; i < (1 << (exh->global_depth - 1)); i++) {
    exh->buckets[(1 << (exh->global_depth - 1)) + i] = exh->buckets[i];
  }

  exh_update_hashtable(exh);
}

exhash_t *exh_init(uint16_t bucket_size, uint16_t entry_size, char path[255]) {
  exhash_t *exh = malloc(sizeof(exhash_t));

  exh->global_depth = 0;
  exh->bucket_size = bucket_size;
  exh->entry_size = entry_size;
  strcpy(exh->path, path);

  char *hfcpath = r_strcat(exh->path, ".hfc");
  exh->headerfile = fopen(hfcpath, "w+b");
  free(hfcpath);

  fwrite(&exh->global_depth, sizeof(uint16_t), 1, exh->headerfile);
  fwrite(&exh->bucket_size, sizeof(uint16_t), 1, exh->headerfile);
  fwrite(&exh->entry_size, sizeof(uint16_t), 1, exh->headerfile);

  char *hfpath = r_strcat(exh->path, ".hf");
  exh->hashfile = fopen(hfpath, "w+b");
  free(hfpath);

  exh->buckets = calloc(1, sizeof(uint32_t));
  exh_bucket_t *initial = exh_create_bucket(exh, 0);
  exh_destroy_bucket(exh, initial);

  exh_update_hashtable(exh);

  return exh;
}

exhash_t *exh_load(char path[255]) {
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
  fread(&exh->entry_size, sizeof(uint16_t), 1, exh->headerfile);

  exh->buckets = calloc(1 << exh->global_depth, sizeof(uint32_t));
  fread(exh->buckets, sizeof(uint32_t), 1 << exh->global_depth, exh->headerfile);

  return exh;
}

void exh_insert(exhash_t *exh, uint32_t key, void *data) {
  uint32_t hash = key % (1 << exh->global_depth);
  uint32_t bucket_pos = exh->buckets[hash];

  exh_bucket_t *bucket = exh_load_bucket(exh, bucket_pos);

  if (bucket->occupied_entries < exh->bucket_size) {
    for (uint16_t i = 0; i < exh->bucket_size; i++) {
      if (bucket->entries[i]->occupied) continue;
      bucket->occupied_entries++;

      bucket->entries[i]->occupied = true;
      bucket->entries[i]->key = key;
      memcpy(bucket->entries[i]->bytes, data, exh->entry_size);

      exh_update_bucket(exh, bucket);
      exh_destroy_bucket(exh, bucket);

      return;
    }
  } else {
    if (bucket->local_depth < exh->global_depth) {
      bucket->local_depth++;

      exh_bucket_t *split_bucket = exh_create_bucket(exh, bucket->local_depth);
      exh->buckets[hash] = split_bucket->bucket_pos;

      for (uint16_t i = 0; i < exh->bucket_size; i++) {
        if (!bucket->entries[i]->occupied) continue;

        uint32_t new_hash = bucket->entries[i]->key % (1 << exh->global_depth);
        if (new_hash != hash) continue;

        split_bucket->occupied_entries++;
        split_bucket->entries[i]->occupied = true;
        split_bucket->entries[i]->key = bucket->entries[i]->key;
        memcpy(split_bucket->entries[i]->bytes, bucket->entries[i]->bytes, exh->entry_size);

        bucket->occupied_entries--;
        bucket->entries[i]->occupied = false;
        memset(bucket->entries[i]->bytes, 0, exh->entry_size);
      }

      exh_update_hashtable(exh);

      exh_update_bucket(exh, bucket);
      exh_update_bucket(exh, split_bucket);

      exh_destroy_bucket(exh, bucket);
      exh_destroy_bucket(exh, split_bucket);

      return exh_insert(exh, key, data);
    } else {
      exh_extend_hashtable(exh);
      return exh_insert(exh, key, data);
    }
  }
}

void *exh_get(exhash_t *exh, uint32_t key) {
  uint32_t hash = key % (1 << exh->global_depth);
  uint32_t bucket_pos = exh->buckets[hash];

  exh_bucket_t *bucket = exh_load_bucket(exh, bucket_pos);
  void *data = NULL;

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    if (!bucket->entries[i]->occupied) continue;
    if (bucket->entries[i]->key != key) continue;

    data = calloc(1, exh->entry_size);
    memcpy(data, bucket->entries[i]->bytes, exh->entry_size);
  }

  exh_destroy_bucket(exh, bucket);
  return data;
}

void exh_remove(exhash_t *exh, uint32_t key) {
  uint32_t hash = key % (1 << exh->global_depth);
  uint32_t bucket_pos = exh->buckets[hash];

  exh_bucket_t *bucket = exh_load_bucket(exh, bucket_pos);

  for (uint16_t i = 0; i < exh->bucket_size; i++) {
    if (!bucket->entries[i]->occupied) continue;
    if (bucket->entries[i]->key != key) continue;

    bucket->entries[i]->occupied = false;
    bucket->entries[i]->key = 0;
    memset(bucket->entries[i]->bytes, 0, exh->entry_size);

    exh_update_bucket(exh, bucket);
    break;
  }

  exh_destroy_bucket(exh, bucket);
}

void exh_destroy(exhash_t *exh) {
  fclose(exh->headerfile);
  fclose(exh->hashfile);
  free(exh->buckets);
  free(exh);
}
