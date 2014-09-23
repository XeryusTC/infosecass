#include <stdio.h>
#include <stdlib.h>

#include "cryptomath.h"

#define KEY_LENGTH 8

int main(int argc, char **argv) {
	unsigned int sk[KEY_LENGTH], m, n, minv, in=0;
	int i;
	char c, d;
	
	printf("Enter your private key (10 unsigned ints): ");
	scanf("%d %d", &m, &n);
	for (i=0; i<KEY_LENGTH; i++) {
		scanf("%d", &sk[i]);
	}

	minv = invmod(m, n);
	printf("%d %d %d\n", m, n, minv);
	
	printf("Enter the encrypted ints: ");
	while (EOF != (c = getchar())) {
		if (c >= '0' && c <= '9') {
			in *= 10;
			in += c - '0';
		} else if (in != 0) {
			in = (in * minv) % n;
			d=0;
			for (i=(KEY_LENGTH-1); i>=0; i--) {
				if (sk[i] <= in) {
					d |= 1 << i;
					in -= sk[i];
				}
			}
			printf("%c", d);
			in = 0;
		}
		if (c == '\n')
			printf("\n");
	}
	printf("\n");
	
	return 0;
}
