#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MIN_KEY_LENGTH 5
#define MAX_KEY_LENGTH 15
#define TOTAL_KEYS (MAX_KEY_LENGTH - MIN_KEY_LENGTH)
#define BUFFER_SIZE 2048
#define NUM_ALPHA 26 // Amount of letters in the alphabet

float stddev(int *key);
void get_secret(char *dest);
float calc_key_stddev(char *secret, int key_length);

char *secret;

int main(int argc, char **argv) {
    int i;
    secret = calloc(BUFFER_SIZE, sizeof(char));
    
    get_secret(secret);
    
    for (i=MIN_KEY_LENGTH; i<=MAX_KEY_LENGTH; ++i) {
        printf("%d: %f\n", i, calc_key_stddev(secret, i));
    }

    free(secret);
    printf("\n");
    return 0;
}

float calc_key_stddev(char *secret, int key_length) {
    int vector[key_length][NUM_ALPHA], i, j;
    float total_stddev = 0.0f;
    
    // Zero the vector
    for (i=0; i<key_length; ++i) {
        for (j=0; j<NUM_ALPHA; ++j) {
            vector[i][j] = 0;
        }
    }

    for (i=0; i<strlen(secret); ++i) {
        vector[i % key_length][secret[i] - 'a']++;
    }
    
    for (i=0; i<key_length; ++i) {
        total_stddev += stddev(vector[i]);
    }
    
    return total_stddev;
}

float stddev(int *key) {
    int i=0;
    const int size = 26;
    float mean, total=0.0f, squares=0.0f;
    
    // Calculate the mean
    for (i=0; i<size; ++i)
        total += key[i];
    mean = total / size;
    
    // Calculate the distances
    for (i=0; i<size; ++i) {
        squares += (key[i] - mean) * (key[i] - mean);
    }

    return sqrt(squares / (float)size);
}

void get_secret(char *dest) {
    char c;
    int i;
    for (i=0; i<BUFFER_SIZE && (c=getchar()); ++i) {
        if (c == '\n') {
            --i; // Go one character back so we dont insert anything on \n, otherwise it will be recognised as \0
        } else if (c >= 'a' && c <= 'z') {
            dest[i] = c;
        } else {
            dest[i] = '\0';
            break;
        }
    }
}

