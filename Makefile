CC=gcc -O3 -Wall -Wextra -std=c99 -Wno-deprecated-declarations

all:
	$(CC) -Wno-unused-function -c fft.c
	$(CC) -Wno-unused-function -c rlwe.c
	$(CC) -Wno-unused-function -c rlwe_kex.c
	$(CC) -Wno-unused-function -Wno-unused-parameter -c rlwe_rand.c
	$(CC) -o rlwe_main -lcrypto rlwe_main.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o
	$(CC) -o rlwe_benchmark -lcrypto rlwe_benchmark.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o

clean:
	rm fft.o rlwe.o rlwe_kex.o rlwe_rand.o rlwe_main rlwe_benchmark

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none *.c *.h

