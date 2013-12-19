all: debug release
debug:addition_benchmark_debug 
release:addition_benchmark

src:= addition_benchmark.cpp 
headers:=../src/bft_layer.h ../src/bft_bft.h

addition_benchmark_debug:${src} ${headers}
	g++ -g -Wall -std=c++11 -mrtm -I. -I../src ${src} -lpthread -o addition_benchmark_debug

addition_benchmark:${src} ${headers}
	g++ -Wall -std=c++11 -mrtm -I. -I../src ${src} -lpthread -o addition_benchmark

#benchrun:benchrun.cpp
#	g++ -Wall -std=c++11 benchrun.cpp -o benchrun




clear:
	rm addition_benchmark addition_benchmark_debug

