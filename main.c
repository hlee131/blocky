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

/* Blockchain datatype */ 
typedef struct blockchain {
	Block **last_block_ptr;	
	Block *blocks[];
} BlockChain;

int main(int argc, char *argv[]) {
	char input[100];
	puts("What would you like to SHA-256 pad? ");
	fgets(input, 100, stdin);
	generate_hash(input);
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
	return make_block("Genesis Block", NULL);
}

BlockChain make_blockchain() {
	Block genesis = make_genesis();
	BlockChain bc = {
		&(blocks[0]),
		{ &genesis }
	}
	return bc;
}

void add_block(Block* new_block, BlockChain* chain) {
	chain = realloc(chain, sizeof(*chain) + sizeof(new_block));
	chain->last_block_ptr++;
	*(chain->last_block_ptr) = new_block; 
}


