#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/sha.h>

#define ROUNDS 10
#define BLOCK_SIZE 8
#define HALF_BLOCK (BLOCK_SIZE / 2)

typedef enum {false=0, true} bool;

void feistel(unsigned char *block, int block_length, unsigned char *dest);
void sha1sum2text(unsigned char *dest, unsigned char *sum);
unsigned char* f(unsigned char *key);
void fill_keys(unsigned char *initial, int length);
void print_block(unsigned char *block);

unsigned char keys[ROUNDS/2][SHA_DIGEST_LENGTH];

int main(int argc, char **argv) {
	unsigned char c, *password = calloc(256, sizeof(char)),
		*block = calloc(BLOCK_SIZE, sizeof(unsigned char)),
		*crypted = calloc(BLOCK_SIZE, sizeof(unsigned char));
	int i=0;
	
	if (isatty(fileno(stdin)))
		printf("Password: ");
	scanf("%s", password);

	fill_keys(password, strlen((char*)password));
	
	// Generate keys
	unsigned char *text = calloc(SHA_DIGEST_LENGTH, sizeof(char));
	for (i=0; i<ROUNDS/2; ++i) {
		sha1sum2text(text, keys[i]);
	}
	
	// Do the feistel thing
	while ((c = getchar()) != (unsigned char)EOF) {
		block[i++] = c;
		if (i % BLOCK_SIZE == 0) {
			feistel(block, BLOCK_SIZE, crypted);
			print_block(crypted);
			i = 0;
			memset(block, 0, BLOCK_SIZE*sizeof(unsigned char));
		}
	}
	if (strlen((char*)block) > 0) {
		feistel (block, strlen((char*)block), crypted);	
		print_block(crypted);
	}
	printf("\n");
	
	return 0;
}

void feistel(unsigned char *block, int block_length, unsigned char *dest) {
	unsigned *b = calloc(BLOCK_SIZE, sizeof(char)), r_old[HALF_BLOCK],
		r_new[HALF_BLOCK], l_old[HALF_BLOCK], l_new[HALF_BLOCK], k1[HALF_BLOCK],
		k2[HALF_BLOCK];
	int i, j;

	// Add padding if block is too short
	if (block_length < BLOCK_SIZE)
		memset(b, (BLOCK_SIZE - block_length), sizeof(char) * BLOCK_SIZE);
	memcpy(b, block, block_length);

	memcpy(&l_old, block, HALF_BLOCK);
	memcpy(&r_old, &block[HALF_BLOCK], HALF_BLOCK);
	for (i=0; i<(ROUNDS/2); ++i) {
		memcpy(&k1, keys[i], HALF_BLOCK * sizeof(char));
		memcpy(&k2, &keys[i][HALF_BLOCK], HALF_BLOCK * sizeof(char));
		
		// Round 1
		memcpy(&l_new, r_old, HALF_BLOCK);
		for (j=0; j<HALF_BLOCK; ++j) {
			r_new[j] = l_old[j] ^ k1[j];
		}
		memcpy(&l_old, l_new, HALF_BLOCK);
		memcpy(&r_old, r_new, HALF_BLOCK);
		
		// Round 2
		memcpy(&l_new, r_old, HALF_BLOCK);
		for (j=0; j<HALF_BLOCK; ++j) {
			r_new[j] = l_old[j] ^ k2[j];
		}
		memcpy(&l_old, l_new, HALF_BLOCK);
		memcpy(&r_old, r_new, HALF_BLOCK);
	}
	memcpy(dest, l_new, HALF_BLOCK);
	memcpy(&dest[HALF_BLOCK], r_new, HALF_BLOCK);
}

unsigned char* f(unsigned char *key) {
	unsigned char *key_text = calloc(SHA_DIGEST_LENGTH, sizeof(unsigned char));
	sha1sum2text(key_text, key);
	return SHA1(key_text, SHA_DIGEST_LENGTH, NULL);
}

void fill_keys(unsigned char *initial, int length) {
	unsigned char *key = calloc(SHA_DIGEST_LENGTH, sizeof(unsigned char));
	int i;
	SHA1(initial, length, key);
	memcpy(keys[0], key, SHA_DIGEST_LENGTH*sizeof(unsigned char));
	for (i=1; i<(ROUNDS/2); ++i) {
		memcpy(keys[i], SHA1(&key[i-1], SHA_DIGEST_LENGTH, NULL),
			SHA_DIGEST_LENGTH*sizeof(unsigned char));
	}
}

void sha1sum2text(unsigned char *dest, unsigned char *sum) {
	int i;
	dest[0] = 0;
	for (i=0; i<SHA_DIGEST_LENGTH; ++i) {
		sprintf((char*)dest, "%s%02x", dest, sum[i]);
	}
}

void print_block(unsigned char *block) {
	static int j=0;
	int i;
	for (i=0; i<BLOCK_SIZE; ++i) {
		printf("%02x", block[i]);
	}
	j += BLOCK_SIZE*2;
	if (j % 80 == 0)
		printf("\n");
}
