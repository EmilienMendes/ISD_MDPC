#ifndef MDPC_H
#define MDPC_H

#include "libs/polynome.h"
#include "libs/md5.h"

#include <string.h>
#include <time.h>

#define MD5_HASH_BYTES 16

void privkey_generation(bit **h0, bit **h1, unsigned int nb_rows, unsigned int nb_columns, unsigned int weight, clock_t *start, clock_t *end);
polynome *pubkey_generation(bit **h0, bit **h1, unsigned int nb_rows, unsigned int nb_columns, clock_t *start, clock_t *end);
bit **cypher(bit **e0, bit **e1, bit **h0, bit **h1, polynome *pubkey, int n, int e, clock_t *start, clock_t *end);

int bitflip(int n, bit **e0, bit **e1, bit **h0, bit **h1, bit **s, int T, int t, bit **e0_output, bit **e1_output);
void mdpc(unsigned int w, unsigned int n, unsigned int e, unsigned int T);

#endif