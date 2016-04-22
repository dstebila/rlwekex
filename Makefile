CC=gcc -O3 -Wall -Wextra -std=c99 -Wno-deprecated-declarations

CPPFLAGS=
LDFLAGS=-lcrypto

# On Mac OS X, the system OpenSSL is too old.  
# Install your own more recent version and point to it.  
# If you have installed OpenSSL via brew, you can use the following two lines.

CPPFLAGS=-I/usr/local/opt/openssl/include #-DCONSTANT_TIME for constant time
LDFLAGS=-L/usr/local/opt/openssl/lib -lcrypto

all:
	$(CC) $(CPPFLAGS) -Wno-unused-function -c fft.c
	$(CC) $(CPPFLAGS) -Wno-unused-function -c rlwe.c
	$(CC) $(CPPFLAGS) -Wno-unused-function -c rlwe_kex.c
	$(CC) $(CPPFLAGS) -Wno-unused-function -Wno-unused-parameter -c rlwe_rand.c
	$(CC) $(CPPFLAGS) -o rlwe_main -lcrypto rlwe_main.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)
	$(CC) $(CPPFLAGS) -o rlwe_benchmark -lcrypto rlwe_benchmark.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)
	$(CC) $(CPPFLAGS) -o rlwe_test -lcrypto rlwe_test.c fft.o rlwe.o rlwe_kex.o rlwe_rand.o $(LDFLAGS)

clean:
	rm fft.o rlwe.o rlwe_kex.o rlwe_rand.o rlwe_main rlwe_benchmark rlwe_test

test:
	./rlwe_test

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none *.c *.h

