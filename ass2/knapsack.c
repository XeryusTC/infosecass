#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b); // Greatest common divisor
int invmod(int a, int b); // Solves the modular inverse (x in ax %b == 1)

int main(int argc, char **argv) {
    printf("%d\n", invmod(41, 491));
    return 0;
}

int gcd(int a, int b) {
    /* algorithm taken from https://en.wikipedia.org/wiki/Greatest_common_divisor#Using_Euclid.27s_algorithm as it is clearer than the slides */
    if (a == b)
        return a;
    else if (a > b)
        return gcd(a - b, b);
    else
        return gcd(a, b - a);
}

int invmod(int a, int b) {
    int t0 = 0, t1 = 1, r0 = b, r1 = a, q, tmp;

    while (r1 != 0) {
        q = r0 / r1;

        tmp = t0;
        t0 = t1;
        t1 = tmp - q * t1;

        tmp = r0;
        r0 = r1;
        r1 = tmp - q * r1;
    }
    if (r0 > 1) return -1;
    if (t0 < 0) t0 += b;

    return t0;
}
