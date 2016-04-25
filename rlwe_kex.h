/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#ifndef _RLWE_KEX_H_
#define _RLWE_KEX_H_

#include <stdint.h>

#include "fft.h"

/* Generate keypair for RLWE KEX
 * - input: parameters: a
 * - output: private key s, public key b
 * - return: 1 on success, 0 on failure
 */
int rlwe_kex_generate_keypair(const uint32_t *a, uint32_t s[1024], uint32_t b[1024], FFT_CTX *ctx);

/* Alice's shared key computation for RLWE KEX
 * - input: Bob's public key b, Alice's private key s, reconciliation data c
 * - output: shared secret k
 * - return: 1 on success, 0 on failure
 */
int rlwe_kex_compute_key_alice(const uint32_t b[1024], const uint32_t s[1024], const uint64_t c[16], uint64_t k[16], FFT_CTX *ctx);

/* Bob's shared key computation for RLWE KEX
 * - input: Alice's public key b, Bob's private key s
 * - output: reconciliation data c, shared secret k
 * - return: 1 on success, 0 on failure
 */
int rlwe_kex_compute_key_bob(const uint32_t b[1024], const uint32_t s[1024], uint64_t c[16], uint64_t k[16], FFT_CTX *ctx);

#endif /* _RLWE_KEX_H_ */
