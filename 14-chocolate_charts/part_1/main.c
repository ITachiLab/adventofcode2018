#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    uint32_t input = 170641;
    uint32_t scoreboard_size = input + 10;
    uint32_t number_of_scores = 2;

    uint8_t *scores = malloc(scoreboard_size * sizeof(uint8_t));
    uint8_t *elf_1 = scores,
            *elf_2 = scores + 1;

    scores[0] = 3;
    scores[1] = 7;

    while (number_of_scores <= input + 10) {
        uint8_t sum = *elf_1 + *elf_2;

        if (sum >= 10) {
            scores[number_of_scores++] = (uint8_t) (sum / 10);
        }

        scores[number_of_scores++] = (uint8_t) (sum % 10);

        elf_1 = scores + (((elf_1 - scores) + *elf_1 + 1) % number_of_scores);
        elf_2 = scores + (((elf_2 - scores) + *elf_2 + 1) % number_of_scores);
    }

    uint8_t *start = scores + input;

    for (uint8_t i = 0; i < 10; i++) {
        printf("%d", *(start++));
    }

    free(scores);
    return 0;
}