#warning "Not using a cryptographically secure pseudorandom number generator.  See README.md and the Makefile to switch to an OpenSSL-based PRNG or define your own."

#include <stdlib.h>
#include <string.h>

#include "rlwe_rand.h"

int RAND_CHOICE_init(RAND_CTX *rand_ctx) {
	return 1;
}

void RAND_CHOICE_cleanup(RAND_CTX *rand_ctx) {
}

uint8_t  RANDOM8(RAND_CTX *rand_ctx) {
	return ((uint8_t) random());
}

uint32_t RANDOM32(RAND_CTX *rand_ctx) {
	return ((uint32_t) (random() << 16) ^ random());
}

uint64_t RANDOM64(RAND_CTX *rand_ctx) {
	return (((uint64_t) RANDOM32 << (uint64_t) 32) | ((uint64_t) RANDOM32));
}

void RANDOM192(uint64_t r[3], RAND_CTX *rand_ctx) {
	r[0] = RANDOM64(rand_ctx);
	r[1] = RANDOM64(rand_ctx);
	r[2] = RANDOM64(rand_ctx);
}

void *(*volatile rlwe_memset_volatile)(void *, int, size_t) = memset;
