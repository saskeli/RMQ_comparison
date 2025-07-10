
#include <iostream>
#include <random>
#include <vector>

#include "counters/counters.hpp"
#include "include/seg_rmq.hpp"
#include "include/tree_rmq.hpp"
#include "include/inverted_tree_tmq.hpp"
#include "include/util.hpp"

const size_t runs = 10;
const size_t queries = 100000;

template <class RMQ_T, class counter_T, uint16_t section>
uint64_t run(size_t size, size_t seed, counter_T& counter) {
  std::vector<uint64_t> vec(size);
  std::vector<uint64_t> a(queries);
  std::vector<uint64_t> b(queries);
  std::mt19937 gen(seed);
  std::uniform_int_distribution<uint64_t> uniform_dist;
  uint64_t checksum = 0;
  for (size_t epoc = 0; epoc < runs; ++epoc) {
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = uniform_dist(gen);
    }   
    for (size_t i = 0; i < a.size(); ++i) {
      uint64_t a_q = uniform_dist(gen) % size;
      uint64_t b_q = a_q + uniform_dist(gen) % (size - a_q);
      a[i] = a_q;
      b[i] = b_q;
    }
    RMQ_T rmq(vec);
    counter.reset();
    for (size_t i = 0; i < a.size(); ++i) {
      checksum += rmq.minimum(a[i], b[i]);
    }
    counter.template accumulate<section>();
  }
  return checksum;
}

int main(int argc, char const* argv[]) {
  if (argc < 3) {
    std::cerr << "n and seed are required." << std::endl;
    std::cerr << argv[0] << " <n> <seed>" << std::endl;
    exit(1);
  }
  size_t n = std::stoull(argv[1]);
  size_t seed = std::stoull(argv[2]);

  count::Counters<3, count::Counter::instructions, count::Counter::L1D_miss, count::Counter::LL_miss>
      counter;

  for (size_t size = 10; size <= n; size *= 2) {
    uint64_t a_c;
    {
      a_c = run<Seq_RMQ, decltype(counter), 0>(size, seed, counter);
    }
    uint64_t b_c; 
    {
      b_c = run<Tree_RMQ, decltype(counter), 1>(size, seed, counter);
    }
    uint64_t c_c;
    { 
      c_c = run<Inv_Tree_RMQ, decltype(counter), 2>(size, seed, counter);
    }
    std::cout << "size: " << size << ", Checksums " << a_c << ", " << b_c
              << ", " << c_c << std::endl;
    std::cout << "Segment tree RMQ:" << std::endl;
    counter.output_counters(0, runs * queries);
    std::cout << "Pointer-based tree RMQ:" << std::endl;
    counter.output_counters(1, runs * queries);
    std::cout << "Inverted pointer-based tree RMQ:" << std::endl;
    counter.output_counters(2, runs * queries);
    counter.clear();
  }

  return 0;
}
