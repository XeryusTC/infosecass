#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {false=0, true} bool;

void print_usage(char *name);
void encrypt(char *text, char *key, char *dest, int length);
void print_text(char *text, int length);

int main(int argc, char **argv) {
	bool quiet = false;
	if (argc > 1 && strcmp(argv[1], "-q")==0)
		quiet = true;
	if (argc < (3+quiet) || strlen(argv[1+quiet]) != strlen(argv[2+quiet]) ||
		(argc >= (4+quiet) && strlen(argv[1+quiet]) != strlen(argv[3+quiet]))) {
		print_usage(argv[0]);
		exit(-1);
	}
	
	int length = strlen(argv[1+quiet]);
	char *encrypted = calloc(length+1, sizeof(char));
	encrypt(argv[1+quiet], argv[2+quiet], encrypted, length);
	if (!quiet)
		printf("Encrypted:       ");
	print_text(encrypted, length);
	
	if (argc == 4+quiet) {
		encrypt(argv[3+quiet], encrypted, encrypted, length);
		if (!quiet)
			printf("Alternative key: ");
		print_text(encrypted, length);
	}
	
	return 0;
}

void encrypt(char *text, char *key, char *dest, int length) {
	int i;
	for (i=0; i<length; ++i) {
		dest[i] = text[i] ^ key[i];
	}
}

void print_text(char *text, int length) {
	bool non_printable = false;
	int i;
	for (i=0; i<length; ++i) {
		if (text[i] < 0x20 || text[i] > 0x7E)
			non_printable = true;
	}
	
	if (!non_printable)
		printf("%s", text);
	else
		for (i=0; i<length; ++i)
			printf("%02X", text[i]);
	printf("\n");
}

void print_usage(char *name) {
	printf("Usage: %s <text> <key> [alttext]\n", name);
	printf("text, key and alttext should all have the same character length\n");
}
