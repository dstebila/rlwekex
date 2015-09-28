#ifndef _RAND_H_
#define _RAND_H_

#include <stdint.h>

#define RLWE_RANDOMNESS_USE_OPENSSL_AES
// #define RLWE_RANDOMNESS_USE_OPENSSL_RC4
// #define RLWE_RANDOMNESS_USE_OPENSSL_RAND
// #define RLWE_RANDOMNESS_USE_C_RANDOM_INSECURE

#if defined(RLWE_RANDOMNESS_USE_OPENSSL_AES)
#include <openssl/evp.h>
#define RAND_CTX EVP_CIPHER_CTX
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RC4)
#include <openssl/evp.h>
#define RAND_CTX EVP_CIPHER_CTX
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RAND)
#define RAND_CTX uint8_t
#elif defined(RLWE_RANDOMNESS_USE_C_RANDOM_INSECURE)
#define RAND_CTX uint8_t
#else
#error "No randomness generation algorithm defined."
#endif

int RAND_CTX_init(RAND_CTX *rand_ctx);
void RAND_CTX_cleanup(RAND_CTX *rand_ctx);

uint8_t  RANDOM8 (RAND_CTX *rand_ctx);
uint32_t RANDOM32(RAND_CTX *rand_ctx);
uint64_t RANDOM64(RAND_CTX *rand_ctx);
void RANDOM192(uint64_t r[3], RAND_CTX *rand_ctx);

#endif
