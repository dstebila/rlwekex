/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>

#include "rlwe_kex.h"

#include "fft.h"
#include "rlwe.h"
#include "rlwe_a.h"

#define ITERATIONS 10000

#if defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
	unsigned long long int x;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
}
#elif defined(__x86_64__)

static __inline__ unsigned long long rdtsc(void)
{
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}
#endif

#define START_TIMER \
	gettimeofday(&timeval_start, NULL); \
	cycles_start = rdtsc();
#define END_TIMER \
	cycles_end = rdtsc(); \
	gettimeofday(&timeval_end, NULL);
#define PRINT_TIMER_AVG(op_name, it) \
	printf("%-30s %15d %15d %15" PRIu64 "\n", (op_name), (it), \
		(uint32_t) ((timeval_end.tv_usec+1000000*timeval_end.tv_sec) - (timeval_start.tv_usec+1000000*timeval_start.tv_sec)) / (it), \
		(cycles_end - cycles_start) / (it));
#define TIME_OPERATION(op, op_name, it) \
	START_TIMER \
	for (int i = 0; i < (it); i++) { \
		(op); \
	} \
	END_TIMER \
	PRINT_TIMER_AVG(op_name, it)

int main() {

	uint64_t cycles_start, cycles_end;
	struct timeval timeval_start, timeval_end;

	uint32_t s[1024];
	uint32_t e[1024];
	uint32_t b[1024];
	uint64_t k[16];
	uint64_t c[16];

	FFT_CTX ctx;
	if (!FFT_CTX_init(&ctx)) {
		printf("Memory allocation error.");
		return -1;
	}

	RAND_CTX rand_ctx;
	if (!RAND_CTX_init(&rand_ctx)) {
		printf("Randomness allocation error.");
		return -1;
	}

	printf("%-30s %15s %15s %15s\n", "Operation", "Iterations", "usec (avg)", "cycles (avg)");
	printf("------------------------------------------------------------------------------\n");

	TIME_OPERATION(sample_ct(s, &rand_ctx), "sample_ct", ITERATIONS / 50)
	TIME_OPERATION(sample(s, &rand_ctx), "sample", ITERATIONS / 50)
	TIME_OPERATION(FFT_mul(b, rlwe_a, s, &ctx), "FFT_mul", ITERATIONS / 50)
	sample(e, &rand_ctx);
	TIME_OPERATION(FFT_add(b, b, e), "FFT_add", ITERATIONS)
	TIME_OPERATION(crossround2_ct(c, b, &rand_ctx), "crossround2_ct", ITERATIONS / 10)
	TIME_OPERATION(crossround2(c, b, &rand_ctx), "crossround2", ITERATIONS / 10)
	TIME_OPERATION(round2_ct(k, b), "round2_ct", ITERATIONS / 10)
	TIME_OPERATION(round2(k, b), "round2", ITERATIONS / 10)
	TIME_OPERATION(rec_ct(k, b, c), "rec_ct", ITERATIONS)
	TIME_OPERATION(rec(k, b, c), "rec", ITERATIONS)
	TIME_OPERATION(rlwe_kex_generate_keypair(rlwe_a, s, b, &ctx), "rlwe_kex_generate_keypair", ITERATIONS / 50)
	TIME_OPERATION(rlwe_kex_compute_key_bob(b, s, c, k, &ctx), "rlwe_kex_compute_key_bob", ITERATIONS / 50)
	TIME_OPERATION(rlwe_kex_compute_key_alice(b, s, c, k, &ctx), "rlwe_kex_compute_key_alice", ITERATIONS / 50)

	FFT_CTX_clear(&ctx);
	FFT_CTX_free(&ctx);

	RAND_CTX_cleanup(&rand_ctx);

	return 0;

}
