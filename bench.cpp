#include <benchmark/benchmark.h>

#include <vector>

static void BM_STRMQ(benchmark::State& state) {
  std::vector<uint64_t> vec(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    state.ResumeTiming();
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_STRMQ)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 22)
    ->Complexity();

static void BM_BinTreeRMQ(benchmark::State& state) {
  std::vector<uint64_t> vec(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    state.ResumeTiming();
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_BinTreeRMQ)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 22)
    ->Complexity();

BENCHMARK_MAIN();