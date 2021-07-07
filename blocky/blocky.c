#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "../hashing/sha256.h"
#include "blocky.h"
#include "pow.h"

char* get_last_hash(Blockchain* bc_ptr) {
	return bc_ptr->chain[bc_ptr->length - 1]->hash;
}

Block* make_block(const char* data, const char* previousHash) {
	/* Fill out some variables 
	 * SHA-256 hash is 64 characters + null terminator long
   	 * 2 x SHA-256 hashes, one previous and one current
	 */
	Block* new_block = malloc(sizeof(Block));
	
	new_block->nonce = 0; 
	new_block->data = malloc((strlen(data) + 2) * sizeof(char));
	new_block->hash = malloc(0);
	new_block->previousHash = malloc(65 * sizeof(char));
	strcpy(new_block->data, data);
	strcpy(new_block->previousHash, previousHash); 
	
	/* Mine block and do proof of work */	
	mine_block(new_block); 		

	return new_block;
}

Blockchain* make_blockchain() {
	
	Blockchain* bc = malloc(sizeof(size_t) + 8);
	bc->chain = malloc(0);
	bc->length = 0;
	add_block(make_block("Genesis", ""), bc);
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

int add_block(Block* new_block, Blockchain* chain) {
	/* Verify proof of work before adding to chain */
	if (!is_valid(new_block)) {
		/* Return 0 for PoW failed */
		return 0; 	
	} else {		
		chain->chain = realloc(chain->chain, ++(chain->length) * 8 + sizeof(size_t));
		chain->chain[chain->length - 1] = new_block;
	
		/* Return 1 for block added */
		return 1;
	}
}

void display_blockchain(Blockchain *bc_ptr) {
	for (int i = 0; i < bc_ptr->length; i++) {
		printf("Block Number %d\n", i);
		printf("\tHash: %s\n", bc_ptr->chain[i]->hash);
		printf("\tData: %s\n", bc_ptr->chain[i]->data);
		printf("\tPrevious Hash: %s\n", bc_ptr->chain[i]->previousHash);
		printf("\tNonce: %d\n\n", bc_ptr->chain[i]->nonce);
	}
}

void save_blockchain(Blockchain *bc_ptr) {
	DIR* dir = opendir("saved"); 
	if (dir) { 
		FILE* fp = fopen("./saved/demo.csv", "w+");
		fprintf(fp, "%d\n", bc_ptr->length); 
		for (int i = 0; i < bc_ptr->length; i++) {
			Block *blk = bc_ptr->chain[i];
			fprintf(fp, "%s, %s, %d\n", blk->hash, blk->data, blk->nonce);
		}
		fprintf(fp, "\n"); 
		fclose(fp); 
	} else {
		puts("Please create a directory called saved in the current working directory first."); 
	}
}

