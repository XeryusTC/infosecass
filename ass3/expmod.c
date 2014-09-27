#include <stdio.h>
#include <stdlib.h>

int expmod(int a, int e, int n) {
    if (a > n)
        return expmod((a % n), e, n);
    else if (e == 1)
        return (a % n);
    else if (e == 0)
        return 1;
    else if (a == 0)
        return 0;
    else if (e % 2 == 0)
        return expmod(a * a, e / 2, n);
    else
        return (expmod(a, e - 1, n) * a) % n;
}

void print_usage(char *name) {
	printf("Usage: %s a e n\nComputes the solution to a^e %% n", name);
}

int main(int argc, char **argv) {
	if (argc != 4) {
		print_usage(argv[0]);
		exit(-1);
	}
	
	printf("%d\n", expmod(atoi(argv[1]), atoi(argv[2]), atoi(argv[3])));
	return 0;
}
