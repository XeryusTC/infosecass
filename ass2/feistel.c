#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define ROUNDS 10
#define BLOCK_SIZE 8
#define HALF_BLOCK (BLOCK_SIZE / 2)

typedef enum {false=0, true} bool;

void feistel(unsigned char *password, int password_length, char *block, int block_length);
unsigned int f(char *key);
void sha1sum2text(unsigned char *dest, unsigned char *sum);

int main(int argc, char **argv) {
	unsigned char *password = calloc(256, sizeof(char));
	printf("Password: ");
	scanf("%s", password);

	feistel(password, strlen((char*)password), "abcdefgh", 8);
	return 0;
}

void feistel(unsigned char *password, int password_length, char *block, int block_length) {
	unsigned char *md = calloc(SHA_DIGEST_LENGTH, sizeof(char)),
		*b = calloc(BLOCK_SIZE, sizeof(char));
	unsigned int r_old=0, r_new=0, l_old=0, l_new=0, k1=0, k2=0;
	int i;

	// Add padding if block is too short
	if (block_length < BLOCK_SIZE)
		memset(b, (BLOCK_SIZE - block_length), sizeof(b));
	memcpy(b, block, block_length);

	// Generate initial key from password
	SHA1(password, password_length, md);

	memcpy(&l_old, block, HALF_BLOCK);
	memcpy(&r_old, &block[HALF_BLOCK-1], HALF_BLOCK);
	memcpy(&k1, md, HALF_BLOCK);
	memcpy(&k2, &md[HALF_BLOCK-1], HALF_BLOCK);
	for (i=0; i<(ROUNDS/2); ++i) {
		r_new = l_old ^ k1;
		printf("%d\n", r_new);

		// Generate next key
		//SHA1(sha1sum2text(md), SHA_DIGEST_LENGTH, md);
	}
}

void sha1sum2text(unsigned char *dest, unsigned char *sum) {
	int i;
	dest[0] = 0;
	for (i=0; i<SHA_DIGEST_LENGTH; ++i) {
		sprintf((char*)dest, "%s%02x", dest, sum[i]);
	}
}
