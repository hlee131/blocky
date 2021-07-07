#include <stdlib.h>

/* Block datatype */
typedef struct block {
        char *hash;
        char *data;
        char *previousHash;
	unsigned int nonce; 
} Block; 

/* Blockchain datatype */
typedef struct blockchain {
        Block** chain;
        size_t length; 
} Blockchain;

Blockchain* make_blockchain();
Block* make_block(const char* data, const char* previousHash);
int add_block(Block* new_block, Blockchain* chain);
void display_blockchain(Blockchain* chain);
char* get_last_hash(Blockchain* bc_ptr);
void destroy_blockchain(Blockchain* chain);
void save_blockchain(Blockchain *bc_ptr);
