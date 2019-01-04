#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define true 1
#define false 0

uint8_t reg[4] = {0, 0, 0, 0};

void addr(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] + reg[b]; }
void addi(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] + b; }
void mulr(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] * reg[b]; }
void muli(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] * b; }
void banr(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] & reg[b]; }
void bani(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] & b; }
void borr(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] | reg[b]; }
void bori(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a] | b; }
void setr(uint8_t a, uint8_t b, uint8_t c) { reg[c] = reg[a]; }
void seti(uint8_t a, uint8_t b, uint8_t c) { reg[c] = a; }
void gtir(uint8_t a, uint8_t b, uint8_t c) { reg[c] = (uint8_t) (a > reg[b]); }
void gtri(uint8_t a, uint8_t b, uint8_t c) { reg[c] = (uint8_t) (reg[a] > b); }
void gtrr(uint8_t a, uint8_t b, uint8_t c) { reg[c] = (uint8_t) (reg[a] > reg[b]); }
void eqir(uint8_t a, uint8_t b, uint8_t c) { reg[c] = (uint8_t) (a == reg[b]); }
void eqri(uint8_t a, uint8_t b, uint8_t c) { reg[c] = (uint8_t) (reg[a] == b); }
void eqrr(uint8_t a, uint8_t b, uint8_t c) { reg[c] = (uint8_t) (reg[a] == reg[b]); }

typedef void Instruction(uint8_t, uint8_t, uint8_t);
typedef uint8_t bool;

Instruction *opcodes[] = {
        addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr
};

void cpy(uint8_t *from, uint8_t *to, uint8_t count) {
    while (count-- != 0) {
        *(to++) = *(from++);
    }
}

bool cmp(uint8_t *first, uint8_t *second, uint8_t count) {
    while ((*(first++) == *(second++)) && count-- != 0);

    return (bool) (count == 0);
}

bool read_numbers(char* input, uint8_t* output) {
    char *ptr = input;
    uint8_t current_number = 0;
    uint8_t result = 0;
    bool is_number = false;

    do {
        if (*ptr >= '0' && *ptr <= '9') {
            result *= 10;
            result += (uint8_t) ((*ptr) - 0x30);
            is_number = true;
        } else {
            if (is_number) {
                output[current_number++] = result;
                result = 0;
                is_number = false;
            }
        }
    } while (*(ptr++) != '\n');

    return true;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    char buffer[24];

    uint8_t instruction[4];
    uint8_t before[4];
    uint8_t after[4];

    uint32_t result = 0;
    uint32_t samples = 0;
    bool is_sample = false;

    if (!file) {
        return -1;
    }

    while (!feof(file) && fgets(buffer, 24, file) != NULL) {
        if (buffer[0] == 'B') {
            samples++;
            is_sample = true;
            read_numbers(buffer, before);
        } else if (buffer[0] == 'A') {
            read_numbers(buffer, after);
        } else if (buffer[0] == '\n') {
            if (is_sample) {
                uint8_t match = 0;

                for (int i = 0; i < (sizeof(opcodes) / sizeof(opcodes[0])); i++) {
                    cpy(before, reg, 4);
                    opcodes[i](instruction[1], instruction[2], instruction[3]);

                    match += cmp(reg, after, 4);

                    if (match == 3) {
                        result++;
                        break;
                    }
                }

                is_sample = false;
            } else {
                break;
            }
        } else {
            read_numbers(buffer, instruction);
        }
    }

    printf("Samples: %d, result: %d", samples, result);

    fclose(file);
    return 0;
}