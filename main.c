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
	Block** chain;
	size_t length; 
} Blockchain;

Blockchain* make_blockchain();
Block* make_block(const char* data, const char* previousHash);
void add_block(Block* new_block, Blockchain** chain);
void display_blockchain(Blockchain* chain);
char* get_last_hash(Blockchain* bc_ptr);

int main(int argc, char *argv[]) {
	/* TODO: Genesis block being overwritten */
	Blockchain* b = make_blockchain();
	display_blockchain(b);
	add_block(make_block("Genesis", ""), &b);
	add_block(make_block("hello world", get_last_hash(b)), &b);
	add_block(make_block("foo bar", get_last_hash(b)), &b);
	add_block(make_block("fizz buzz", get_last_hash(b)), &b);
	add_block(make_block("lorem ipsum", get_last_hash(b)), &b);
	add_block(make_block("something", get_last_hash(b)), &b);
	add_block(make_block("something", get_last_hash(b)), &b);
	display_blockchain(b);
	return 0; 
}

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
	
	Blockchain* bc = malloc(sizeof(size_t));
	bc->chain = malloc(0);
	bc->length = 0;
	return bc;
}

void destroy_blockchain(Blockchain* chain) {
	free(chain);
	/* free blocks too */ 
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

