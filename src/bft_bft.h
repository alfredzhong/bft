
#ifndef BFT_BFT_H
#define BFT_BFT_H
// leave out next pointers now

#include <mutex>
#include <algorithm>
#include <vector>
#include <iostream>
#include <immintrin.h>

#include <bft_layer.h>
#include <stdio.h>

#define DEFAULT_INIT_LAYERS 5


namespace bft {
    template <class K, class V>
    class bft_bft {

        private:
            std::vector<bft::bft_layer<K, V> > *layers = NULL;
            int cur_size; 
            std::mutex *fallback_mutex;
            int first_layer_capacity;
            bool use_rtx = true;
        public:
            bft_bft(int first_layer_capacity, int initial_layers) {
                cur_size = 0;
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(initial_layers);
                for (int i=0; i<initial_layers; i++) {
                    bft::bft_layer<K, V>* layer 
                        = new bft::bft_layer<K, V>(pow(2, i) * first_layer_capacity);
                    layers->push_back(*layer);
                }
                fallback_mutex = new std::mutex();
                this->first_layer_capacity = first_layer_capacity;
                use_rtx = true;
            }

            bft_bft() {
                cur_size = 0;
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(DEFAULT_INIT_LAYERS);
                bft::bft_layer<K, V>* first_layer 
                    = new bft::bft_layer<K, V>(first_layer_capacity);
                layers->push_back(first_layer);
                fallback_mutex = new std::mutex();
                first_layer_capacity = BFT_DEFAULT_LAYER_SIZE;
                use_rtx = true;
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
                int start_txn;
                if (use_rtx) {
                    start_txn = _xbegin();
                } else {
                    start_txn = 999;
                }
                if (start_txn == -1) {
                    if (layers->at(0).size() + 1 <= first_layer_capacity) {
                        if (layers->at(0).add(node) != 0) {
                            ret = -1;
                            _xend();
                            return ret;
                        }
                        cur_size ++;
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
                    std::cout<<"bft_bft::add(): txn fail, use fallback mutex"<<std::endl;
                    fflush(stdout);

                    layers->at(0).lock();
                    std::cout<<"layer 0 lock"<<std::endl;
                    fflush(stdout);

                    if (layers->at(0).size() + 1 <= first_layer_capacity) {
                        if (layers->at(0).add(node) != 0) {
                            ret = -1;
                            layers->at(0).unlock();
                            std::cout<<"layer 0 unlock"<<std::endl;
                            fflush(stdout);
                            return ret;
                        } 
                        cur_size ++;
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
                                layers->at(j).lock();
                                std::cout<<"layer "<<j<<" lock"<<std::endl;
                                fflush(stdout);
                                std::cout<<"to merge"<<std::endl;
                                layers->at(i).merge_to(&layers->at(j));
                                fflush(stdout);
                                layers->at(i).unlock();
                                std::cout<<"layer "<<i<<" unlock"<<std::endl;
                                if (layers->at(j).size() < pow(2,j) * first_layer_capacity) {
                                    fflush(stdout);
                                    layers->at(j).unlock();
                                    std::cout<<"layer "<<j<<" unlock"<<std::endl;
                                    break;
                                }
                                i++; j++;
                            }
                        } else {
                            layers->at(0).unlock();
                            std::cout<<"layer 0 unlock"<<std::endl;
                        }

                        return ret;
                    } 

                    layers->at(0).unlock();
                    std::cout<<"error, when adding, layer 0 has no space"<<std::endl;
                }
                return 0;
            }

            void set_rtx(bool use_rtx) {
                this->use_rtx = use_rtx;
            }

            void clear() {
                int start_txn;
                if (use_rtx) {
                    start_txn = _xbegin();
                } else {
                    start_txn = 999;
                }
                if (start_txn == -1) {
                    int n = layers->size();
                    for (int i=0; i<n; i++) {
                        layers->at(i).clear();
                    }
                    cur_size = 0;
                    _xend();
                } else {
                    fallback_mutex->lock();
                    int n = layers->size();
                    for (int i=0; i<n; i++) {
                        layers->at(i).clear();
                    }
                    cur_size = 0;
                    fallback_mutex->unlock();
                }
            }


            std::string to_string() {
                if (layers ==  NULL || layers->size() == 0) {
                    return "<empty layers>";
                }
                std::string ret = "";
                int n = layers->size();
                for (int i=0; i<n; i++) {
                    ret += layers->at(i).to_string();
                }
                ret += "\n";
                return ret;
            }

    };
}


#endif
