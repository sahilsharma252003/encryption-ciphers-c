#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MODULUS 256
#define MULTIPLIER 1103515245
#define INCREMENT 12345

unsigned long sdbm_hash(const char *str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash = c + (hash << 16) + (hash << 6) - hash;
    }
    return hash;
}

unsigned char lcg_next(unsigned long *state) {
    *state = (*state * MULTIPLIER + INCREMENT) % MODULUS;
    return (unsigned char)(*state);
}

void process_file(const char *password, const char *input_file, const char *output_file) {
    FILE *fin = fopen(input_file, "rb");
    if (!fin) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    
    FILE *fout = fopen(output_file, "wb");
    if (!fout) {
        perror("Error opening output file");
        fclose(fin);
        exit(EXIT_FAILURE);
    }
    
    unsigned long seed = sdbm_hash(password);
    unsigned char buffer;
    
    while (fread(&buffer, 1, 1, fin) == 1) {
        buffer ^= lcg_next(&seed); 
        fwrite(&buffer, 1, 1, fout);
    }
    
    fclose(fin);
    fclose(fout);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s password input_file output_file\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    process_file(argv[1], argv[2], argv[3]);
    return EXIT_SUCCESS;
}
