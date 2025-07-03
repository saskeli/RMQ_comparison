
BENCH=-isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
CMAKE_OPT=-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release

HEADERS=include/seg_rmq.hpp include/tree_rmq.hpp include/util.hpp

CFLAGS=-std=c++23 -Wall -Wextra -Wshadow -pedantic -march=native

GFLAGS = -lpthread -DGTEST_ON -isystem googletest/googletest/include -pthread -L googletest/build/lib

DEFAULT: bench

bench: bench.cpp $(HEADERS) benchmark/build/lib/libgtest.a
	g++ $(CFLAGS) -Ofast bench.cpp $(BENCH) -o bench

profile: profile.cpp $(HEADERS) counters/counters.hpp
	g++ $(CFLAGS) -Ofast profile.cpp -o profile 

benchmark/build/lib:
	git submodule update --init

benchmark/build/lib/libgtest.a: | benchmark/build/lib
	mkdir -p benchmark/build
	(cd benchmark; cmake $(CMAKE_OPT) -S . -B "build")
	(cd benchmark; cmake --build "build" --config Release)

googletest/googletest:
	git submodule update --init

googletest/build/lib/libgtest_main.a: | googletest/googletest
	(mkdir -p googletest/build && cd googletest/build && cmake .. && make)

test/test: googletest/build/lib/libgtest_main.a test/test.cpp $(HEADERS)
	g++ $(CFLAGS) $(GFLAGS) -g test/test.cpp -o test/test -lgtest_main -lgtest

test: test/test
	test/test $(ARG)

counters/counters.hpp: 
	git submodule update --init