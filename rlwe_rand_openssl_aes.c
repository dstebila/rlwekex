#include "rlwe_rand.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

int RAND_CTX_init(RAND_CTX *rand_ctx) {
	unsigned char aes_key[16];
	unsigned char aes_iv[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	RAND_bytes(aes_key, 16);
	EVP_CIPHER_CTX_init(rand_ctx);
	return EVP_EncryptInit_ex(rand_ctx, EVP_aes_128_cbc(), NULL, aes_key, aes_iv);
}

void RAND_CTX_cleanup(RAND_CTX *rand_ctx) {
	EVP_CIPHER_CTX_cleanup(rand_ctx);
}

uint8_t  RANDOM8(RAND_CTX *rand_ctx) {
	return (uint8_t) RANDOM64(rand_ctx);
}

uint32_t RANDOM32(RAND_CTX *rand_ctx) {
	return (uint32_t) RANDOM64(rand_ctx);
}

uint64_t RANDOM64(RAND_CTX *rand_ctx) {
	unsigned char in[16];
	memset(in, 0, 16);
	unsigned char out[16];
	memset(out, 0, 16);
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, out, &outlen, in, 16);
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
	uint64_t *x = (uint64_t *) out;
	return *x;
}

void RANDOM192(uint64_t r[3], RAND_CTX *rand_ctx) {
	unsigned char in[24];
	memset(in, 0, 24);
	unsigned char *out = (unsigned char *) r;
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, out, &outlen, in, 24);
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
	}
}
