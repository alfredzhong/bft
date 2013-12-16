
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
            bft_node(K key, V value) {
                this->key = key;
                this->value = value;
            }
    };

    template <class K, class V>
    class bft_layer {
        private:
            std::vector<bft_node<K,V> > *data;
            int *next;
            int capacity;
            bool (*compare_func)(bft::bft_node<K,V>, bft::bft_node<K,V>);

            std::mutex *fallback_mutex;
        public:
            bft_layer(int capacity) {
                this->capacity = capacity;
                data = new std::vector<bft::bft_node<K,V> >();
                data->reserve(capacity);
                compare_func = NULL;
                fallback_mutex = new std::mutex();
            }


            bft_layer(int capacity, bool (*compare_func)(bft::bft_node<K,V>, bft::bft_node<K,V>)) {
                this->capacity = capacity;
                data = new std::vector<bft::bft_node<K,V> >();
                data->reserve(capacity);
                this->compare_func = compare_func;
                fallback_mutex = new std::mutex();
            }


            bft_layer() {
                this->capacity = BFT_DEFAULT_LAYER_SIZE;
                data = new std::vector<bft::bft_node<K,V> >();
                data->reserve(this->capacity);
                compare_func = NULL;
            }

            ~bft_layer() {
                if (data != NULL) {
                    data->clear();
                    delete data;
                    data = NULL;
                } 
            }

            void set_compare_func(bool (*compare_func)(bft::bft_node<K,V>, bft::bft_node<K,V>)) {
                this->compare_func = compare_func;
            }

            // return 0, if add was successful
            // return -1 otherwise
            int add(bft::bft_node<K,V> t) {
                if (data->size() + 1> capacity) {
                    return -1;
                }
                data->push_back(t);
                return 0;
            }

            int size() {
                return data->size();
            }

            bft::bft_node<K,V>* get(int i) {
                if (i<0 || i >= data->size()) {
                    return NULL;
                }
                bft::bft_node<K,V>* ret = &(data->at(i));
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
            
            // need to overload < == > for key class K
            std::vector<bft::bft_node<K,V> >* binary_search(K key) {
                if (data->size() <= 0) return NULL;

                std::vector<bft::bft_node<K,V> >* ret 
                    = new std::vector<bft::bft_node<K,V> >();

                if (data->size() == 1) {
                    if (data->at(0).key == key) {
                        ret->push_back(data->at(0));
                        return ret;
                    } 
                    delete ret;
                    return NULL;
                }

                int n = data->size();
                int start = 0, end = data->size()-1;
                if (key < data->at(start).key 
                        || key > data->at(end).key) {
                    delete ret;
                    return NULL;
                }
                while (start<end) {
                    int mid = start + (end-start)/2;
                    if (data->at(mid).key == key) {
                        int i = mid;
                        while (i-1>=0 && data->at(i-1).key == key) {
                            i--;
                        }

                        int j = mid;
                        while (j+1<n && data->at(j+1).key == key) {
                            j++;
                        }

                        for (int k = i; k<= j; k++) {
                            ret->push_back(data->at(k)); 
                        }
                        break;
                    } else if (key < data->at(mid).key) {
                        end = mid - 1;
                    } else {
                        start = mid + 1;
                    }
                }

                if (ret->size() == 0) {
                    delete ret;
                    return NULL;
                }
                 
                return ret;
            }


            std::vector<bft::bft_node<K,V> >* find(K key) {
                return binary_search(key);
            }

            void merge_to(bft::bft_layer<K,V> *another_layer) {
                int n = data->size();
                int m = another_layer->size();
                for (int i=0; i<n; i++) {
                    another_layer->data.push_back(data->at(i));
                }
                int i = n-1, j = m-1, k = m + n-1;
                while (i>=0 && j>=0) {
                    //if (
                }
            }


    };



}


#endif
