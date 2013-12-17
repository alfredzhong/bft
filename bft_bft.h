
#ifndef BFT_BFT_H
#define BFT_BFT_H
// leave out next pointers now

#include <mutex>
#include <algorithm>
#include <vector>
#include <iostream>
#include <immintrin.h>

#include <bft_layer.h>

#define DEFAULT_INIT_LAYERS 5


namespace bft {
    template <class K, class V>
    class bft_bft {

        private:
            std::vector<bft::bft_layer<K, V> > *layers = NULL;
            int cur_size; 
            std::mutex *fallback_mutex;
            int first_layer_capacity;
        public:
            bft_bft(int first_layer_capacity, int initial_layers) {
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(initial_layers);
                for (int i=0; i<initial_layers; i++) {
                    bft::bft_layer<K, V>* layer 
                        = new bft::bft_layer<K, V>(pow(2, i) * first_layer_capacity);
                    layers->push_back(*layer);
                }
                fallback_mutex = new std::mutex();
                this->first_layer_capacity = first_layer_capacity;
            }

            bft_bft() {
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(DEFAULT_INIT_LAYERS);
                bft::bft_layer<K, V>* first_layer 
                    = new bft::bft_layer<K, V>(first_layer_capacity);
                layers->push_back(first_layer);
                fallback_mutex = new std::mutex();
                first_layer_capacity = BFT_DEFAULT_LAYER_SIZE;
            }

            ~bft_bft() {
                cur_size = 0;
                delete layers;
            }

            // thread safe size()
            int size() {
                int ret = 0;
                if (_xbegin() == -1) {
                    ret = cur_size;
                    _xend();
                } else {
                    fallback_mutex->lock();
                    ret = cur_size;
                    fallback_mutex->unlock();
                }
                return ret;
            }

            int add(bft::bft_node<K, V> node) {
                int ret = 0;
                if (_xbegin() == -1) {
                    if (layers->at(0).size() + 1 <= first_layer_capacity) {
                        if (layers->at(0).add(node) != 0) {
                            ret = -1;
                            _xend();
                            return ret;
                        }
                        if (layers->at(0).size() == first_layer_capacity) {
                            layers->at(0).sort();
                            int i = 0, j = 1;
                            while (true) {
                                if (layers->size() == i+1) {
                                    int new_layer_capacity = pow(2,i+1)*first_layer_capacity;
                                    bft::bft_layer<K, V>* next_layer 
                                        = new bft::bft_layer<K, V>(new_layer_capacity);
                                    layers->push_back(*next_layer);
                                }
                                layers->at(i).merge_to(&layers->at(j));
                                if (layers->at(j).size() < pow(2,j) * first_layer_capacity) {
                                    break;
                                }
                                i++; j++;
                            }
                        }

                        _xend();
                        return ret;
                    }
                } else {
                    std::cout<<"bft_bft::add(): txn fail"<<std::endl;
                }
                return 0;
            }

            std::string to_string() {
                return "";
            }

    };
}


#endif
