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
int use_rtx = 1;

void addition_bench() {
        bft::bft_bft<int, int> *tree 
            = (bft::bft_bft<int, int> *)tree_ptr;
            for (int i=0; i<num_insertion_per_thread; i++) {
                //std::cout<<"i="<<i<<std::endl;
                bft::bft_node<int,int> cur_kv(i, i*100);
                try {
                    tree->add(cur_kv);
                } catch (std::out_of_range e) {
                    std::cout<<"out of range caught in add"<<std::endl;
                    exit(-1);
                }

                //std::cout<<tree->to_string();
                //std::cout<<"done add"<<std::endl;
            }

            std::cout<<"tree cleared"<<std::endl;
}



int main(int argc, char** argv) {
    if (argc == 5) {
        num_insertion_per_thread = atoi(argv[1]);
        num_threads = atoi(argv[2]);
        repeats = atoi(argv[3]);
        use_rtx = atoi(argv[4]);
    }
    std::cout<<"num_insertion_per_thread = "<<num_insertion_per_thread<<std::endl;
    std::cout<<"num_threads = "<<num_threads<<std::endl;
    std::cout<<"repeats = "<<repeats<<std::endl;

    std::thread *threads[num_threads];
    for (int i=0; i<num_threads; i++) {
        threads[i] = NULL;
    }

    if (use_rtx == 0) {
        tree.set_rtx(false);
    } else {
        tree.set_rtx(true);
    }


    for (int j=0; j<repeats; j++) {
        for (int i=0; i<num_threads; i++) {
            if (threads[i] != NULL) {
                delete threads[i];
            }
            threads[i] = new std::thread(addition_bench);
        }

        for (int i=0; i<num_threads; i++) {
            threads[i]->join();
        }

        tree.clear();
    }

    std::cout<<tree.to_string();

    return 0;
}

