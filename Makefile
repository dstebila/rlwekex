CC=clang

CCFLAGS=-O3 -Wall -Wextra -std=c99 -DRLWE_RANDOMNESS_USE_DEV_URANDOM
LDFLAGS=

all:
	$(CC) $(CCFLAGS) -Wno-unused-function -c fft.c
	$(CC) $(CCFLAGS) -Wno-unused-function -c rlwe.c
	$(CC) $(CCFLAGS) -Wno-unused-function -c rlwe_kex.c
	$(CC) $(CCFLAGS) -Wno-unused-function -c rlwe_rand.c
	$(CC) $(CCFLAGS) -o rlwe_main -lcrypto rlwe_main.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)
	$(CC) $(CCFLAGS) -o rlwe_benchmark -lcrypto rlwe_benchmark.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)

clean:
	rm fft.o rlwe.o rlwe_kex.o rlwe_rand.o rlwe_main rlwe_benchmark

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none *.c *.h

