#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sha256.h"

/* Block datatype */
typedef struct block {
	char *hash;
	char *data;
	char *previousHash;
} Block; 

Block** make_blockchain();

int main(int argc, char *argv[]) {
	/*
	char input[100];
	puts("What would you like to SHA-256 pad? ");
	fgets(input, 100, stdin);
	generate_hash(input);
	*/
	Block** my_blockchain = make_blockchain();
	return 0; 
}

Block make_block(char* data, char* previousHash) {
	/* Fill out some variables */
	Block new_block;
	new_block.data = data;
	new_block.previousHash = previousHash;
	
	/* Calculate new hash and add to block */
	char toHash[strlen(data) + strlen(previousHash)];
	strcpy(toHash, data);
	strcat(toHash, previousHash);
	new_block.hash = generate_hash(toHash);
	
	return new_block;
}

Block make_genesis() {
	return make_block("Genesis Block", "");
}

/* A Blockchain is simply an array of Block pointers
 * The code opts for pointers instead of Blocks themselves 
 * as it is easier to determine the length of the blockchain 
 */
Block** make_blockchain() {
	Block genesis = make_genesis();
	
	/* pointer is eight bytes */
	Block **bc = malloc(8); 
	bc[0] = &genesis; 

	return bc;
}

void destroy_blockchain(Block *blockchain[]) {
	free(blockchain); 
}

void add_block(Block* new_block, Block** chain) {
	size_t chain_length = sizeof(chain) / 8; 
	chain = realloc(chain, sizeof(chain) + 8);
	chain[chain_length] = new_block;
}


