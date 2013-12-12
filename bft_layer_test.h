#ifndef BFT_LAYER_TEST_H
#define BFT_LAYER_TEST_H

#include <bft_layer.h>

//helpers
void print_error_code(int i);

int add_sequence(bft::bft_layer<int, int> &l, int start, int end);

bool compare_int(bft::bft_node<int,int> i, bft::bft_node<int,int> j);


//tests
int bft_layer_test();

int test_creation();

int test_addition();

int test_sort();
int test_sort2();

int test_clear();

int test_find();
int test_find2();


#endif
