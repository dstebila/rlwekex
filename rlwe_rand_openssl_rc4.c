#warning "RC4 should not be considered a cryptographically secure pseudorandom number generator due to biases in its keystream output.  See rlwe.c to switch to an AES-based PRNG."

#include <openssl/rand.h>
#include <openssl/rc4.h>

#define RANDOM_VARS \
	RC4_KEY rc4_key; \
	unsigned char rc4_key_bytes[16]; \
	RAND_bytes(rc4_key_bytes, 16); \
	RC4_set_key(&rc4_key, 16, rc4_key_bytes);

#define RANDOM8   ((uint8_t) randomplease(&rc4_key))
#define RANDOM32 ((uint32_t) randomplease(&rc4_key))
#define RANDOM64 ((uint64_t) randomplease(&rc4_key))

uint64_t randomplease(RC4_KEY *rc4_key) {
	uint64_t b;
	uint64_t z = (uint64_t) 0;
	RC4(rc4_key, 8, (unsigned char *) &z, (unsigned char *) &b);
	return b;
}
