
BENCH=-isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
CMAKE_OPT=-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release

HEADERS=include/seg_rmq.hpp include/tree_rmq.hpp include/util.hpp

DEFAULT: bench

bench: bench.cpp $(HEADERS) benchmark/build/lib/libgtest.a
	g++ bench.cpp -std=c++11 $(BENCH) -o bench

benchmark/build/lib:
	git submodule update --init

benchmark/build/lib/libgtest.a: | benchmark/build/lib
	mkdir -p benchmark/build
	(cd benchmark; cmake $(CMAKE_OPT) -S . -B "build")
	(cd benchmark; cmake --build "build" --config Release)

googletest:
	git submodule update --init

googletest/build/lib/libgtest_main.a: | $(GTEST_DIR)/googletest
	(mkdir -p $(GTEST_DIR)/build && cd $(GTEST_DIR)/build && cmake -DCMAKE_C_COMPILER="$(CC)" -DCMAKE_CXX_COMPILER="$(CXX)" -DBUILD_SHARED_LIBS=OFF .. && $(MAKE))

test/test: googletest/build/lib/libgtest_main.a test/test.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(GFLAGS) $(INCLUDE) $(TEST_PERF_FLAGS) test/test.cpp -o test/test -lgtest_main -lgtest -lrapidcheck $(LIBS)

test: test/test
	test/test $(ARG)
