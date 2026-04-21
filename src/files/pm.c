#include "files/pm.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "datast/exhash.h"
#include "objects/people.h"
#include "objects/address.h"

void pm_processing(char *pmpath, char *pm_hfpath, char *ads_hfpath) {
  FILE *pm = fopen(pmpath, "r");

  exhash_t *people = exh_init(5, people_sizeof(), pm_hfpath);
  exhash_t *addresses = exh_init(5, ads_sizeof(), ads_hfpath);

  char buf[255];
  while(fgets(buf, 255, pm)) {
    if (!strncmp(buf, "p", 1)) {
      char cpf[16];
      char name[32];
      char surname[32];
      char gender;
      char dob[11];

      sscanf(buf, "%*s %s %s %s %c %s", cpf, name, surname, &gender, dob);

      people_t *person = people_init(cpf, name, surname, gender, dob);
      exh_insert(people, cpf, person);

      people_destroy(person);
    }

    if (!strncmp(buf, "m", 1)) {
      char cpf[16];
      char cep[16];
      char face;
      uint16_t number;
      char complement[32];

      sscanf(buf, "%*s %s %s %c %" SCNu16 " %s", cpf, cep, &face, &number, complement);

      address_t *ads = ads_init(cpf, cep, face, number, complement);
      exh_insert(addresses, cpf, ads);

      ads_destroy(ads);
    }
  }

  exh_destroy(people);
  exh_destroy(addresses);
}
