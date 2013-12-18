#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <bft_bft.h>
#include <bft_layer.h>


bool compare(bft::bft_node<int,int> i, bft::bft_node<int,int> j) {
    return i.key < j.key;
}


bft::bft_bft<int,int> tree(2, 10, compare); 
void *tree_ptr = (void*)&tree;
int num_insertion_per_thread = 20;
int num_threads = 10;
int repeats = 100;

void addition_bench() {
    bft::bft_bft<int, int> *tree 
        = (bft::bft_bft<int, int> *)tree_ptr;
    for (int j=0; j<repeats; j++) {
        for (int i=0; i<num_insertion_per_thread; i++) {
            //std::cout<<"i="<<i<<std::endl;
            bft::bft_node<int,int> cur_kv(i, i*100);
            tree->add(cur_kv);
            //std::cout<<tree->to_string();
            //std::cout<<"done add"<<std::endl;
        }
        
        //tree->clear();
    }
}



int main(int argc, char** argv) {
    if (argc == 4) {
        num_insertion_per_thread = atoi(argv[1]);
        num_threads = atoi(argv[2]);
        repeats = atoi(argv[3]);
    }
    std::cout<<"num_insertion_per_thread = "<<num_insertion_per_thread<<std::endl;
    std::cout<<"num_threads = "<<num_threads<<std::endl;
    std::cout<<"repeats = "<<repeats<<std::endl;

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

