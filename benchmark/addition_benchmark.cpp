#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <bft_bft.h>
#include <bft_layer.h>

int num_threads;
bft::bft_bft<int,int> tree(2, 10); 
void *tree_ptr = (void*)&tree;

void addition_bench() {
    bft::bft_bft<int, int> *tree 
        = (bft::bft_bft<int, int> *)tree_ptr;
    for (int i=0; i<20; i++) {
        //std::cout<<"i="<<i<<std::endl;
        bft::bft_node<int,int> cur_kv(i, i*100);
        tree->add(cur_kv);
        //std::cout<<tree->to_string();
        //std::cout<<"done add"<<std::endl;
    }
    //return NULL;
}



int main(int argc, char** argv) {
    if (argc < 3) {
        return -1;
    }
    num_threads = atoi(argv[1]);
    std::thread *threads[num_threads];
    for (int i=0; i<num_threads; i++) {
        threads[i] = NULL;
    }

    int use_rtx = atoi(argv[2]);
    if (use_rtx == 0) {
        tree.set_rtx(false);
    } else {
        tree.set_rtx(true);
    }

    for (int i=0; i<num_threads; i++) {
        threads[i] = new std::thread(addition_bench);
    }

    for (int i=0; i<num_threads; i++) {
        threads[i]->join();
    }

    std::cout<<tree.to_string();

    return 0;
}

