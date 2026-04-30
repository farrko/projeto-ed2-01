#ifndef QRY_H
#define QRY_H

#include "svg.h"
#include "datast/exhash.h"

void qry_processing(const char *qrypath, const char *txtpath, svg_t *svg, exhash_t *people, exhash_t *blocks, exhash_t *addresses);

#endif
