#warning "Using OpenSSL's RAND_bytes to generate all randomness for sampling will result in very slow performance.  See README.md and the Makefile to switch to an AES-based PRNG seeded from OpenSSL's RAND_bytes."

#include "rlwe_rand.h"

#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

int RAND_CHOICE_init(RAND_CTX *rand_ctx) {
	return 1;
}

void RAND_CHOICE_cleanup(RAND_CTX *rand_ctx) {
}

uint8_t  RANDOM8(RAND_CTX *rand_ctx) {
	uint8_t b;
	int r = RAND_bytes((unsigned char *) &b, 1);
	if (r != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return b;
}

uint32_t RANDOM32(RAND_CTX *rand_ctx) {
	uint32_t b;
	int r = RAND_bytes((unsigned char *) &b, 4);
	if (r != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return b;
}

uint64_t RANDOM64(RAND_CTX *rand_ctx) {
	uint64_t b;
	int r = RAND_bytes((unsigned char *) &b, 8);
	if (r != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return b;
}

void RANDOM192(uint64_t x[3], RAND_CTX *rand_ctx) {
	int r = RAND_bytes((unsigned char *) x, 24);
	if (r != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
}

void *(*volatile rlwe_memset_volatile)(void *, int, size_t) = memset;
