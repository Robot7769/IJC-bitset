LC_ALL = cs_CZ.utf8
#$(LC_ALL)
CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra -O2 #-m32
LDLIBS = -lm #-m32

.PHONY: all zip clean run

all: primes primes-i steg-decode steg-encode
# souboty .o (závislosti)
error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

bitset.o: bitset.c bitset.h
	$(CC) $(CFLAGS) -c bitset.c -o bitset.o

bitset-i.o: bitset.c bitset.h
	$(CC) -DUSE_INLINE $(CFLAGS) -c bitset.c -o bitset-i.o

eratosthenes.o: eratosthenes.c eratosthenes.h bitset.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

eratosthenes-i.o: eratosthenes.c eratosthenes.h bitset.h
	$(CC) -DUSE_INLINE $(CFLAGS) -c eratosthenes.c -o eratosthenes-i.o

primes.o: primes.c bitset.h eratosthenes.h
	$(CC) $(CFLAGS) -c primes.c -o primes.o

primes-i.o: primes.c bitset.h eratosthenes.h
	$(CC) -DUSE_INLINE $(CFLAGS) -c primes.c -o primes-i.o

ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o

steg-decode.o: steg-decode.c error.h bitset.h eratosthenes.h ppm.h 
	$(CC) $(CFLAGS) -fstack-protector -c steg-decode.c -o steg-decode.o -fstack-protector

steg-encode.o: steg-encode.c error.h bitset.h eratosthenes.h ppm.h 
	$(CC) $(CFLAGS) -fstack-protector -c steg-encode.c -o steg-encode.o -fstack-protector


#spustitelné binárky
primes: primes.o error.o bitset.o eratosthenes.o 
	$(CC) primes.o error.o bitset.o eratosthenes.o $(LDLIBS) -o primes

primes-i: primes-i.o error.o bitset-i.o eratosthenes-i.o 
	$(CC) primes-i.o error.o bitset-i.o eratosthenes-i.o $(CFLAGS) -o primes-i

steg-decode: steg-decode.o error.o bitset.o eratosthenes.o ppm.o
	$(CC) steg-decode.o error.o bitset.o eratosthenes.o ppm.o $(CFLAGS) -o steg-decode

steg-encode: steg-encode.o error.o bitset.o eratosthenes.o ppm.o
	$(CC) steg-encode.o error.o bitset.o eratosthenes.o ppm.o $(CFLAGS) -o steg-encode 


#pomocné příkazy
zip:
	zip xskrab12.zip *.c *.h Makefile


clean: 
	rm -f *.o primes primes-i steg-decode steg-encode


run: primes primes-i
	./primes
	./primes-i