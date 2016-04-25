#warning "Using /dev/urandom to generate all randomness for sampling will result in very slow performance.  See README.md and the Makefile to switch to an AES-based PRNG seeded from OpenSSL's RAND_bytes."

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "rlwe_rand.h"


int RAND_CHOICE_init(int *rand_ctx) {
	int fd = open("/dev/urandom", O_RDONLY);
	if (!fd) {
		return 0;
	}
	*rand_ctx = fd;
	return 1;
}

void RAND_CHOICE_cleanup(int *rand_ctx) {
	if (*rand_ctx) {
		close(*rand_ctx);
	}
}
static void mkrandom(int *rand_ctx, uint8_t *buf, size_t len) {
	read(*rand_ctx, buf, len);
}

uint8_t  RANDOM8(int *rand_ctx) {
	uint8_t bit;
	mkrandom(rand_ctx, &bit, 1);
	return bit;
}

uint32_t RANDOM32(int *rand_ctx) {
	uint32_t buf;
	mkrandom(rand_ctx, (uint8_t *)&buf, 4);
	return buf;
}

uint64_t RANDOM64(int *rand_ctx) {
	uint64_t buf;
	mkrandom(rand_ctx, (uint8_t *)&buf, 8);
	return buf;
}

void RANDOM192(uint64_t r[3], int *rand_ctx) {
	mkrandom(rand_ctx, (uint8_t *)r, 24);
}

void *(*volatile rlwe_memset_volatile)(void *, int, size_t) = memset;
