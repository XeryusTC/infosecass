#ifndef CRYPTOMATH_H
#define CRYPTOMATH_H

int gcd(int a, int b); // Greatest common divisor
int invmod(int a, int b); // Solves the modular inverse (x in ax %b == 1)
int expmod(int a, int e, int n); // Solves a^e % n = x

#endif /* CRYPTOMATH_H */
