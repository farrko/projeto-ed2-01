#include "files/qry.h"
#include "files/svg.h"
#include "datast/exhash.h"
#include "objects/address.h"
#include "objects/block.h"
#include "objects/people.h"
#include "shapes/text.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void command_rq(char cep[16], exhash_t *blocks, exhash_t *addresses, exhash_t *people, svg_t *svg, FILE *txt) {
  fprintf(txt, "\n\n--- COMANDO RQ --- argumentos: %s --- \n\n", cep);

  block_t *block = exh_remove(blocks, cep);
  if (block == NULL) return;

  text_t *x = text_init(0, block_get_x(block), block_get_y(block), "start", "#880808", "#000000", "sans-serif", "normal", "medium", "X");
  svg_write_text(svg, x);
  text_destroy(x);

  size_t ads_length = exh_entries_amount(addresses);
  uint8_t *ads_list = exh_get_all(addresses);

  for (size_t i = 0; i < ads_length; i++) {
    address_t *current_address = (address_t *)(ads_list + i * ads_sizeof());

    if (strncmp(ads_get_cep(current_address), cep, 15) == 0) {
      address_t *ads = exh_remove(addresses, ads_get_cpf(current_address));
      ads_destroy(ads);

      people_t *person = exh_get(people, ads_get_cpf(current_address));
      fprintf(txt, "- Pessoa removida:\n\t-Nome: %s\n\t- CPF: %s\n\n", people_get_name(person), people_get_cpf(person));

      people_destroy(person);
    }
  }

  free(ads_list);
}

void command_pq() {

}

void command_censo() {

}

void command_h() {

}

void command_nasc() {

}

void command_rip() {

}

void command_mud() {

}

void command_dspj() {

}

void qry_processing() {

}
