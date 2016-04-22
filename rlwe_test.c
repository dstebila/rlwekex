/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "rlwe.h"
#include "rlwe_rand.h"
#include "rlwe_table.h"

const int ROUNDS = 1000;
const int MAX_NOISE = 52;

int chi_squared(RAND_CTX *rand_ctx) {
  int ret = -1;
  uint32_t s[1024];
  uint32_t count[2 * MAX_NOISE];
  memset(count, 0, sizeof(uint32_t) * 2 * MAX_NOISE);
  
  int i, j;
  for(i = 0; i < ROUNDS; i++) {
#ifdef CONSTANT_TIME
    rlwe_sample_ct(s, rand_ctx);
#else
    rlwe_sample(s, rand_ctx);
#endif
    for(j = 0; j < 1024; j++) {
      if(abs(s[j]) >= MAX_NOISE) {
        printf("Noise is out of bounds\n");
        goto err;
      }        
      count[s[j] + MAX_NOISE]++;
    }
  }
  double sum = 0;
  int df = 0; // degrees of freedom
  double chi_sqr = 0;
  for(j = 1; j < 2 * MAX_NOISE; j++) { // skip count[0], which must be zero anyways
    int v = abs(j - MAX_NOISE);
    double point_mass = v > 0 ? (rlwe_table[v][2] - rlwe_table[v - 1][2]) * .5 : (double) rlwe_table[0][2];
    double expect = ROUNDS * 1024 * (point_mass /  0xFFFFFFFFFFFFFFFFULL);
    
    if(expect == 0) {
      if(count[j] == 0)
        continue; // nothing to do
      else {
        printf("Unexpected value (%d) has been output.\n", j - MAX_NOISE);
        goto err;
      }
    }
    printf("%2d: observed %6d, expected %g\n", j - MAX_NOISE, count[j], expect);
    sum += expect;   
    
    chi_sqr += (count[j] - expect) * (count[j] - expect) / expect;
    df++;
  }
  printf("The chi-squared statistic = %f (df = %d)\n", chi_sqr, df);

  if (chi_sqr > 2 * df) { 
    printf("Chi-squared test failed.\n");
    goto err;
  }
  
  ret = 1;
  
 err:
  return ret;
}

int main() {
  RAND_CTX rand_ctx;
  if(!RAND_CTX_init(&rand_ctx)) {
    printf("Cannot initialize PRNG.\n");
    return -1;
  }
  
#ifdef CONSTANT_TIME
    printf("Testing constant time sampling...\n");
#else
    printf("Testing variable time sampling...\n");
#endif
  
  chi_squared(&rand_ctx);
  
  RAND_CTX_cleanup(&rand_ctx);

  return 0;
}
