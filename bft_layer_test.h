#ifndef BFT_LAYER_TEST_H
#define BFT_LAYER_TEST_H

#include <bft_layer.h>

void print_error_code(int i);

int bft_layer_test();

int test_creation();

int test_addition();

int test_sort();
int test_sort2();

int testClear();

int add_sequence(bft::bft_layer<int> &l, int start, int end);
bool compare_int(int i, int j);


#endif
