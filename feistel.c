#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ROUNDS 8
#define BLOCK_SIZE 8  


uint32_t sdbm_hash(const char *str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++))
        hash = c + (hash << 16) + (hash << 6) - hash;
    return (uint32_t)(hash & 0xFF);
}


uint32_t feistel_round(uint32_t x, uint32_t round_key) {
    return x ^ round_key;
}


void feistel_encrypt(uint32_t *L, uint32_t *R, uint32_t key) {
    for (int i = 0; i < ROUNDS; i++) {
        uint32_t round_key = key + i;
        uint32_t temp = *L;
        *L = *R;
        *R = temp ^ feistel_round(*R, round_key);
    }
}


void feistel_decrypt(uint32_t *L, uint32_t *R, uint32_t key) {
    for (int i = ROUNDS - 1; i >= 0; i--) {
        uint32_t round_key = key + i;
        uint32_t temp = *R;
        *R = *L;
        *L = temp ^ feistel_round(*L, round_key);
    }
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

    uint32_t key = sdbm_hash(password);
    unsigned char buffer[BLOCK_SIZE];
    size_t bytes_read;

    if (strcmp(mode, "-e") == 0) {
        
        while ((bytes_read = fread(buffer, 1, BLOCK_SIZE, fin)) > 0) {
            if (bytes_read < BLOCK_SIZE) {
                memset(buffer + bytes_read, 0, BLOCK_SIZE - bytes_read);
            }
            uint32_t L, R;
            memcpy(&L, buffer, 4);
            memcpy(&R, buffer + 4, 4);
            feistel_encrypt(&L, &R, key);
            memcpy(buffer, &L, 4);
            memcpy(buffer + 4, &R, 4);
            fwrite(buffer, 1, BLOCK_SIZE, fout);
        }
    } else if (strcmp(mode, "-d") == 0) {
        // Decryption mode
        while ((bytes_read = fread(buffer, 1, BLOCK_SIZE, fin)) == BLOCK_SIZE) {
            uint32_t L, R;
            memcpy(&L, buffer, 4);
            memcpy(&R, buffer + 4, 4);
            feistel_decrypt(&L, &R, key);
            memcpy(buffer, &L, 4);
            memcpy(buffer + 4, &R, 4);
            fwrite(buffer, 1, BLOCK_SIZE, fout);
        }
    } else {
        fprintf(stderr, "Invalid mode. Use -e for encryption or -d for decryption.\n");
        fclose(fin);
        fclose(fout);
        exit(EXIT_FAILURE);
    }

    fclose(fin);
    fclose(fout);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s -e|-d password input_file output_file\n", argv[0]);
        return EXIT_FAILURE;
    }
    process_file(argv[1], argv[2], argv[3], argv[4]);
    return EXIT_SUCCESS;
}
