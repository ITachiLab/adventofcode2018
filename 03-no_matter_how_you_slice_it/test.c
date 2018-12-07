#define TEST(x) uint8_t x(void) 
#define assert_equals(actual, expected) {\
    if (expected != actual) {\
        return 1;\
    }\
}

#define assert_true(test) {\
    if (!test) {\
        return 1;\
    }\
}

#define assert_false(test) {\
    if (test) {\
        return 1;\
    }\
}

#define RUN(x) if (x() != 0) printf("[FAILED] " #x "\n"); else printf("[PASSED] " #x "\n");

TEST(should_init_claim) {
    Claim claim;
    init_claim(&claim, 69, 3, 5, 4, 4);

    assert_equals(claim.id, 69);
    assert_equals(claim.tl.x, 3);
    assert_equals(claim.tl.y, 5);
    assert_equals(claim.br.x, 7);
    assert_equals(claim.br.y, 9);

    return 0;
}

TEST(should_overlap) {
    Claim claim;
    init_claim(&claim, 69, 3, 5, 4, 4);

    assert_true(is_in(&claim, 4, 6));

    return 0;
}

TEST(should_not_overlap) {
    Claim claim;
    init_claim(&claim, 69, 3, 5, 4, 4);

    assert_false(is_in(&claim, 8, 10));

    return 0;
}

TEST(should_parse_claim_input) {
    const char* valid_input = "#1 @ 755,138: 26x19";
    Claim claim;

    parse_claim(valid_input, &claim);

    assert_equals(claim.id, 1);
    assert_equals(claim.tl.x, 755);
    assert_equals(claim.tl.y, 138);
    assert_equals(claim.br.x, 781);
    assert_equals(claim.br.y, 157);

    return 0;
}

TEST(should_create_and_search_bst) {
    Node *root;

    root = overlap_insert(root, 1, 1);
    overlap_insert(root, 1, 2);
    overlap_insert(root, 10, 15);
    overlap_insert(root, 13, 16);

    Node *find = overlap_search(root, 1, 1);

    assert_equals(find->key, HASH(1, 1));

    return 0;
}

int main(void) {
    RUN(should_init_claim);
    RUN(should_overlap);
    RUN(should_not_overlap);
    RUN(should_parse_claim_input);
    RUN(should_create_and_search_bst);
}
