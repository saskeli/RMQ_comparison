#include <benchmark/benchmark.h>

#include <vector>

#include "include/seg_rmq.hpp"
#include "include/tree_rmq.hpp"
#include "include/inverted_tree_tmq.hpp"
#include "include/util.hpp"

static void BM_STRMQ(benchmark::State& state) {
  std::vector<uint64_t> vec(state.range(0));
  std::mt19937 gen(state.range(0));
  std::uniform_int_distribution<uint64_t> uniform_dist;
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = uniform_dist(gen);
  }
  Seq_RMQ rmq(vec);
  uint64_t checksum = 0;
  for (auto _ : state) {
    state.PauseTiming();
    uint64_t a = uniform_dist(gen) % vec.size();
    uint64_t b = uniform_dist(gen) % (vec.size() - a);
    b += a;
    state.ResumeTiming();
    checksum += rmq.minimum(a, b);
  }
  state.SetLabel(checksum & 1 ? "FIZZ" : "BUZZ");
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_STRMQ)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 27)
    ->Complexity();

static void BM_BinTreeRMQ(benchmark::State& state) {
  std::vector<uint64_t> vec(state.range(0));
  std::random_device r;
  std::mt19937 gen(r());
  std::uniform_int_distribution<uint64_t> uniform_dist;
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = uniform_dist(gen);
  }
  Tree_RMQ rmq(vec);
  uint64_t checksum = 0;
  for (auto _ : state) {
    state.PauseTiming();
    uint64_t a = uniform_dist(gen) % vec.size();
    uint64_t b = uniform_dist(gen) % (vec.size() - a);
    b += a;
    state.ResumeTiming();
    checksum += rmq.minimum(a, b);
  }
  state.SetLabel(checksum & 1 ? "FIZZ" : "BUZZ");
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_BinTreeRMQ)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 24)
    ->Complexity();

static void BM_InvTreeRMQ(benchmark::State& state) {
  std::vector<uint64_t> vec(state.range(0));
  std::random_device r;
  std::mt19937 gen(r());
  std::uniform_int_distribution<uint64_t> uniform_dist;
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = uniform_dist(gen);
  }
  Inv_Tree_RMQ rmq(vec);
  uint64_t checksum = 0;
  for (auto _ : state) {
    state.PauseTiming();
    uint64_t a = uniform_dist(gen) % vec.size();
    uint64_t b = uniform_dist(gen) % (vec.size() - a);
    b += a;
    state.ResumeTiming();
    checksum += rmq.minimum(a, b);
  }
  state.SetLabel(checksum & 1 ? "FIZZ" : "BUZZ");
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_InvTreeRMQ)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 24)
    ->Complexity();

BENCHMARK_MAIN();