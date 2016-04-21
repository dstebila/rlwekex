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

#include "rlwe_rand.h"

#include "rlwe.c"

const int ROUNDS = 10000;
const int MAX_NOISE = 52;

static uint64_t test_values[][3] = {
	{0ULL, 0ULL, 0ULL}, // 0
	{1ULL, 0ULL, 0ULL}, // 1
	{2ULL, 0ULL, 0ULL}, // 2
	{0xFFFFFFFFFFFFFFFFULL, 0ULL, 0ULL}, // 2^64-1
	{0ULL, 1ULL, 0ULL}, // 2^64
	{1ULL, 1ULL, 0ULL}, // 2^64+1
	{0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0ULL}, // 2^128-1
	{0ULL, 0ULL, 1ULL}, // 2^128
	{1ULL, 0ULL, 1ULL}, // 2^128 + 1
	{0x6a09e667f3bcc907ULL, 0xb2fb1366ea957d3eULL,  0x3adec17512775099ULL}, // -1
	{0x6a09e667f3bcc908ULL, 0xb2fb1366ea957d3eULL,  0x3adec17512775099ULL}, // digits of sqrt{2}
	{0x6a09e667f3bcc909ULL, 0xb2fb1366ea957d3eULL,  0x3adec17512775099ULL}, // +1
	{0ULL, 0ULL, 0xFFFFFFFFFFFFFFFFULL},
	{0ULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL},
	{0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL}  // = 2^192-1
};

int test_comparer() {
	int i, j;
	int n = sizeof(test_values) / (sizeof(uint64_t) * 3);

	printf("Testing the comparison function... ");

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if ((i < j) != cmplt_ct(test_values[i], test_values[j])) {
				printf("Test values (%d, %d) are compared incorrectly.\n", i, j);
				return -1;
			}
		}
	}
	printf("OK\n");
	return 1;
}


typedef void FuncSampler(uint32_t *s, RAND_CTX *rand_ctx);

int chi_squared(FuncSampler sampler, RAND_CTX *rand_ctx) {
	int ret = -1;
	uint32_t s[1024];
	uint32_t count[2 * MAX_NOISE];
	memset(count, 0, sizeof(uint32_t) * 2 * MAX_NOISE);

	int i, j;
	for (i = 0; i < ROUNDS; i++) {
		sampler(s, rand_ctx);
		for (j = 0; j < 1024; j++) {
			if (abs(s[j]) >= MAX_NOISE) {
				printf("Noise is out of bounds\n");
				goto err;
			}
			count[s[j] + MAX_NOISE]++;
		}
	}
	double sum = 0;
	int df = 0; // degrees of freedom
	double chi_sqr = 0;
	for (j = 1; j < 2 * MAX_NOISE; j++) { // skip count[0], which must be zero anyways
		int v = abs(j - MAX_NOISE);
		double point_mass = v > 0 ? (rlwe_table[v][2] - rlwe_table[v - 1][2]) * .5 : (double) rlwe_table[0][2];
		double expect = ROUNDS * 1024 * (point_mass /  0xFFFFFFFFFFFFFFFFULL);

		if (expect == 0) {
			if (count[j] == 0) {
				continue; // nothing to do
			} else {
				printf("Unexpected value (%d) has been output.\n", j - MAX_NOISE);
				goto err;
			}
		}
		printf("%2d: observed %7d, expected %g\n", j - MAX_NOISE, count[j], expect);
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
	int ret = -1;

	RAND_CTX rand_ctx;

	if (!RAND_CHOICE_init(&rand_ctx)) {
		printf("Cannot initialize PRNG.\n");
		return -1;
	}

	if (test_comparer() <= 0) {
		goto cleanup;
	}

#ifdef CONSTANT_TIME
	printf("Testing constant time sampling...\n");
	chi_squared(rlwe_sample_ct, &rand_ctx);
#else
	printf("Testing variable time sampling...\n");
	chi_squared(rlwe_sample, &rand_ctx);
#endif

	ret = 0; // success!
cleanup:
	RAND_CHOICE_cleanup(&rand_ctx);
	return ret;
}
