#warning "Not using a cryptographically secure pseudorandom number generator.  See rlwe.c to switch to an OpenSSL-based PRNG or define your own."

/* RANDOM_VARS is used to define any variables that are needed by your particular instantiation of the RANDOM8, RANDOM32, or RANDOM64 functions */
#define RANDOM_VARS

#define RANDOM8 ((uint8_t) random())
#define RANDOM32 ((uint32_t) (random() << 16) ^ random())
#define RANDOM64 (((uint64_t) RANDOM32 << (uint64_t) 32) | ((uint64_t) RANDOM32))
