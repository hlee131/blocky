#include <stdio.h>

#include "sha256.h"

/* Block datatype */
typedef struct block {
	char *hash;
	char *data;
	char *previousHash;
} Block; 

/* Blockchain datatype 
typedef struct chain {
	Block chain[];
} Blockchain;
*/

int main(int argc, char *argv[]) {
	char input[100];
	puts("What would you like to SHA-256 pad? ");
	fgets(input, 100, stdin);
	generate_hash(input);
	return 0; 
}

