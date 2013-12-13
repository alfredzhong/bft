
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
            std::vector<bft::bft_layer<K, V> > *layers;
            int cur_size; 
            std::mutex *fallback_mutex;
        public:
            bft_bft(int first_layer_size, int initial_layers) {
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(initial_layers);
                bft::bft_layer<K, V> first_layer(first_layer_size);
                layers->push_back(first_layer);
                fallback_mutex = new std::mutex();
            }

            bft_bft() {
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(DEFAULT_INIT_LAYERS);
                bft::bft_layer<K, V> first_layer;
                layers->push_back(first_layer);
                fallback_mutex = new std::mutex();
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
                return 0;
            }

    };
}


#endif
