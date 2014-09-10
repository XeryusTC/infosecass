#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

void print_usage();
const char* generate_mapping(const int offset);
const char* decrypt_mapping(const char* mapping);
void encrypt(const char* mapping);

int main(int argc, char **argv) {
    int decrypt = FALSE, offset = 0;
    char mapping[27];
    memset(mapping, 0, 27*sizeof(char));

    if (argc == 1) {
        print_usage();
        exit(-1);
    }
    if (strcmp(argv[1], "-d") == 0) {
        if (argc == 2) {
            print_usage();
            exit(-1);
        }
        decrypt = TRUE;
    }

    if (strlen(argv[1+decrypt]) == 26)
        memcpy(mapping, argv[1+decrypt], 26*sizeof(char));
    else if (sscanf(argv[1+decrypt], "%d", &offset))
        memcpy(mapping, generate_mapping(offset), 26*sizeof(char));
    else {
        print_usage();
        exit(-1);
    }

    if (decrypt) 
        memcpy(mapping, decrypt_mapping(mapping), sizeof(mapping));

    encrypt(mapping);

    return 0;
}

const char* generate_mapping(const int offset) {
    int i;
    char *mapping = calloc(27, sizeof(char));
    
    for (i=0; i<26; ++i)
        mapping[i] = (i + offset) % 26 + 'a';
    return mapping;
}

const char* decrypt_mapping(const char* mapping) {
    int i;
    char *dmap = calloc(27, sizeof(char));

    for (i=0; i<26; ++i)
        dmap[mapping[i]-'a'] = i + 'a';

    return dmap;
}

void encrypt(const char* mapping) {
    char c;
    while ((c = getchar())) {
        c = tolower(c);
        if (c >= 'a' && c <= 'z')
            putchar(mapping[c-'a']);
        else if (c == '\n')
            printf("\n");
        else if (c == EOF)
            break;
    }
}

void print_usage() {
    printf("Usage: substitution [-d] mapping\n");
    printf("Where:\n");
    printf("\t-d: decrypt");
    printf("\n\tmapping: 26 letter char-mapping");
    printf("\n\t\t or an int-value\n\n");
    printf("En/Decrypts stdin to stdout. Only letters are encrypted,\n");
    printf("all other characters are silently ignored\n");
    printf("Use an int-value to to a letter shift (%% 26, 0: a = a)\n");
    printf("Shift 3 is the classical Caesar encryption\n");
}
