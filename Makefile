CC=gcc

all:
	$(CC) demo.c blocky/blocky.c blocky/pow.c hashing/sha256.c -lm 

debug: 
	gcc -g demo.c blocky/blocky.c blocky/pow.c hashing/sha256.c -lm -o debug

clean:
	rm -f a.out debug

