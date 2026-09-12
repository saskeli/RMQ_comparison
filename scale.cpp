
#include <iostream>
#include <random>
#include <vector>

#include "counters/counters.hpp"
#include "include/inverted_tree_tmq.hpp"
#include "include/seg_rmq.hpp"
#include "include/tree_rmq.hpp"
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
      uint64_t a_q = uniform_dist(gen) % (size / 2);
      uint64_t b_q = a_q + size / 2 + uniform_dist(gen) % (size / 2 - a_q);
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
    std::cerr << "seed is required." << std::endl;
    std::cerr << argv[0] << " <seed>" << std::endl;
    exit(1);
  }
  size_t seed = std::stoull(argv[1]);

  count::Counters<true, 2, count::Counter::instructions,
                  count::Counter::L1D_miss, count::Counter::L1D_access,
                  count::Counter::LL_miss>
      counter;
  std::vector<size_t> sizes = {
      10,        16,        32,        64,         128,        256,
      512,       1024,      2048,      4096,       8192,       16384,
      32768,     65536,     131072,    262144,     524288,     1048576,
      2097152,   4194304,   8388608,   16777216,   33554432,   67108864,
      134217728, 268435456, 536870912, 1073741824, 2147483648, 3000000000};
  for (auto n : sizes) {
    uint64_t a_c;
    {
      a_c = run<Seq_RMQ, decltype(counter), 0>(n, seed, counter);
    }
    uint64_t c_c;
    {
      c_c = run<Inv_Tree_RMQ, decltype(counter), 1>(n, seed, counter);
    }
    std::cout << "size: " << n << ", Checksums " << a_c << ", " << c_c << std::endl;
    std::cout << "Segment tree RMQ:" << std::endl;
    counter.output_counters(0, runs * queries);
    std::cout << "Inverted pointer-based tree RMQ:" << std::endl;
    counter.output_counters(1, runs * queries);
    counter.clear();
  }

  return 0;
}
