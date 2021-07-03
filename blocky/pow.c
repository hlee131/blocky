#include <memory.h>
#include <math.h>
#include <string.h>

#include "blocky.h"
#include "../hashing/sha256.h"

/* Represents the mining difficulty */ 
const static int difficulty = 10;

int main() {
	char* invalid = "00000234234abf34dfe";
	char* valid = "0000000000278346782342";
	printf("invalid: %d\tvalid: %d", is_valid(invalid), is_valid(valid));
}

/* Validate nonce */
int is_valid(Block* blk) {

	/* Formula works for domain for all real integers excluding 0 */
	int nonce_len = blk->nonce == 0 ? 1 : floor(log10(abs(blk->nonce))); 
	char toHash[nonce_len + strlen(blk->data) + strlen(blk->previousHash) + 1];
	char* hash = generate_hash(toHash); 	

	char target[difficulty];
	memset(target, 0x30, difficulty);
	return memcmp(target, hash, difficulty) == 0; 
}

/* Mine block */  
void mine_block(Block* blk) {
	while (!is_valid(blk)) {
		/* Free old generate_hash call */
		free(blk->hash);
		blk->nonce++; 
		
		/* Formula works for domain for all real integers excluding 0 */
		int nonce_len = blk->nonce == 0 ? 1 : floor(log10(abs(blk->nonce))); 
		char toHash[nonce_len + strlen(blk->data) + strlen(blk->previousHash) + 1];
        	sprintf(toHash, "%d%s%s", blk->nonce, blk->data, blk->previousHash);
        
		blk->hash = generate_hash(toHash)
	}
}
