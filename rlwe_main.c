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

#include "rlwe_kex.h"

#include "fft.h"
#include "rlwe_a.h"
#include "rlwe_rand.h"

#define CHECK_OK(op, val) \
	{ \
		int tmp_ret; \
		tmp_ret = (op); \
		if (tmp_ret != (val)) { \
			fprintf(stderr, "Error (return code %d) at %s:%d\n", tmp_ret, __FILE__, __LINE__); \
			return -1; \
		} \
	}

int main() {

	uint32_t *a = rlwe_a;
	uint32_t s_alice[1024];
	uint32_t b_alice[1024];
	uint32_t s_bob[1024];
	uint32_t b_bob[1024];
	uint64_t c[16];
	uint64_t k_alice[16];
	uint64_t k_bob[16];

	FFT_CTX ctx;
	if (!FFT_CTX_init(&ctx)) {
		printf("Memory allocation error.");
		return -1;
	}

	CHECK_OK(rlwe_kex_generate_keypair(a, s_alice, b_alice, &ctx), 1)
	CHECK_OK(rlwe_kex_generate_keypair(a, s_bob, b_bob, &ctx), 1)

	CHECK_OK(rlwe_kex_compute_key_bob(b_alice, s_bob, c, k_bob, &ctx), 1)
	CHECK_OK(rlwe_kex_compute_key_alice(b_bob, s_alice, c, k_alice, &ctx), 1)

	int keys_match = 1;
	for (int i = 0; i < 16; i++) {
		keys_match &= (k_alice[i] == k_bob[i]);
	}
	if (keys_match) {
		printf("Keys match.\n");
	} else {
		printf("Keys don't match! :(\n");
		FFT_CTX_free(&ctx);
		return -1;
	}

	FFT_CTX_clear(&ctx);
	FFT_CTX_free(&ctx);

	return 0;

}
