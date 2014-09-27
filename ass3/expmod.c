#include <stdio.h>
#include <stdlib.h>

#include "cryptomath.h"

int main(int argc, char **argv) {
	if (argc != 4) {
		printf("Usage: %s a e n\nComputes the solution to a^e %% n", argv[0]);
		exit(-1);
	}
	
	printf("%d\n", expmod(atoi(argv[1]), atoi(argv[2]), atoi(argv[3])));
	return 0;
}
