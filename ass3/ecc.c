#include <stdio.h>
#include <stdlib.h>

void print_usage(char *name);

int main(int argc, char **argv) {
	int x1, x2, x3, y1, y2, y3, m, a, p;
	
	if (argc < 5) {
		print_usage(argv[0]);
		exit(-1);
	}
	a  = atoi(argv[1]);
	p  = atoi(argv[2]);
	x1 = atoi(argv[3]);
	y1 = atoi(argv[4]);
	if (argc == 7) { // Two points
		x2 = atoi(argv[5]);
		y2 = atoi(argv[6]);
		if (x2 - x1 == 0) {
			printf ("Could not add points\n");
			exit (-1);
		}
		printf("%d %d\n", (y2 - y1), (x2 - x1));
		m  = ((y2 - y1) / (x2 - x1)) % p;
	} else { // One point
		m = ((3 * x1 * x1 + a) / (2 * y1)) % p;
		x2 = x1;
	}
	
	printf("m: %d\n", m);
	
	x3 = (m * m - x1 - x2) % p;
	y3 = (m * (x1 - x3) - y1) % p;
	
	printf("x3: %d\ty3: %d\n", x3, y3);
	
	return 0;
}

void print_usage(char *name) {
	printf("Usage: %s a p x1 y1 [x2 y2]\n", name);
	printf("Calculates point addition for eleptic curve y^2 = x^3 + ax + b ");
	printf("(mod p).\nWhen only one point is given it doubles that point.\n");
}
