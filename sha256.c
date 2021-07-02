#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>

#include "sha256.h"

#define HASH_FORMAT "%x%x%x%x%x%x%x%x"
#define HASH_ARG(b) b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]

#define ROTR(x, bits) ((x >> bits) | x << (32 - bits))
#define SHR(x, bits) (x >> bits) 

/* Functions from RFC6234 Section 5.1 (https://datatracker.ietf.org/doc/html/rfc6234#section-5.1) */
#define CH(x, y, z) ((x & y) ^ (~x & z)) 
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define BSIG0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define BSIG1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SSIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define SSIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

/* Holds any info needed for SHA256 */
typedef struct {

	/* Holds current chunk of 512-bit data
	 * 64 * 8 bit each = 512 bits
         */
	BYTE current_chunk[64];

	/* Stores the length of current_chunk in bits */
	size_t current_chunk_length;

	/* Holds the states, i.e. h[0], h[1], etc. */
	WORD state[8];

} SHA256_CTX; 

/* Initialize round constants */
WORD static const k[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

/* Creates a context and returns the pointer */
SHA256_CTX* make_context() {
	SHA256_CTX* ptr = calloc(1, 64 * sizeof(BYTE) + 1 * sizeof(size_t) + 8 * sizeof(WORD));
	ptr->current_chunk_length = 0;
	/* Initial hash values */ 
	WORD initial[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
	memcpy(ptr->state, initial, sizeof(WORD) * 8);
	return ptr; 
}

/* Called for each 512-bit chunk of data and modifies the state */
void do_chunk_loop(SHA256_CTX* ctx) {
	
	/* Message schedule */
	WORD w[64];
	
	/* Break into 32 bit words */
	for (int i = 0; i < 16; i++) {
		int ii = i * 4;
		w[i] =  (ctx->current_chunk[ii] << 24) + (ctx->current_chunk[ii + 1] << 16) + (ctx->current_chunk[ii + 2] << 8) + ctx->current_chunk[ii + 3];
	}
	
	/* Finish message schedule */
	for (int i = 16; i < 64; i++) {
		WORD t1 = SSIG0(w[i-15]);
		WORD t2 = SSIG1(w[i-2]);
		w[i] = w[i-16] + t1 + w[i-7] + t2; 
	}

	/* Initialize working variables */
	WORD a = ctx->state[0];
	WORD b = ctx->state[1];
	WORD c = ctx->state[2];
	WORD d = ctx->state[3];
	WORD e = ctx->state[4];
	WORD f = ctx->state[5];
	WORD g = ctx->state[6];
	WORD h = ctx->state[7];
	
	
	/* Main compression loop */
	for (int i = 0; i < 64; i++) {
		WORD t1 = h + BSIG1(e) + CH(e, f, g) + k[i] + w[i];
		WORD t2 = BSIG0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2; 
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
	
	/* Reset values for next chunk */ 
	ctx->current_chunk_length = 0;
	memset(ctx->current_chunk, 0x00, 64);
}

/* Generates hash
 * Arguments: String to hash
 */
char* generate_hash(BYTE message[]) {
	
	SHA256_CTX* ctx = make_context(); 
	char* final_digest = malloc(65); 
		
	for (int i = 0; i < strlen(message); i++) {
		ctx->current_chunk[ctx->current_chunk_length++] = message[i]; 
		if (ctx->current_chunk_length == 64) {
			do_chunk_loop(ctx); 
		}
	} 
	
	/* Meaning the 64 bit length value can be fit into the current chunk
	 * Otherwise, pad current chunk with zeroes and create new chunk for length
	 */ 
	size_t len = ctx->current_chunk_length; 
	
	/* Append extra 1 bit */
	ctx->current_chunk[len++] = 0x80;
	
	if (len >= 56)  {
		while (len < 64) ctx->current_chunk[len++] = 0x00;	
		do_chunk_loop(ctx); 
	}	

	while (len < 56) ctx->current_chunk[len++] = 0x00;
	
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
	sprintf(final_digest, HASH_FORMAT, HASH_ARG(ctx->state));
	return final_digest; 
		
}
