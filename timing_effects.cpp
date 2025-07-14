
#include <iostream>
#include <random>
#include <vector>

#include "counters/counters.hpp"
#include "include/seg_rmq.hpp"
#include "include/util.hpp"

const size_t runs = 100;

template <bool separate_queries, bool pipeline_flush>
uint64_t run(size_t n, size_t seed, size_t n_q) {
  count::Counters<pipeline_flush, 1, count::Counter::instructions> counter;
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
    Seq_RMQ rmq(vec);
    if constexpr (not separate_queries) {
      counter.reset();
    }
    for (size_t i = 0; i < n_q; ++i) {
      if constexpr (separate_queries) {
        counter.reset();
      }
      checksum += rmq.minimum(a[i], b[i]);
      if constexpr (separate_queries) {
        counter.accumulate();
      }
    }
    if constexpr (not separate_queries) {
      counter.accumulate();
    }
  }
  std::cout << "Separate queries: " << separate_queries << "\n"
            << "Pipeline flush: " << pipeline_flush << std::endl;
  counter.output_counters(0);
  return checksum;
}

int main(int argc, char const* argv[]) {
  /*if (argc < 6) {
    std::cerr << "n, seed, query_count, separate_queries and pipeline_flush are
  required." << std::endl; std::cerr << argv[0] << " <n> <seed> <query_count>
  <0|1> <0|1>" << std::endl; exit(1);
  }*/
  size_t n = std::stoull(argv[1]);
  size_t seed = std::stoull(argv[2]);
  size_t n_q = std::stoull(argv[3]);
  /*bool separate_queries = std::stoull(argv[4]) % 2 == 1;
  bool pipeline_flush = std::stoull(argv[5]) % 2 == 1;

  std::cout << "Running " << runs << " iterations of\n"
            << "\t" << n_q << " queries on\n"
            << "\t" << n << " element structures with seed\n"
            << "\t" << seed << std::endl;
*/
  uint64_t c;
  /*if (separate_queries) {
    if (pipeline_flush) {*/
  c = run<true, true>(n, seed, n_q);
  /*} else {
    c = run<true, false>(n, seed, n_q);
  }
} else {
  if (pipeline_flush) {
    c = run<false, true>(n, seed, n_q);
  } else {
    c = run<false, false>(n, seed, n_q);
  }
}*/

  std::cout << "Checksum " << c << std::endl;

  return 0;
}
