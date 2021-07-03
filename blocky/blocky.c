#include <stdio.h>
#include <string.h>

#include "../hashing/sha256.h"
#include "blocky.h"

char* get_last_hash(Blockchain* bc_ptr) {
	return bc_ptr->chain[bc_ptr->length - 1]->hash;
}

Block* make_block(const char* data, const char* previousHash) {
	/* Fill out some variables 
	 * SHA-256 hash is 64 characters + null terminator long
   	 * 2 x SHA-256 hashes, one previous and one current
	 */
	Block* new_block = malloc(sizeof(Block));
	
	new_block->data = malloc((strlen(data) + 1) * sizeof(char));
	new_block->previousHash = malloc(65 * sizeof(char));
	strcpy(new_block->data, data);
	strcpy(new_block->previousHash, previousHash); 
	
	char toHash[strlen(data) + strlen(previousHash) + 1];
	strcpy(toHash, data);
	strcat(toHash, previousHash);
	new_block->hash = generate_hash(toHash);

	return new_block;
}

Blockchain* make_blockchain() {
	
	Blockchain* bc = malloc(sizeof(size_t) + 8);
	bc->chain = malloc(0);
	bc->length = 0;
	add_block(make_block("Genesis", ""), &bc);
	return bc;
}

void destroy_blockchain(Blockchain* chain) {
	/* Free values inside blocks and the blocks */
	for (int i = 0; i < chain->length; i++) {
		free(chain->chain[i]->hash);
		free(chain->chain[i]->previousHash);
		free(chain->chain[i]->data);	
		free(chain->chain[i]);
	}
	free(chain->chain);	
	free(chain);
}

void add_block(Block* new_block, Blockchain** chain) {
	*chain = (Blockchain*) realloc(*chain, ++((*chain)->length) * 8 + sizeof(size_t));
	(*chain)->chain[(*chain)->length - 1] = new_block;
}

void display_blockchain(Blockchain *bc_ptr) {
	for (int i = 0; i < bc_ptr->length; i++) {
		printf("Block Number %d\n", i);
		printf("\tHash: %s\n", bc_ptr->chain[i]->hash);
		printf("\tData: %s\n", bc_ptr->chain[i]->data);
		printf("\tPrevious Hash: %s\n\n", bc_ptr->chain[i]->previousHash);	
	}
}

