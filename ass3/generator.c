#include <stdio.h>
#include <stdlib.h>

#include "cryptomath.h"

// returns a list of bits represeting 2 to 34. If the bit is set then that
// corresponding number is a generator
int generator(int p) {
	int found = 0xffffffff;
	int q = 2, p_remaining = p-1, i;
	while (q <= p_remaining) {
		if (p_remaining % q != 0) { // q is not a prime factor, so skip
			++q;
		} else {
			// first remove this prime factor
			do {
				p_remaining /= q;
			} while (p_remaining % q == 0);
			// Check the numbers 2 throug 34 if they're a generator
			for (i=0; i<32; ++i) {
				if (expmod((i+1), ((p-1)/q), p) == 1)
					found &= ~(1 << i);
			}
		}
	}
	return found;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s p\nComputes generator values for a prime p\n",
			argv[0]);
		exit(-1);
	}
	
	int found = generator(atoi(argv[1])), i;
	printf("Generators: ");
	for (i=0; i<32; ++i) {
		if (found & (1 << i))
			printf("%d, ", i+2);
	}
	printf("\n");
	
	return 0;
}
