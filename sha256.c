#include <stdlib.h>

/* 8 bit byte and 32 bit word respectively */
typedef unsigned char BYTE;
typedef unsigned int WORD;

typedef char[65] SHA256_HASH; 

/* Holds any info needed for SHA256 */
typedef struct {

	/* Holds current chunk of 512-bit data
	 * 64 * 8 bit each = 512 bits
         */
	BYTE current_chunk[64];

	/* Holds original input length for final
	 * 64 bits in last chunk
	 */ 
	size_t original_length;

	/* Stores the length of current_chunk in bits */
	size_t current_chunk_length;

	/* Holds the states, i.e. h[0], h[1], etc. */
	WORD state[8];

} SHA256_CTX; 

/* Creates a context and returns the pointer */
SHA256_CTX* SHA256_CTX() {
	SHA256_CTX* ptr = calloc(64 * sizeof(BYTE) + 2 * sizeof(size_t) + 8 * sizeof(WORD));
	ptr->original_length = 0;
	ptr->current_chunk_length = 0;
	/* Initial hash values */ 
	ptr->state = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
}

/* Called for each 512-bit chunk of data and modifies the state */
void do_chunk_loop(SHA256_CTX* ctx) {
	ctx->current_chunk_length = 0;
}

/* Generates hash
 * Arguments: String to hash
 */
SHA256_HASH generate_hash(BYTE message[]) {
	
	SHA256_CTX* ctx = SHA256_CTX(); 
	/* ctx->original_length = strlen(message); */
		
	for (int i = 0; i < strlen(message); i++) {
		ctx->current_chunk[current_chunk_length++] = message[i]; 
		if (current_chunk_length == 64) {
			do_chunk_loop(ctx); 
		}
	} 
	
	/* Meaning the 64 bit length value can be fit into the current chunk
	 * Otherwise, pad current chunk with zeroes and create new chunk for length
	 */ 
	size_t len = ctx->current_chunk_length; 
	if (len < 56) {
		/* Append extra 1 bit */ 
		ctx->current_chunk[len++] = 0x80; 
	} else {
		while (len < 64) ctx->current_chunk[len++] = NULL;	
		do_chunk_loop(ctx); 
	}	

	while (len < 56) ctx->current_chunk[len++] = NULL;
	
	/* Append length in 64-bit big-endian
	 * unsigned long long is 64 bits 
	 */ 
	unsigned long long length_bit = strlen(message) * 8; 
	for (int i = 1; i <= 8; i++) {
		int index = 64 - i;
		int shift = (i - 1) * 8;

		ctx->current_chunk[index] = (length_bit >> shift) & 0xFF; 
	}
	/* Run chunk loop on final chunk */
	do_chunk_loop(ctx); 
		
}
