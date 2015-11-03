/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#ifndef _RLWE_H_
#define _RLWE_H_

#include <stdint.h>

#include "fft.h"
#include "rlwe_rand.h"

#ifdef CONSTANT_TIME
void sample_ct(uint32_t *s, RAND_CTX *rand_ctx);
void round2_ct(uint64_t *out, const uint32_t *in);
void crossround2_ct(uint64_t *out, const uint32_t *in, RAND_CTX *rand_ctx);
void rec_ct(uint64_t *out, const uint32_t *w, const uint64_t *b);
#else
void sample(uint32_t *s, RAND_CTX *rand_ctx);
void round2(uint64_t *out, const uint32_t *in);
void crossround2(uint64_t *out, const uint32_t *in, RAND_CTX *rand_ctx);
void rec(uint64_t *out, const uint32_t *w, const uint64_t *b);
#endif

void key_gen(uint32_t *out, const uint32_t *a, const uint32_t *s, const uint32_t *e, FFT_CTX *ctx);

#endif /* _RLWE_H_ */
