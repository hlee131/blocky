#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "blocky.h"
#include "pow.h"
#include "../hashing/sha256.h"

/* Represents the mining difficulty (higher requires more work)
 * Value must be 1 or greater 
 */ 
const static int difficulty = 1;
/*
int main() {
	
	Block blk* = malloc(sizeof(Block)); 
	blk->data = malloc(8);
	strcpy(blk->data, "Genesis"); 
	blk->previousHash = malloc(65);
	strcpy(blk->previousHash, "");
	blk->nonce = 

}*/

/* Validate nonce */
int is_valid(Block* blk) {
	/* Formula works for domain for all real integers excluding 0 */
	int nonce_len = blk->nonce == 0 ? 1 : floor(log10(abs(blk->nonce))); 
	char toHash[nonce_len + strlen(blk->data) + strlen(blk->previousHash) + 1];
	sprintf(toHash, "%d%s%s", blk->nonce, blk->data, blk->previousHash);
	char* hash = generate_hash(toHash); 	

	char target[difficulty];
	memset(target, 0x30, difficulty);
	int validity = memcmp(target, hash, difficulty) == 0; 
	free(hash);
	return validity;
}

/* Mine block */  
void mine_block(Block* blk) {
	while (1) {
		/* Free old generate_hash call */
		free(blk->hash);
		
		/* Formula works for domain for all real integers excluding 0 */
		int nonce_len = blk->nonce == 0 ? 1 : floor(log10(abs(blk->nonce))); 
		char toHash[nonce_len + strlen(blk->data) + strlen(blk->previousHash) + 1];
        	sprintf(toHash, "%d%s%s", blk->nonce, blk->data, blk->previousHash);
        	blk->hash = generate_hash(toHash);
		printf("%s -> %s\n", toHash, blk->hash);	
		
		if (is_valid(blk)) break;
		else blk->nonce++; 
	}
}
