#include <openssl/rand.h>
#include <openssl/aes.h>

#define RANDOM_VARS \
	AES_KEY aes_key; \
	unsigned char aes_key_bytes[16]; \
	RAND_bytes(aes_key_bytes, 16); \
	AES_set_encrypt_key(aes_key_bytes, 128, &aes_key); \
	unsigned char aes_ivec[AES_BLOCK_SIZE]; \
	memset(aes_ivec, 0, AES_BLOCK_SIZE); \
	unsigned char aes_ecount_buf[AES_BLOCK_SIZE]; \
	memset(aes_ecount_buf, 0, AES_BLOCK_SIZE); \
	unsigned int aes_num = 0; \
	unsigned char aes_in[AES_BLOCK_SIZE]; \
	memset(aes_in, 0, AES_BLOCK_SIZE);
#define RANDOM8   ((uint8_t) randomplease(&aes_key, aes_ivec, aes_ecount_buf, &aes_num, aes_in))
#define RANDOM32 ((uint32_t) randomplease(&aes_key, aes_ivec, aes_ecount_buf, &aes_num, aes_in))
#define RANDOM64 ((uint64_t) randomplease(&aes_key, aes_ivec, aes_ecount_buf, &aes_num, aes_in))

uint64_t randomplease(AES_KEY *aes_key, unsigned char aes_ivec[AES_BLOCK_SIZE],
                      unsigned char aes_ecount_buf[AES_BLOCK_SIZE],
                      unsigned int *aes_num, unsigned char aes_in[AES_BLOCK_SIZE]) {
	uint64_t out;
	AES_ctr128_encrypt(aes_in, (unsigned char *) &out, 8, aes_key, aes_ivec, aes_ecount_buf, aes_num);
	return out;
}
