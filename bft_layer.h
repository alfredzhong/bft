
#ifndef BFT_LAYER_H
#define BFT_LAYER_H
// leave out next pointers now

#include <mutex>
#include <algorithm>
#include <vector>
#include <iostream>

#define BFT_DEFAULT_LAYER_SIZE 1024

namespace bft {

    template<class K, class V>
    class bft_node {
        public:
            K key;
            V value;
    };



    template <class T>
    class bft_layer {
        private:
            std::vector<T> *data;
            int *next;
            int capacity;
            bool (*compare_func)(T, T);

            std::mutex fallback_mutax;
        public:
            bft_layer(int capacity) {
                this->capacity = capacity;
                data = new std::vector<T>();
                data->reserve(capacity);
                compare_func = NULL;
            }


            bft_layer(int capacity, bool (*compare_func)(T, T)) {
                this->capacity = capacity;
                data = new std::vector<T>();
                data->reserve(capacity);
                this->compare_func = compare_func;
            }


            bft_layer() {
                this->capacity = BFT_DEFAULT_LAYER_SIZE;
                data = new std::vector<T>();
                data->reserve(this->capacity);
                compare_func = NULL;
            }

            ~bft_layer() {
                data->clear();
                delete data;
            }

            void set_compare_func(bool (*compare_func)(T, T)) {
                this->compare_func = compare_func;
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

            int sort() {
                if (data->size() <= 1) return 0;
                if (compare_func==NULL) return -1;
                std::sort(data->begin(), data->end(), compare_func);
                return 0;
            }



            
    };



}


#endif
