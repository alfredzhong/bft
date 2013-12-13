
#ifndef BFT_BFT_H
#define BFT_BFT_H
// leave out next pointers now

#include <mutex>
#include <algorithm>
#include <vector>
#include <iostream>

#include <bft_layer.h>

#define DEFAULT_INIT_LAYERS 5


namespace bft {
    template <class K, class V>
    class bft_bft {

        private:
            std::vector<bft::bft_layer<K, V> > *layers;
            int cur_size; 
        public:
            bft_bft(int first_layer_size, int initial_layers) {
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(initial_layers);
                bft::bft_layer<K, V> first_layer(first_layer_size);
                layers->push_back(first_layer);
            }

            bft_bft() {
                layers = new std::vector<bft::bft_layer<K, V> >();
                layers->reserve(DEFAULT_INIT_LAYERS);
                bft::bft_layer<K, V> first_layer;
                layers->push_back(first_layer);
            }

            ~bft_bft() {
                cur_size = 0;
                delete layers;
            }
    };
}


#endif
