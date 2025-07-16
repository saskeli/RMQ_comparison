
#include <iostream>
#include <random>
#include <vector>

#include "counters/counters.hpp"

const size_t runs = 100;

template <bool separate_queries, bool pipeline_flush>
uint64_t run(size_t n, size_t seed) {
  count::Counters<pipeline_flush, 1, count::Counter::instructions> counter;
  std::vector<uint64_t> a(n);
  std::vector<uint64_t> b(n);
  std::mt19937 gen(seed);
  std::uniform_int_distribution<uint32_t> uniform_dist;
  uint64_t checksum = 0;
  for (size_t epoc = 0; epoc < runs; ++epoc) {
    for (size_t i = 0; i < n; ++i) {
      a[i] = uniform_dist(gen);
      b[i] = uniform_dist(gen);
    }
    if constexpr (not separate_queries) {
      counter.reset();
    }
    for (size_t i = 0; i < n; ++i) {
      if constexpr (separate_queries) {
        counter.reset();
      }
      a[i] *= b[i];
      if constexpr (separate_queries) {
        counter.accumulate();
      }
    }
    if constexpr (not separate_queries) {
      counter.accumulate();
    }
    checksum += a[uniform_dist(gen) % n];
  }
  std::cout << "Separate queries: " << separate_queries << "\n"
            << "Pipeline flush: " << pipeline_flush << std::endl;
  counter.output_counters(0, runs * n);
  return checksum;
}

int main(int argc, char const* argv[]) {
  if (argc < 5) {
    std::cerr << "n, seed, separate_queries and pipeline_flush "
                 "are required."
              << std::endl;
    std::cerr << argv[0] << " <n> <seed> <0|1> <0|1>" << std::endl;
    exit(1);
  }
  size_t n = std::stoull(argv[1]);
  size_t seed = std::stoull(argv[2]);
  bool separate_queries = std::stoull(argv[3]) % 2 == 1;
  bool pipeline_flush = std::stoull(argv[4]) % 2 == 1;

  std::cout << "Running " << runs << " iterations of\n"
            << "\t" << n << " element structures with seed\n"
            << "\t" << seed << std::endl;
  uint64_t c;
  if (separate_queries) {
    if (pipeline_flush) {
      c = run<true, true>(n, seed);
    } else {
      c = run<true, false>(n, seed);
    }
  } else {
    if (pipeline_flush) {
      c = run<false, true>(n, seed);
    } else {
      c = run<false, false>(n, seed);
    }
  }

  std::cout << "Checksum " << c << std::endl;

  return 0;
}
