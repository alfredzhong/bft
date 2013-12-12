all:bft_test



bft_test:bft_layer.h bft_layer_test.cpp main.cpp
	g++ -Wall -std=c++11 -mrtm -I. main.cpp bft_layer_test.cpp -o bft_test

clear:
	rm bft_test

