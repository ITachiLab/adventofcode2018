#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HASH(x, y) (((uint32_t)(x) << 16) | (y))

#define TEST_MODE

typedef struct {
    uint16_t x;
    uint16_t y;
} Coords;

typedef struct {
    uint16_t id;
    Coords tl;
    Coords br;
} Claim;

typedef struct Node Node;
struct Node {
    uint32_t key;
    Node* left;
    Node* right;
};

Node *overlap_set;

Node* overlap_insert(Node *node, uint16_t x, uint16_t y) {
    uint32_t key = HASH(x, y); 

    if (!node) {
        Node *new_node = malloc(sizeof(Node));
        new_node->key = key;

        // Initialize tree if empty
        if (!overlap_set) {
            overlap_set = new_node;
        }

        return new_node;
    }

    if (key < node->key) {
        node->left = overlap_insert(node->left, x, y);
    } else if (key > node->key) {
        node->right = overlap_insert(node->right, x, y);
    }

    return node;
}

Node* overlap_search(Node* node, uint16_t x, uint16_t y) {
    uint32_t key = HASH(x, y);

    if (!node || node->key == key) {
        return node;
    }

    if (node->key < key) {
        return overlap_search(node->right, x, y);
    }

    return overlap_search(node->left, x, y);
}

/**
 * Initialize claim object.
 */
void init_claim(Claim *claim, uint16_t id, uint16_t cx, uint16_t cy, uint16_t width, uint16_t height) {
    claim->id = id;
    claim->tl.x = cx;
    claim->tl.y = cy;
    claim->br.x = cx + width;
    claim->br.y = cy + height;
}

/**
 * Check whether a given coordinate is in a given claim.
 */
uint8_t is_in(Claim *hay, uint16_t x, uint16_t y) {
    return  
        x <= hay->br.x &&
        y <= hay->br.y &&
        x >= hay->tl.x &&
        y >= hay->tl.y;
}

/**
 * Parse input line from file.
 * Example:
 *
 * #1 @ 755,138: 26x19
 *
 */
uint8_t parse_claim(const char *str, Claim *claim) {
    uint16_t id, cx, cy, width, height;

    sscanf(str, "%*c%hu %*c %hu%*c%hu%*c %hu%*c%hu", &id, &cx, &cy, &width, &height);

    init_claim(claim, id, cx, cy, width, height);
}

uint16_t calculate_overlaps(Claim *current_claim, Claim **claims, uint16_t claims_number) {
    uint16_t overlap_count = 0;

    for (int x = current_claim->tl.x; x <= current_claim->br.x; x++) {
        for (int y = current_claim->tl.y; y <= current_claim->br.y; y++) {

            for (int i = 0; i < claims_number; i++) {
                if (claims[i]->id != current_claim->id) {
                    overlap_count += is_in(claims[i], x, y);
                }
            }
        }
    }
}

#ifdef TEST_MODE
#include "test.c"
#else

int main(void) {
    FILE *fp;
    char read_buffer[128];
    Claim claims[1500];
    const char *file_name = "input.txt";

    fp = fopen(file_name, "r");

    if (fp == NULL) {
        printf("Could not open file");
        return 1;
    }

    int c = 0;

    while (fgets(read_buffer, 128, fp) != NULL) {
       parse_claim(read_buffer, &claims[c]);
       c++;
    }



    fclose(fp);

    return 0;
}

#endif

