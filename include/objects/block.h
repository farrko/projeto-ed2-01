#ifndef BLOCK_H

#include <stddef.h>

typedef struct block_t block_t;

block_t *block_init(const char cep[16], double x, double y, double width, double height, double border_width, const char color[16], const char border_color[16]);

size_t block_sizeof();

const char *block_get_cep(block_t *block);
double block_get_x(block_t *block);
double block_get_y(block_t *block);
double block_get_width(block_t *block);
double block_get_height(block_t *block);
double block_get_border_width(block_t *block);
const char *block_get_color(block_t *block);
const char *block_get_border_color(block_t *block);

void block_set_x(block_t *block, double x);
void block_set_y(block_t *block, double y);
void block_set_width(block_t *block, double w);
void block_set_height(block_t *block, double h);
void block_set_border_width(block_t *block, double border_width);
void block_set_color(block_t *block, const char *color);
void block_set_border_color(block_t *block, const char *border_color);

void block_destroy(block_t *block);

#endif
