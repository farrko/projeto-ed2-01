#include <stdlib.h>
#include <string.h>

#include "objects/block.h"

struct block_t {
  char cep[16];
  double x;
  double y;
  double width;
  double height;
  double border_width;
  char color[16];
  char border_color[16];
};

block_t *block_init(const char cep[16], double x, double y, double width, double height, double border_width, const char color[16], const char border_color[16]) {
  block_t *block = calloc(1, sizeof(block_t));

  strncpy(block->cep, cep, 15);
  block->x = x;
  block->y = y;
  block->width = width;
  block->height = height;
  block->border_width = border_width;
  strncpy(block->color, color, 15);
  strncpy(block->border_color, border_color, 15);

  return block;
}

size_t block_sizeof() {
  return sizeof(block_t);
}

const char *block_get_cep(const block_t *block) {
  return block->cep;
}
double block_get_x(const block_t *block) {
  return block->x;
}
double block_get_y(const block_t *block) {
  return block->y;
}
double block_get_width(const block_t *block) {
  return block->width;
}
double block_get_height(const block_t *block) {
  return block->height;
}
double block_get_border_width(const block_t *block) {
  return block->border_width;
}
const char *block_get_color(const block_t *block) {
  return block->color;
}
const char *block_get_border_color(const block_t *block) {
  return block->border_color;
}

void block_set_x(block_t *block, double x) {
  block->x = x;
}

void block_set_y(block_t *block, double y) {
  block->y = y;
}

void block_set_width(block_t *block, double w) {
  block->width = w;
}

void block_set_height(block_t *block, double h) {
  block->height = h;
}

void block_set_border_width(block_t *block, double border_width) {
  block->border_width = border_width;
}

void block_set_color(block_t *block, const char *color) {
  strncpy(block->color, color, sizeof(block->color) - 1);
  block->color[sizeof(block->color) - 1] = '\0';
}

void block_set_border_color(block_t *block, const char *border_color) {
  strncpy(block->border_color, border_color, sizeof(block->border_color) - 1);
  block->border_color[sizeof(block->border_color) - 1] = '\0';
}

void block_destroy(block_t *block) {
  free(block);
}
