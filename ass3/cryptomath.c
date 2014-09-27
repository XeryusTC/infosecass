#include <stdio.h>
#include <stdlib.h>

#include "cryptomath.h"

int gcd(int a, int b) {
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
