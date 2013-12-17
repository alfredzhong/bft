
#include <bft_bft.h>
#include <bft_bft_test.h>
#include <bft_layer.h>
#include <bft_layer_test.h>
#include <iostream>

int main() {
    int ret = 0;
    ret = bft_layer_test(); 
    if (ret==0) {
        std::cout<<"Layer Tests Pass!"<<std::endl;
    }

    ret = bft_test();
    if (ret != 0) {
        print_error_code(ret);
    } else {
        std::cout<<"BFT Tests Pass!"<<std::endl;
    }

    return 0;
}

