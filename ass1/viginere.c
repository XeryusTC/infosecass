#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MIN_KEY_LENGTH 5
#define MAX_KEY_LENGTH 15
#define TOTAL_KEYS (MAX_KEY_LENGTH - MIN_KEY_LENGTH)
#define BUFFER_SIZE 2048
#define NUM_ALPHA 26 // Amount of letters in the alphabet

typedef enum { false, true } bool;

void get_secret(char *dest);
void get_frequencies(char *secret, int **dest_vector, int key_length);
float calc_key_stddev(char *secret, int key_length);
float stddev(int *key);
int find_spike(float *key_stddevs);
void cycle_keys(char *secret, int key_length);
void cycle_keys_rec(char *secret, int n, char *candidates, int *num_candidates,
        int key_length, char *key);
int guess_nth_letter(int n, int *vector, int key_length, char *dest);
void decrpyt(char *secret, int key_length, char *key, int limit);

int main(int argc, char **argv) {
    int i, best_key_length;
    char *secret = calloc(BUFFER_SIZE, sizeof(char));
    float key_stddevs[TOTAL_KEYS];

    get_secret(secret);
    printf("ENCRYPTED TEXT (%d bytes)\n%s\n\n", strlen(secret), secret);

    for (i=MIN_KEY_LENGTH; i<=MAX_KEY_LENGTH; ++i) {
        key_stddevs[i - MIN_KEY_LENGTH] = calc_key_stddev(secret, i);
        printf("Sum of %2d std. devs: %2.1f\n", i,
            key_stddevs[i - MIN_KEY_LENGTH]);
    }

    best_key_length = find_spike(key_stddevs) + MIN_KEY_LENGTH;
    printf("\nMost likely key length: %d\n", best_key_length);

    cycle_keys(secret, best_key_length);

    free(secret);
    printf("\n");
    return 0;
}

void cycle_keys(char *secret, int key_length) {
    int vector[key_length][NUM_ALPHA], i, j, max, num_candidates[key_length];
    char candidates[key_length * NUM_ALPHA],
        *key = calloc(key_length+1, sizeof(char));

    get_frequencies(secret, (int**)vector, key_length);

    printf("\nCANDIDATES:\n");

    // Get all possibilities for each letter
    for (i=0; i<key_length; ++i) {
        max = 0;
        printf("letter %d: ", i+1);

        num_candidates[i] = guess_nth_letter(i, vector[i], key_length,
            &candidates[i*NUM_ALPHA]);
    }
    printf("\n");

    cycle_keys_rec(secret, 0, candidates, num_candidates, key_length, key);
    free(key);
}

void cycle_keys_rec(char *secret, int n, char *candidates,
        int *num_candidates, int key_length, char *key) {
    int i;

    if (n == key_length) {
        printf("\tTrying key \"%s\":\n", key);
        decrpyt(secret, key_length, key, 70);
    } else {
        for (i=0; i<num_candidates[n]; ++i) {
            key[n] = candidates[n*NUM_ALPHA + i];
            cycle_keys_rec(secret, n+1, candidates, num_candidates, key_length, key);
        }
    }
}

int guess_nth_letter(int n, int *vector, int key_length, char *dest) {
    int max = 0, i = 0, count = 0, j = 0;

    // Get the maximum frequency
    for (i=0; i<NUM_ALPHA; ++i)
        if (vector[i] > max)
            max = vector[i];

    // Find possible candidates
    for (i=0; i<NUM_ALPHA; ++i) {
        if (vector[i] + 2 >= max) {
            ++count;
            dest[j++] = (i - 4 + NUM_ALPHA) % NUM_ALPHA + 'a';
            dest[j] = '\0';
            printf("%c", dest[j]);
        }
    }
    printf("%s\n", dest);
    return count;
}

void decrpyt(char *secret, int key_length, char *key, int limit) {
    int i;

    for (i=0; i<limit; ++i) {
        putchar((secret[i] - key[i%key_length] + NUM_ALPHA) % NUM_ALPHA + 'a');
    }
    printf("\n");
}

void get_frequencies(char *secret, int **dest_vector, int key_length) {
    int i, j, vect[key_length][NUM_ALPHA];
    // Zero the vector
    for (i=0; i<key_length; ++i)
        for (j=0; j<NUM_ALPHA; ++j)
            vect[i][j] = 0;

    for (i=0; i<strlen(secret); ++i)
        vect[i % key_length][secret[i] - 'a']++;

    memcpy(dest_vector, vect, sizeof(vect));
}

int find_spike(float *key_stddevs) {
    int i, largest_spike_id=0;
    float spike_size, largest_spike=0.0f;

    for (i=0; i<TOTAL_KEYS; ++i) {
        spike_size = 0.0f;
        if (i > 0)
            spike_size += key_stddevs[i] - key_stddevs[i-1];
        if (i < TOTAL_KEYS)
            spike_size += key_stddevs[i] - key_stddevs[i+1];
        if (spike_size > largest_spike) {
            largest_spike_id = i;
            largest_spike = spike_size;
        }
    }

    return largest_spike_id;
}

float calc_key_stddev(char *secret, int key_length) {
    int vector[key_length][NUM_ALPHA], i, j;
    float total_stddev = 0.0f;

    get_frequencies(secret, (int**)vector, key_length);

    for (i=0; i<key_length; ++i)
        total_stddev += stddev(vector[i]);

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
    for (i=0; i<NUM_ALPHA; ++i)
        squares += pow(key[i] - mean, 2);

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
            // This is an unrecognised char, move the position in the buffer
            // one back so we don't accidentally add \0 halfway through the
            // secret
            --i;
        }
    }
}

