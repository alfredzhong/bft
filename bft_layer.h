
#ifndef BFT_LAYER_H
#define BFT_LAYER_H
// leave out next pointers now

#include <mutex>
#include <algorithm>
#include <vector>
#include <iostream>

#define BFT_DEFAULT_LAYER_SIZE 1024

namespace bft {

    template <class T>
    class bft_layer {
        private:
            std::vector<T> *data;
            int *next;
            int capacity;

            std::mutex fallback_mutax;
        public:
            bft_layer(int capacity) {
                this->capacity = capacity;
                data = new std::vector<T>();
                data->reserve(capacity);
            }


            bft_layer() {
                this->capacity = BFT_DEFAULT_LAYER_SIZE;
                data = new std::vector<T>();
                data->reserve(this->capacity);
            }

            ~bft_layer() {
                data->clear();
                delete data;
            }

            // return 0, if add was successful
            // return -1 otherwise
            int add(T t) {
                if (data->size() + 1> capacity) {
                    return -1;
                }
                data->push_back(t);
                return 0;
            }

            int size() {
                return data->size();
            }

            T* get(int i) {
                if (i<0 || i >= data->size()) {
                    return NULL;
                }
                T* ret = &(data->at(i));
                return ret;
            }

            void clear() {
                data->clear();
            }

            void sort() {
                if (data->size() <= 1) return;
                std::sort(data->begin(), data->end());
            }


            
    };



}


#endif
