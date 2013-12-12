
#include <bft_layer_test.h>
#include <bft_layer.h>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>

void print_error_code(int i) {
    std::cerr<<"error code: "<<i<<std::endl;
}

int bft_layer_test() {
    int ret = 0;
    if ( (ret = test_creation()) != 0) {
        print_error_code(ret);
    }

    if ( (ret=test_addition()) != 0) {
        print_error_code(ret);
    }

    return ret;
}

// test of creation layer object
int test_creation() {
    bft::bft_layer<int> layer_int();
    bft::bft_layer<std::string> layer_string();
    bft::bft_layer<std::vector<int> > layer_vector_int();
    return 0;
}

int test_addition() {
    int ret = 0;
    bft::bft_layer<int> layer(10);
    for (int i=0; i<10; i++) {
        if (layer.add(i) != 0) {
            ret = 2;
            break;
        }
    }

    //layer if full
    for (int i=0; i<100; i++) {
        if (layer.add(i) != -1) {
            ret = 3;
            break;
        }
    }

    //test of correctness
    for (int i=0; i<10; i++) {
        int* cur = layer.get(i);
        if (*cur != i) {
            ret = 4;
            break;
        }
    }

    return ret;
}

int test_sort() {
    int ret = 0;
    int n = 10;
    bft::bft_layer<int> layer(n);
    for (int i=n-1; i>=0; i--) {
        if (layer.add(i) != 0) {
            ret = 5;
            break;
        }

        if (layer.size() != i+1) {
            ret = 6;
            break;
        }
    }

    layer.sort();

    for (int i=0; i<n; i++) {
        int* cur = layer.get(i);
        if (*cur != i) {
            ret = 7;
            break;
        }
    }

    return ret;
}

int add_sequence(bft::bft_layer<int> l, int start, int end) {
    int ret;
    for (int i=start; i<=end; i++) {
        ret = l.add(i);
        if (ret != 0) {
            ret = 8;
            break;
        }
    }
    return ret;
}

int testClear() {
    bft::bft_layer<int> l = new bft::bft_layer<int>(10);
    int ret = add_sequence(l, 0, 9);
    if (ret != 0) {
        ret = 9;
    } else if (l.size() != 10) {
        ret = 10;
    }
    l.clear();
    if (l.size() != 0) {
        ret = 11;
    }
    return ret;
}


