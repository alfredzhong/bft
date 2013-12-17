all:addition_benchmark

src:= addition_benchmark.cpp 
headers:=../src/bft_layer.h ../src/bft_bft.h

addition_benchmark:${src} ${headers}
	g++ -g -Wall -std=c++11 -mrtm -I. -I../src ${src} -lpthread -o addition_benchmark

clear:
	rm addition_benchmark

