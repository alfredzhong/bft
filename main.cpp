
#include <bft_layer.h>
#include <bft_layer_test.h>
#include <iostream>

int main() {
    int ret = 0;
    ret = bft_layer_test(); 
    if (ret==0) {
        std::cout<<"Pass!"<<std::endl;
    }

    return 0;
}

