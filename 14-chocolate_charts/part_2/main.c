#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int equals(uint8_t *first, uint8_t *second, uint8_t length) {
    while ((*(first++) == *(second++)) && length != 0) {
        length--;
    }

    return length == 0;
}

int main() {
    uint8_t input[] = {1,7,0,6,4,1};
    uint8_t input_length = 6;

    uint32_t scoreboard_size = 30000000;
    uint32_t number_of_scores = 2;

    uint8_t *scores = malloc(scoreboard_size * sizeof(uint8_t));
    uint8_t *elf_1 = scores,
            *elf_2 = scores + 1;

    scores[0] = 3;
    scores[1] = 7;

    while (1) {
        uint8_t sum = *elf_1 + *elf_2;

        if (sum >= 10) {
            scores[number_of_scores++] = (uint8_t) (sum / 10);
        }

        if (equals((scores + number_of_scores - 1) - (input_length - 1), input, input_length)) {
            break;
        }

        scores[number_of_scores++] = (uint8_t) (sum % 10);

        if (equals((scores + number_of_scores - 1) - (input_length - 1), input, input_length)) {
            break;
        }

        elf_1 = scores + (((elf_1 - scores) + *elf_1 + 1) % number_of_scores);
        elf_2 = scores + (((elf_2 - scores) + *elf_2 + 1) % number_of_scores);
    }

    printf("%d", number_of_scores - input_length);

    free(scores);
    return 0;
}