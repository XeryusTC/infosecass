#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "cryptomath.h"

#define KEY_LENGTH 8

int next_prime(int p);

int main(int argc, char **argv) {
    // sk = Super increasing knapsack, gk = Generic Knapsack
    unsigned int sk[KEY_LENGTH], gk[KEY_LENGTH], i, total=0, m, n;

    srand(time(NULL));

	// Generate superincreasing knapsack
    for (i=0; i<KEY_LENGTH; ++i) {
        sk[i] = rand() % 4 + total + 1;
        total += sk[i];
    }

	// Find n and m, where n rp m
	n = next_prime(total);
    do {
		m = rand() % (total/4);
	} while (gcd(n, m) != 1);
	
	// Generate generic knapsack
	for (i=0; i<KEY_LENGTH; ++i) {
		gk[i]  = (m * sk[i]) % n;
	}
	
	printf("Private key: %3d %3d ", m, n);
	for (i=0; i<KEY_LENGTH; ++i) {
		printf("%3d ", sk[i]);
	}
	printf("\nPublic key:  ");
	for (i=0; i<KEY_LENGTH; ++i) {
		printf("%3d ", gk[i]);
	}
	printf("\n");

    return 0;
}

int next_prime(int p) {
	if (p % 2 == 0)
		++p;
    do {
        p += 2;
    } while (expmod(2, p-1, p) != 1);
    return p;
}
