#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define IV_SIZE 8
#define MULTIPLIER 1103515245UL
#define INCREMENT 12345UL
#define MODULUS 256UL   

unsigned long sdbm_hash(const char *str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++))
        hash = c + (hash << 16) + (hash << 6) - hash;
    return hash;
}


unsigned char lcg_next(unsigned long *state) {
    *state = (*state * MULTIPLIER + INCREMENT) % MODULUS;
    return (unsigned char)(*state);
}

void generate_iv(unsigned char *iv) {
    unsigned char fixed_iv[IV_SIZE] = {0x6b, 0xf7, 0x6f, 0x2f, 0x85, 0x5a, 0x1c, 0x51};
    memcpy(iv, fixed_iv, IV_SIZE);
}

unsigned long iv_to_ulong(const unsigned char *iv) {
    unsigned long val = 0;
    for (int i = 0; i < IV_SIZE; i++) {
        val |= ((unsigned long)iv[i]) << (8 * i);
    }
    return val;
}

long get_file_size(FILE *f) {
    long current = ftell(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, current, SEEK_SET);
    return size;
}

void process_file(const char *mode, const char *password, const char *input_filename, const char *output_filename) {
    FILE *fin = fopen(input_filename, "rb");
    if (!fin) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *fout = fopen(output_filename, "wb");
    if (!fout) {
        perror("Error opening output file");
        fclose(fin);
        exit(EXIT_FAILURE);
    }

    unsigned long seed;
    unsigned char iv[IV_SIZE];

    if (strcmp(mode, "encrypt") == 0) {
        
        generate_iv(iv);
        
        unsigned long iv_val = iv_to_ulong(iv);
        for (int i = 0; i < IV_SIZE; i++) {
            unsigned char byte = (iv_val >> (8 * i)) & 0xFF;
            fwrite(&byte, 1, 1, fout);
        }
       
        seed = sdbm_hash(password) ^ iv_val;
    } else { 
        
        unsigned char iv_bytes[IV_SIZE];
        if (fread(iv_bytes, 1, IV_SIZE, fin) != IV_SIZE) {
            fprintf(stderr, "Error reading IV from ciphertext\n");
            fclose(fin);
            fclose(fout);
            exit(EXIT_FAILURE);
        }
        
        unsigned long iv_val = 0;
        for (int i = 0; i < IV_SIZE; i++) {
            iv_val |= ((unsigned long)iv_bytes[i]) << (8 * i);
        }
        memcpy(iv, iv_bytes, IV_SIZE);
        seed = sdbm_hash(password) ^ iv_val;
        
        long file_size = get_file_size(fin);
        
        long ciphertext_end = file_size - IV_SIZE; 
        while (ftell(fin) < ciphertext_end) {
            unsigned char buffer;
            if (fread(&buffer, 1, 1, fin) != 1) break;
            buffer ^= lcg_next(&seed);
            fwrite(&buffer, 1, 1, fout);
        }
        fclose(fin);
        fclose(fout);
        return;
    }

    
    unsigned char buffer;
    while (fread(&buffer, 1, 1, fin) == 1) {
        buffer ^= lcg_next(&seed);
        fwrite(&buffer, 1, 1, fout);
    }
    fclose(fin);

    
    unsigned long iv_val = iv_to_ulong(iv);
    for (int i = 0; i < IV_SIZE; i++) {
        unsigned char byte = (iv_val >> (8 * i)) & 0xFF;
        fwrite(&byte, 1, 1, fout);
    }
    fclose(fout);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s -e|-d password input_file output_file\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "-e") == 0) {
        process_file("encrypt", argv[2], argv[3], argv[4]);
    } else if (strcmp(argv[1], "-d") == 0) {
        process_file("decrypt", argv[2], argv[3], argv[4]);
    } else {
        fprintf(stderr, "Invalid option. Use -e for encryption or -d for decryption.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
