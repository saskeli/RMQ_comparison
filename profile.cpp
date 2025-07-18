
#include <iostream>
#include <random>
#include <vector>

#include "counters/counters.hpp"
#include "include/inverted_tree_tmq.hpp"
#include "include/seg_rmq.hpp"
#include "include/tree_rmq.hpp"
#include "include/util.hpp"

const size_t runs = 100;

template <class RMQ_T, class counter_T, uint16_t section>
uint64_t run(size_t n, size_t seed, counter_T& counter, size_t n_q) {
  std::vector<uint64_t> vec(n);
  std::vector<uint64_t> a(n_q);
  std::vector<uint64_t> b(n_q);
  std::mt19937 gen(seed);
  std::uniform_int_distribution<uint64_t> uniform_dist;
  uint64_t checksum = 0;
  for (size_t epoc = 0; epoc < runs; ++epoc) {
    for (size_t i = 0; i < n; ++i) {
      vec[i] = uniform_dist(gen);
    }
    for (size_t i = 0; i < n_q; ++i) {
      uint64_t a_q = uniform_dist(gen) % n;
      uint64_t b_q = a_q + uniform_dist(gen) % (n - a_q);
      a[i] = a_q;
      b[i] = b_q;
    }
    RMQ_T rmq(vec);
    counter.reset();
    for (size_t i = 0; i < n_q; ++i) {
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
  size_t n_q = n;
  if (argc > 3) {
    n_q = std::stoull(argv[3]);
  }

  std::cout << "Running " << runs << " iterations of\n"
            << "\t" << n_q << " queries on\n"
            << "\t" << n << " element structures with seed\n"
            << "\t" << seed << std::endl;

  count::Default<3> counter;

  uint64_t a_c = run<Seq_RMQ, count::Default<3>, 0>(n, seed, counter, n_q);
  uint64_t b_c = run<Tree_RMQ, count::Default<3>, 1>(n, seed, counter, n_q);
  uint64_t c_c = run<Inv_Tree_RMQ, count::Default<3>, 2>(n, seed, counter, n_q);
  std::cout << "Checksums " << a_c << ", " << b_c << ", " << c_c << std::endl;

  std::cout << "\nSegment tree RMQ:" << std::endl;
  counter.output_counters(0, runs * n_q);

  std::cout << "\nPointer-based tree RMQ:" << std::endl;
  counter.output_counters(1, runs * n_q);

  std::cout << "\nPointer-based inverted tree RMQ:" << std::endl;
  counter.output_counters(2, runs * n_q);
  return 0;
}
