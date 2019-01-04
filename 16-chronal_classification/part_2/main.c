#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define true 1
#define false 0

uint32_t reg[4] = {0, 0, 0, 0};

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

Instruction *valid_opcodes[16];

void cpy_to_reg(const uint8_t *from) {
    for (int i = 0; i < 4; i++) {
        reg[i] = from[i];
    }
}

bool cmp_reg(const uint8_t *with) {
    for (int i = 0; i < 4; i++) {
        if (reg[i] != with[i]) {
            return false;
        }
    }

    return true;
}

bool instruction_available(Instruction *instr) {
    for (int i = 0; i < 16; i++) {
        if (valid_opcodes[i] == instr) {
            return true;
        }
    }

    return false;
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

    bool is_sample = false;

    if (!file) {
        return -1;
    }

    while (!feof(file) && fgets(buffer, 24, file) != NULL) {
        if (buffer[0] == 'B') {
            is_sample = true;
            read_numbers(buffer, before);
        } else if (buffer[0] == 'A') {
            read_numbers(buffer, after);
        } else if (buffer[0] == '\n') {
            if (is_sample) {
                Instruction *possible_instruction = NULL;

                if (!valid_opcodes[instruction[0]]) {
                    for (int i = 0; i < (sizeof(opcodes) / sizeof(opcodes[0])); i++) {
                        if (!instruction_available(opcodes[i])) {
                            cpy_to_reg(before);
                            opcodes[i](instruction[1], instruction[2], instruction[3]);

                            if (cmp_reg(after)) {
                                if (!possible_instruction) {
                                    possible_instruction = opcodes[i];
                                } else {
                                    possible_instruction = NULL;
                                    break;
                                }
                            }
                        }
                    }

                    // Only one instruction gave the same result, so it must be the one valid.
                    if (possible_instruction) {
                        valid_opcodes[instruction[0]] = possible_instruction;
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

    for (int i = 0; i < 16; i++) {
        printf("%p\n", valid_opcodes[i]);
    }

    printf("\n");

    for (int i = 0; i < 16; i++) {
        printf("%p\n", opcodes[i]);
    }

    reg[0] = reg[1] = reg[2] = reg[3] = 0;

    while (!feof(file) && fgets(buffer, 24, file) != NULL) {
        if (buffer[0] == '\n') {
            continue;
        }

        read_numbers(buffer, instruction);
        valid_opcodes[instruction[0]](instruction[1], instruction[2], instruction[3]);
    }

    printf("%d", reg[0]);

    fclose(file);
    return 0;
}