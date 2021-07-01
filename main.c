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
Block* make_block(char* data, char* previousHash);
void add_block(Block* new_block, Blockchain** chain);
void display_blockchain(Blockchain* chain);

int main(int argc, char *argv[]) {
	/*
	char input[100];
	puts("What would you like to SHA-256 pad? ");
	fgets(input, 100, stdin);
	generate_hash(input);
	*/
	Blockchain* my_blockchain = make_blockchain();
	display_blockchain(my_blockchain);
	Block* new_block = make_block("sdf", "sdf");
	display_blockchain(my_blockchain);
	add_block(new_block, &my_blockchain);
	display_blockchain(my_blockchain);
	return 0; 
}

Block* make_block(char* data, char* previousHash) {
	/* Fill out some variables 
	 * SHA-256 hash is 64 characters + null terminator long
   	 * 2 x SHA-256 hashes, one previous and one current
	 */
	size_t hash_size = 65 * sizeof(char);
	size_t data_size = ( 1 + strlen(data)) * sizeof(data);
	Block* new_block = malloc(2 * hash_size + data_size);
	new_block->data = data;
	new_block->previousHash = previousHash;
	/* Calculate new hash and add to block */
	/* char toHash[] = "hello";
	strcat(toHash, previousHash);
	new_block->hash = generate_hash(toHash);*/
	return new_block;
}

Block* make_genesis() {
	return make_block("Gen", "");
}

Blockchain* make_blockchain() {
	
	Block **blockchain = malloc(8);
	blockchain[0] = make_genesis();  
	Blockchain* bc = malloc(8 + sizeof(size_t));
	bc->chain = blockchain;
	bc->length = 1;
	return bc;
}

void destroy_blockchain(Blockchain* chain) {
	free(chain);
	/* free blocks too */ 
}

void add_block(Block* new_block, Blockchain** chain) {
	*chain = (Blockchain*) realloc(*chain, ++((*chain)->length) * 8 + sizeof(size_t));
	(*chain)->chain[(*chain)->length - 1] = new_block; 
	printf("%p", *chain);	
}

void display_blockchain(Blockchain *bc_ptr) {
	printf("\nReceived: %p \n", bc_ptr);
	for (int i = 0; i < bc_ptr->length; i++) {
		printf("Block Number %d\n", i);
		printf("\tHash: %s\n", bc_ptr->chain[i]->hash);
		printf("\tData: %s\n", bc_ptr->chain[i]->data);
		printf("\tPrevious Hash: %s\n\n", bc_ptr->chain[i]->previousHash);	
	}
}

