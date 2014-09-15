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

int main(int argc, char **argv) {
    int i;
    char *secret = calloc(BUFFER_SIZE, sizeof(char));
    float key_stddevs[TOTAL_KEYS];
    
    get_secret(secret);
    printf("%d: %s\n", strlen(secret), secret);
    
    for (i=MIN_KEY_LENGTH; i<=MAX_KEY_LENGTH; ++i) {
        key_stddevs[i - MIN_KEY_LENGTH] = calc_key_stddev(secret, i);
        printf("Sum of %2d std. devs: %2.1f\n", i, key_stddevs[i - MIN_KEY_LENGTH]);
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

    // Tally the frequencies of letters
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
    float mean, total=0.0f, squares=0.0f;
    
    // Calculate the mean
    for (i=0; i<NUM_ALPHA; ++i)
        total += key[i];
    mean = total / NUM_ALPHA;
    
    // Calculate the distances
    for (i=0; i<NUM_ALPHA; ++i) {
        squares += pow(key[i] - mean, 2);
    }

    return sqrt(squares / (NUM_ALPHA - 1.0f));
}

void get_secret(char *dest) {
    char c;
    int i;
    for (i=0; i<BUFFER_SIZE && (c=getchar()); ++i) {
        if (c == EOF) {
            dest[i] = '\0';
            return;
        } else if (c >= 'a' && c <= 'z') {
            dest[i] = c;
        } else {
            // This is an unrecognised char, move the position in the buffer one back so we don't accidentally add \0 halfway through the secret
            --i;
        }
    }
}

