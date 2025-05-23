#include <cstdint>
#include <random>

template <class V_T>
void populate_random(V_T& vec, uint64_t seed = 0) {
  std::random_device r;
  std::mt19937 gen(seed ? seed : r());
  std::uniform_int_distribution<uint64_t> uniform_dist;
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = uniform_dist(gen);
  }
}
