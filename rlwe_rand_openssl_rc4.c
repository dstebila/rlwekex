#warning "RC4 should not be considered a cryptographically secure pseudorandom number generator due to biases in its keystream output.  See README.md and the Makefile to switch to an AES-based PRNG."

#include "rlwe_rand.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

int RAND_CHOICE_init(RAND_CTX *rand_ctx) {
	unsigned char rc4_key[16];
	RAND_bytes(rc4_key, 16);
	EVP_CIPHER_CTX_init(rand_ctx);
	return EVP_EncryptInit_ex(rand_ctx, EVP_rc4(), NULL, rc4_key, NULL);
}

void RAND_CHOICE_cleanup(RAND_CTX *rand_ctx) {
	EVP_CIPHER_CTX_cleanup(rand_ctx);
}

uint8_t  RANDOM8(RAND_CTX *rand_ctx) {
	uint8_t in = 0;
	uint8_t out;
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, (unsigned char *) &out, &outlen, (unsigned char *) &in, sizeof(uint8_t));
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return out;
}

uint32_t RANDOM32(RAND_CTX *rand_ctx) {
	uint32_t in = 0;
	uint32_t out;
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, (unsigned char *) &out, &outlen, (unsigned char *) &in, sizeof(uint32_t));
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return out;
}

uint64_t RANDOM64(RAND_CTX *rand_ctx) {
	uint64_t in = 0;
	uint64_t out;
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, (unsigned char *) &out, &outlen, (unsigned char *) &in, sizeof(uint64_t));
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	return out;
}

void RANDOM192(uint64_t r[3], RAND_CTX *rand_ctx) {
	uint64_t in[] = {0, 0, 0};
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, (unsigned char *) r, &outlen, (unsigned char *) &in, sizeof(uint64_t));
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
}

void *(*volatile rlwe_memset_volatile)(void *, int, size_t) = memset;
