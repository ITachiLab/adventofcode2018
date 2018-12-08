#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t fabric[1000][1000] = {0};
uint32_t overlaps = 0;
uint8_t claims[1307];

void fill_count(uint16_t id, uint16_t cx, uint16_t cy, uint16_t width, uint16_t height) {
    for (int x = cx, i = 0; i < width; x++, i++) {
        for (int y = cy, j = 0; j < height; y++, j++) {
            if (!fabric[x][y]) {
                fabric[x][y] = id;
            } else {
                claims[id - 1] = 1;
                claims[fabric[x][y] - 1] = 1;
            }
        }
    }
}

int main(void) {
    FILE *fp;
    char read_buffer[128];
    const char *file_name = "input.txt";

    fp = fopen(file_name, "r");

    if (fp == NULL) {
        printf("Could not open file");
        return 1;
    }

    while (fgets(read_buffer, 128, fp) != NULL) {
        uint16_t id, cx, cy, width, height;
        sscanf(read_buffer, "%*c%hu %*c %hu%*c%hu%*c %hu%*c%hu", &id, &cx, &cy, &width, &height);

        fill_count(id, cx, cy, width, height);
    }

    fclose(fp);

    for (int i = 0; i < 1307; i++) {
        if (!claims[i]) {
            printf("Non overlapping claim: %d", i + 1);
            break;
        }
    }

    return 0;
}
