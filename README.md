Post-quantum key exchange from the ring learning with errors problem
====================================================================

This software implements the key exchange protocol based on the ring learning with errors (ring-LWE) problem from the following paper:

* Joppe W. Bos, Craig Costello, Michael Naehrig, and Douglas Stebila. Post-quantum key exchange for the TLS protocol from the ring learning with errors problem. Cryptology ePrint Archive, Report 2014/599, August, 2014. <http://eprint.iacr.org/2014/599>

This software was initially distributed by the authors from <https://github.com/dstebila/rlwekex>

Instructions
------------
The software is plain C (C99 standard).  Compilation has been tested using gcc on Linux and clang on Mac OS X.  

To compile:

	make

To run the sample key generation program:

	./rlwe_main

To run the benchmark program:

	./rlwe_benchmark

Cryptographically secure random number generation
-------------------------------------------------
Note that the key generation and key exchange algorithms make use of a random number generator during execution.  For maximum portability, the randomness generation macros currently use C's `random()` function which is **not secure**.  Developers using this code should replace the `RANDOM_VARS`, `RANDOM8`, `RANDOM32`, `RANDOM64` macros in `rlwe.c` with a cryptographically secure pseudorandom number generator before use.  

For example, if using the OpenSSL library, the following code would use OpenSSL's PRNG.

	#include <openssl/rand.h>
	#define RANDOM_VARS
	#define RANDOM8 (random8())
	#define RANDOM32 (random32())
	#define RANDOM64 (random64())

	uint8_t random8() {
		uint8_t b;
		int r = RAND_bytes((unsigned char *) &b, 1);
		if (r != 1) {
			/* throw error */
		}
		return b;
	}
	uint32_t random32() {
		uint32_t b;
		int r = RAND_bytes((unsigned char *) &b, 4);
		if (r != 1) {
			/* throw error */
		}
		return b;
	}
	uint64_t random64() {
		uint64_t b;
		int r = RAND_bytes((unsigned char *) &b, 8);
		if (r != 1) {
			/* throw error */
		}
		return b;
	}

Note, however, that using OpenSSL's `RAND_bytes` for generating all the randomness dramatically decreases performance, and it is unnecessary to have full entropy 
for every random sample.  An alternative is to use RAND_bytes to pick a truly random seed and then use a pseudorandom generator, such as AES in counter mode,
to generate the pseudorandom values required during key generation and key exchange:

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

License
-------
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

See the file LICENSE for complete information.
