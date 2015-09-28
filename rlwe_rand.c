#include "rlwe_rand.h"

#if defined(RLWE_RANDOMNESS_USE_OPENSSL_AES)
#include "rlwe_rand_openssl_aes.c"
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RC4)
#include "rlwe_rand_openssl_rc4.c"
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RAND)
#include "rlwe_rand_openssl_rand.c"
#elif defined(RLWE_RANDOMNESS_USE_C_RANDOM_INSECURE)
#include "rlwe_rand_c.c"
#else
#error "No randomness generation algorithm defined."
#endif
