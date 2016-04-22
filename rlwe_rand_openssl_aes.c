#include "rlwe_rand.h"

#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int RAND_CHOICE_init(RAND_CTX *rand_ctx) {
	int ret = 1;
	unsigned char aes_key[16];
	unsigned char aes_iv[16];
	ret &= RAND_bytes(aes_key, 16);
	ret &= RAND_bytes(aes_iv, 16);
	EVP_CIPHER_CTX_init(rand_ctx);
	ret &= EVP_EncryptInit_ex(rand_ctx, EVP_aes_256_ctr(), NULL, aes_key, aes_iv);
	OPENSSL_cleanse(aes_key, sizeof(aes_key));
	OPENSSL_cleanse(aes_iv, sizeof(aes_iv));
	return ret;
}

void RAND_CHOICE_cleanup(RAND_CTX *rand_ctx) {
	EVP_CIPHER_CTX_cleanup(rand_ctx);
}

uint8_t  RANDOM8(RAND_CTX *rand_ctx) {
	return (uint8_t) RANDOM64(rand_ctx);
}

uint32_t RANDOM32(RAND_CTX *rand_ctx) {
	return (uint32_t) RANDOM64(rand_ctx);
}

uint64_t RANDOM64(RAND_CTX *rand_ctx) {
	unsigned char in[16] = {0};
	unsigned char out[16] = {0};
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, out, &outlen, in, 16);
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
		abort();
	}
	uint64_t *x = (uint64_t *) out;
	return *x;
}

void RANDOM192(uint64_t r[3], RAND_CTX *rand_ctx) {
	unsigned char in[24] = {0};
	unsigned char *out = (unsigned char *) r;
	int outlen;
	int ret = EVP_EncryptUpdate(rand_ctx, out, &outlen, in, 24);
	if (ret != 1) {
		fprintf(stderr, "Randomness generation failed.\n");
		abort();
	}
}

void *(*volatile rlwe_memset_volatile)(void *, int, size_t) = memset;
