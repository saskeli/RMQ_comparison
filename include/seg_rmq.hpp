#pragma once

#include <stddef.h>

#include <cstdint>
#include <vector>

class Seq_RMQ {
 private:
  typedef std::vector<uint64_t> V_T;
  uint64_t tree_size_;
  V_T tree_;

 public:
  Seq_RMQ(V_T& vec) {
    tree_size_ = 64 - __builtin_clzll(vec.size());
    tree_size_ = uint64_t(1) << tree_size_;
    tree_ = V_T(tree_size_ * 2, UINT64_MAX);
    
    for (size_t i = 0; i < vec.size(); ++i) {
      tree_[i + tree_size_] = vec[i];
    }
    for (size_t i = tree_size_ - 1; i > 0; --i) {
      tree_[i] = std::min(tree_[i * 2], tree_[i * 2 + 1]);
    }
  }

  uint64_t minimum(size_t a, size_t b) const {
    uint64_t min = UINT64_MAX;
    a += tree_size_;
    b += tree_size_;
    while (a <= b) {
      min = std::min(min, tree_[a]);
      min = std::min(min, tree_[b]);
      a += a % 2;
      b -= not bool(b % 2);
      a /= 2;
      b /= 2;
    }
    return min;
  }

  void set(size_t i, uint64_t v) {
    i += tree_size_;
    tree_[i] = v;
    for (uint64_t parent = i / 2; parent > 0; parent /= 2) {
      tree_[parent] = std::min(tree_[parent * 2], tree_[parent * 2 + 1]);
    }
  }
};