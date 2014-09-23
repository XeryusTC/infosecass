#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "cryptomath.h"

#define KEY_LENGTH 8

int next_pseudo_prime(int p);

int main(int argc, char **argv) {
    // siks = Super Increasing KnapSack, gks = Generic Knap Sack
    unsigned int siks[KEY_LENGTH], gks[KEY_LENGTH], i, total=0;

    srand(time(NULL));

    for (i=0; i<KEY_LENGTH; ++i) {
        siks[i] = rand() % 5 + total + 1;
        total += siks[i];
        printf("%d ", siks[i]);
    }

    printf("\n%d\n", next_pseudo_prime(total));

    return 0;
}

int next_pseudo_prime(int p) {
    printf("%d\n", p);
    do {
        p++;
    } while (expmod(2, p-1, p) != 1);
    return p;
}
