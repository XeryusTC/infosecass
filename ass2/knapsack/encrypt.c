#include <stdio.h>
#include <stdlib.h>

#define KEY_LENGTH 8

int main(int argc, char **argv) {
	unsigned int ks[KEY_LENGTH], i, out;
	char c;

	printf("Enter public key (%d unsigned ints): ", KEY_LENGTH);
	for (i=0; i<KEY_LENGTH; ++i)
		scanf("%d", &ks[i]);
	getchar(); // Eat extra \n

	printf("Enter message to encrypt:\n");
	while (EOF != (c = getchar())) {
		if (c=='\n') {
			printf("\n");
			continue;
		}
		out = 0;
		for (i=0; i<KEY_LENGTH; ++i)
			out += ks[i] * ((c >> i) & 1);
		printf("%d ", out);
	}
	return 0;
}
