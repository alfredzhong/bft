all:bft_test

src:=bft_layer_test.cpp main.cpp bft_bft_test.cpp 
headers:=bft_layer.h bft_bft_test.h bft_bft.h

bft_test:${src} ${headers}
	g++ -Wall -std=c++11 -mrtm -I. ${src} -o bft_test

clear:
	rm bft_test

