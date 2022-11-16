
#include "tree.h"
#include "string.h"

extern Tree* build_encoding_tree(Tree* keys);

static void get_key_codes(Tree* key_codes, Tree* encoding_tree);

extern Tree* make_codes(Tree* huffman_tree);