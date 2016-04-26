Post-quantum key exchange from the ring learning with errors problem
====================================================================

This software implements the key exchange protocol based on the ring learning with errors (ring-LWE) problem from the following paper:

* Joppe W. Bos, Craig Costello, Michael Naehrig, Douglas Stebila. Post-quantum key exchange for the TLS protocol from the ring learning with errors problem. In *Proc. IEEE Symposium on Security and Privacy (S&P) 2015*, pp. 553-570. IEEE, May 2015. DOI:[10.1109/SP.2015.40](http://dx.doi.org/10.1109/SP.2015.40), Eprint <http://eprint.iacr.org/2014/599>.

This software was initially distributed by the authors from <https://github.com/dstebila/rlwekex>

Instructions
------------
The software is plain C (C99 standard).  Compilation has been tested using gcc on Linux and clang on Mac OS X.

To compile:

	make

Note that compilation on Mac OS X with the downloaded ZIP will fail, because the library uses OpenSSL for randomness generation, and Mac OS X has an old version of OpenSSL in a peculiar location.  Either change the random number generator to the insecure one (see the instructions below) or install and use a newer version of OpenSSL (see the comments at the top of the Makefile).

To run the sample key generation program:

	./rlwe_main

To run the benchmark program:

	./rlwe_benchmark

Cryptographically secure random number generation
-------------------------------------------------
Note that the key generation and key exchange algorithms make use of a random number generator during execution.  The sampling code is configured by default to use OpenSSL's PRNG to generate a seed and expand it using AES.  Several other options are available; see `Makefile`.  C's `random()` can be used for testing purposes by defining the macro `RLWE_RANDOMNESS_USE_INSECURE_LIBC`, but this is **not secure**.  Developers can also define the `RANDOM_VARS`, `RANDOM8`, `RANDOM32`, `RANDOM64` macros with a cryptographically secure pseudorandom number generator of their own choosing.

License
-------
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

See the file LICENSE for complete information.
