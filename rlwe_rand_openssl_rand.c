#warning "Using OpenSSL's RAND_bytes to generate all randomness for sampling will result in very slow performance.  See rlwe.c to switch to a PRNG seeded from OpenSSL's RAND_bytes."

#include <openssl/rand.h>

#define RANDOM_VARS

#define RANDOM8 (random8())
#define RANDOM32 (random32())
#define RANDOM64 (random64())

uint8_t random8() {
	uint8_t b;
	int r = RAND_bytes((unsigned char *) &b, 1);
	if (r != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return b;
}
uint32_t random32() {
	uint32_t b;
	int r = RAND_bytes((unsigned char *) &b, 4);
	if (r != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return b;
}
uint64_t random64() {
	uint64_t b;
	int r = RAND_bytes((unsigned char *) &b, 8);
	if (r != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return b;
}
