#include <bft_bft.h>
#include <bft_layer.h>
#include <string>
#include <bft_bft_test.h>


//tests
int bft_test() {
    int ret = 0;
    if ((ret = test_bft_creation()) != 0) {
        ret = 1;
        return ret;
    }

    if ((ret = test_bft_addition()) != 0) {
        ret = 1;
        return ret;
    }
    return ret;
}

int test_bft_creation() {
    int ret = 0;
    bft::bft_bft<int, int> tree1();
    bft::bft_bft<int, int> tree2(10, 3);
    bft::bft_bft<std::string, int> tree3(10, 3);
    bft::bft_bft<std::string, std::string> tree4(10, 3);
    return ret;
}


int test_bft_addition() {
    int ret = 0;
    bft::bft_bft<int, int> tree1(10, 10);
    for (int i=0; i<100; i++) {
        std::cout<<"i="<<i<<std::endl;
        bft::bft_node<int,int> cur_kv(i, i*100);
        tree1.add(cur_kv);
    }
    return ret;
}
