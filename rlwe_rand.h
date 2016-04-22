#ifndef _RAND_H_
#define _RAND_H_

#include <stdint.h>
#include <stddef.h>

#define RLWE_RANDOMNESS_USE_DEV_RANDOM
// #define RLWE_RANDOMNESS_USE_OPENSSL_AES
// #define RLWE_RANDOMNESS_USE_OPENSSL_RC4
// #define RLWE_RANDOMNESS_USE_OPENSSL_RAND
// #define RLWE_RANDOMNESS_USE_C_RANDOM_INSECURE

#if defined(RLWE_RANDOMNESS_USE_OPENSSL_AES)
#include <openssl/evp.h>
#define RAND_CTX EVP_CIPHER_CTX
#define RAND_CHOICE openssl_aes
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RC4)
#include <openssl/evp.h>
#define RAND_CTX EVP_CIPHER_CTX
#define RAND_CHOICE openssl_rc4
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RAND)
#define RAND_CTX uint8_t
#define RAND_CHOICE openssl_rand
#elif defined(RLWE_RANDOMNESS_USE_DEV_URANDOM)
#define RAND_CTX int
#define RAND_CHOICE dev_urandom
#elif defined(RLWE_RANDOMNESS_USE_INSECURE_LIBC)
#define RAND_CTX int
#define RAND_CHOICE insecure_libc
#else
#error "No randomness generation algorithm defined."
#endif

int RAND_CHOICE_init(RAND_CTX *rand_ctx);
void RAND_CHOICE_cleanup(RAND_CTX *rand_ctx);

uint8_t  RANDOM8 (RAND_CTX *rand_ctx);
uint32_t RANDOM32(RAND_CTX *rand_ctx);
uint64_t RANDOM64(RAND_CTX *rand_ctx);
void RANDOM192(uint64_t r[3], RAND_CTX *rand_ctx);

extern void *(*volatile rlwe_memset_volatile)(void *, int, size_t);

#endif
