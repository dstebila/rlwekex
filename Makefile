CC=gcc


# Choose your PRNG 
# RLWE_RANDOMNESS_USE_OPENSSL_AES
# RLWE_RANDOMNESS_USE_OPENSSL_RC4
# RLWE_RANDOMNESS_USE_OPENSSL_RAND
# RLWE_RANDOMNESS_USE_DEV_URANDOM
# RLWE_RANDOMNESS_USE_INSECURE_LIBC
PRNG=RLWE_RANDOMNESS_USE_OPENSSL_AES
PRNG_CCFLAGS=
# LDFLAGS needs to have -lcrypto if you are using an OpenSSL-based PRNG
PRNG_LDFLAGS=-lcrypto

# On Mac OS X, the system OpenSSL is too old.  
# Install your own more recent version and point to it.  
# If you have installed OpenSSL via brew, you can use the following two lines.
PRNG_CCFLAGS=-I/usr/local/opt/openssl/include
PRNG_LDFLAGS=-L/usr/local/opt/openssl/lib -lcrypto

CCFLAGS=-O3 -Wall -Wextra -std=c99 -Wno-deprecated-declarations -D$(PRNG) $(PRNG_CCFLAGS) #-DCONSTANT_TIME
LDFLAGS=$(PRNG_LDFLAGS)

all:
	$(CC) $(CCFLAGS) -Wno-unused-function -c fft.c
	$(CC) $(CCFLAGS) -Wno-unused-function -c rlwe.c
	$(CC) $(CCFLAGS) -Wno-unused-function -c rlwe_kex.c
	$(CC) $(CCFLAGS) -Wno-unused-function -Wno-unused-parameter -c rlwe_rand.c
	$(CC) $(CCFLAGS) -o rlwe_main rlwe_main.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)
	$(CC) $(CCFLAGS) -o rlwe_benchmark rlwe_benchmark.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)
	$(CC) $(CCFLAGS) -Wno-unused-function -o rlwe_test rlwe_test.c fft.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)

clean:
	rm fft.o rlwe.o rlwe_kex.o rlwe_rand.o rlwe_main rlwe_benchmark rlwe_test

test:
	./rlwe_test

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none *.c *.h

