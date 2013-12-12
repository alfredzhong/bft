
#include <bft_layer_test.h>
#include <bft_layer.h>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>


//helpers
bool compare_int(bft::bft_node<int,int> i, bft::bft_node<int,int> j) {
    return i.key<j.key;
}

void print_error_code(int i) {
    std::cerr<<"error code: "<<i<<std::endl;
}

int add_sequence(bft::bft_layer<int, int> &l, int start, int end) {
    int ret = 0;
    for (int i=start; i<=end; i++) {
        bft::bft_node<int,int> node(i, 10*i);
        ret = l.add(node);
        if (ret != 0) {
            ret = 8;
            break;
        }
    }
    return ret;
}


// Tests
int bft_layer_test() {
    int ret = 0;
    if ( (ret = test_creation()) != 0) {
        print_error_code(ret);
    }

    if ( (ret=test_addition()) != 0) {
        print_error_code(ret);
    }

    if ( (ret=test_sort()) != 0) {
        print_error_code(ret);
    }

    if ( (ret=test_sort2()) != 0) {
        print_error_code(ret);
    }

    if ( (ret=test_clear()) != 0) {
        print_error_code(ret);
    }

    if ( (ret=test_find()) != 0) {
        print_error_code(ret);
    }

    if ( (ret=test_find2()) != 0) {
        print_error_code(ret);
    }

    return ret;
}

// test of creation layer object
int test_creation() {
    bft::bft_layer<int, int> layer_int();
    bft::bft_layer<std::string, int> layer_string();
    bft::bft_layer<std::vector<int>, int> layer_vector_int();
    return 0;
}

int test_addition() {
    int ret = 0;
    bft::bft_layer<int, int> layer(10);
    for (int i=0; i<10; i++) {
        bft::bft_node<int,int> node(i, 10*i);
        if (layer.add(node) != 0) {
            ret = 2;
            break;
        }
    }

    //layer if full
    for (int i=0; i<100; i++) {
        bft::bft_node<int,int> node(i, 10*i);
        if (layer.add(node) != -1) {
            ret = 3;
            break;
        }
    }

    //test of correctness
    for (int i=0; i<10; i++) {
        bft::bft_node<int, int>* cur = layer.get(i);
        if (cur->key != i || cur->value != 10*i) {
            ret = 4;
            return ret;
        }
    }

    return ret;
}

int test_sort() {
    int ret = 0;
    int n = 10;
    bft::bft_layer<int, int> layer(n, compare_int);
    for (int i=n-1; i>=0; i--) {
        bft::bft_node<int,int> node(i, 10*i);
        if (layer.add(node) != 0) {
            ret = 5;
            return ret;
        }

        if (layer.size() != 10-i) {
            ret = 6;
            return ret;
        }
    }

    if ( (ret = layer.sort()) != 0) {
        ret = 77;
        return ret;
    }

    for (int i=0; i<n; i++) {
        bft::bft_node<int, int>* cur = layer.get(i);
        if (cur->key != i || cur->value != 10*i) {
            ret = 8;
            return ret;
        }
    }

    return ret;
}


int test_sort2() {
    int ret = 0;
    int n = 10;
    bft::bft_layer<int, int> layer(n);
    for (int i=n-1; i>=0; i--) {
        bft::bft_node<int,int> node(i, 10*i);
        if (layer.add(node) != 0) {
            ret = 4;
            return ret;
        }

        if (layer.size() != 10-i) {
            ret = 41;
            return ret;
        }
    }

    if (layer.sort() != -1) {
        ret = 42;
        return ret;
    }

    layer.set_compare_func(&compare_int);

    if (layer.sort() != 0) {
        ret = 43;
        return ret;
    }


    for (int i=0; i<n; i++) {
        bft::bft_node<int, int>* cur = layer.get(i);
        if (cur->key != i || cur->value != 10*i) {
            ret = 44; 
            return ret;
        }
    }

    return ret;
}


int test_clear() {
    bft::bft_layer<int, int> l(10); 
    int ret = add_sequence(l, 0, 9);
    if (ret != 0) {
        ret = 5;
    } else if (l.size() != 10) {
        ret = 51;
    }
    l.clear();
    if (l.size() != 0) {
        ret = 52;
    }
    return ret;
}

int test_find() {
    int ret = 0;
    bft::bft_layer<int,int> layer(10, compare_int);
    add_sequence(layer, 0, 9);
    std::vector<bft::bft_node<int, int> > *results = layer.find(8);
    if (results->size() != 1) {
        ret = 6;
        return ret;
    }

    if (results->at(0).key != 8) {
        ret = 61;
        return ret;
    }

    if (results->at(0).value != 80) {
        ret = 62;
        return ret;
    }

    return ret;
}


int test_find2() {
    int ret = 0;
    bft::bft_layer<int,int> layer(100, compare_int);
    layer.add(bft::bft_node<int, int>(1,10));
    layer.add(bft::bft_node<int, int>(1,10));;
    layer.add(bft::bft_node<int, int>(2,20));;
    layer.add(bft::bft_node<int, int>(3,30));;
    layer.add(bft::bft_node<int, int>(4,40));;
    layer.add(bft::bft_node<int, int>(5,50));;
    layer.add(bft::bft_node<int, int>(5,50));;
    layer.add(bft::bft_node<int, int>(5,50));;
    layer.add(bft::bft_node<int, int>(5,50));;
    layer.add(bft::bft_node<int, int>(5,50));;
    std::vector<bft::bft_node<int, int> > *results = layer.find(5);
    if (results == NULL || results->size() != 5) {
        ret = 7;
        return ret;
    }

    results->clear();
    delete results;
    results = layer.find(1);
    if (results == NULL || results->size() != 2) {
        ret = 71;
        return ret;
    }

    results->clear();
    delete results;
    results = layer.find(2);
    if (results == NULL || results->size() != 1) {
        ret = 72;
        return ret;
    }


    return ret;
}

