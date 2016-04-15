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
#include <stdlib.h>
#include <string.h>

#include "rlwe_kex.h"

#include "fft.h"
#include "rlwe.h"
#include "rlwe_rand.h"

int rlwe_kex_generate_keypair(const uint32_t *a, uint32_t s[1024], uint32_t b[1024], FFT_CTX *ctx) {
	int ret;
	uint32_t e[1024];
	RAND_CTX rand_ctx;
	ret = RAND_CHOICE_init(&rand_ctx);
	if (!ret) {
		return ret;
	}
#if CONSTANT_TIME
	rlwe_sample_ct(s, &rand_ctx);
	rlwe_sample_ct(e, &rand_ctx);
#else
	rlwe_sample(s, &rand_ctx);
	rlwe_sample(e, &rand_ctx);
#endif
	rlwe_key_gen(b, a, s, e, ctx);
	rlwe_memset_volatile(e, 0, 1024 * sizeof(uint32_t));
	RAND_CHOICE_cleanup(&rand_ctx);
	return ret;
}

int rlwe_kex_compute_key_alice(const uint32_t b[1024], const uint32_t s[1024], const uint64_t c[16], uint64_t k[16], FFT_CTX *ctx) {
	uint32_t w[1024];
	FFT_mul(w, b, s, ctx);
#if CONSTANT_TIME
	rlwe_rec_ct(k, w, c);
#else
	rlwe_rec(k, w, c);
#endif
	rlwe_memset_volatile(w, 0, 1024 * sizeof(uint32_t));
	return 1;
}

int rlwe_kex_compute_key_bob(const uint32_t b[1024], const uint32_t s[1024], uint64_t c[16], uint64_t k[16], FFT_CTX *ctx) {
	int ret;
	uint32_t v[1024];
	uint32_t eprimeprime[1024];
	RAND_CTX rand_ctx;
	ret = RAND_CHOICE_init(&rand_ctx);
	if (!ret) {
		return ret;
	}
#if CONSTANT_TIME
	rlwe_sample_ct(eprimeprime, &rand_ctx);
#else
	rlwe_sample(eprimeprime, &rand_ctx);
#endif
	rlwe_key_gen(v, b, s, eprimeprime, ctx);
#if CONSTANT_TIME
	rlwe_crossround2_ct(c, v, &rand_ctx);
	rlwe_round2_ct(k, v);
#else
	rlwe_crossround2(c, v, &rand_ctx);
	rlwe_round2(k, v);
#endif
	rlwe_memset_volatile(v, 0, 1024 * sizeof(uint32_t));
	rlwe_memset_volatile(eprimeprime, 0, 1024 * sizeof(uint32_t));
	RAND_CHOICE_cleanup(&rand_ctx);
	return ret;
}

