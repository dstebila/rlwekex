/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#include <stdlib.h>
#include <string.h>

#include "rlwe_kex.h"

#include "fft.h"
#include "rlwe.h"

void rlwe_kex_generate_keypair(const uint32_t *a, uint32_t s[1024], uint32_t b[1024], FFT_CTX *ctx) {
	uint32_t e[1024];
#if CONSTANT_TIME
	sample_ct(s);
	sample_ct(e);
#else
	sample(s);
	sample(e);
#endif
	key_gen(b, a, s, e, ctx);
	memset((char *) e, 0, 1024 * sizeof(uint32_t));
}

void rlwe_kex_compute_key_alice(const uint32_t b[1024], const uint32_t s[1024], const uint64_t c[16], uint64_t k[16], FFT_CTX *ctx) {
	uint32_t w[1024];
	FFT_mul(w, b, s, ctx);
#if CONSTANT_TIME
	rec_ct(k, w, c);
#else
	rec(k, w, c);
#endif
	memset((char *) w, 0, 1024 * sizeof(uint32_t));
}

void rlwe_kex_compute_key_bob(const uint32_t b[1024], const uint32_t s[1024], uint64_t c[16], uint64_t k[16], FFT_CTX *ctx) {
	uint32_t v[1024];
	uint32_t eprimeprime[1024];
#if CONSTANT_TIME
	sample_ct(eprimeprime);
#else
	sample(eprimeprime);
#endif
	key_gen(v, b, s, eprimeprime, ctx);
#if CONSTANT_TIME
	crossround2_ct(c, v);
	round2_ct(k, v);
#else
	crossround2(c, v);
	round2(k, v);
#endif
	memset((char *) v, 0, 1024 * sizeof(uint32_t));
	memset((char *) eprimeprime, 0, 1024 * sizeof(uint32_t));
}

