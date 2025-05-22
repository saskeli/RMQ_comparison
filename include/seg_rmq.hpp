#pragma once

#include <stddef.h>

#include <cstdint>
#include <vector>

class Seq_RMQ {
 private:
  typedef std::vector<uint64_t> V_T;
  V_T& vec_;
  V_T tree_;

 public:
  Seq_RMQ(V_T& vec) : vec_(vec) {
    size_t tree_size = 64 - __builtin_clzll(vec.size());
    tree_ = V_T(tree_size, UINT64_MAX);
    for (size_t i = 1; i < vec_.size(); i += 2) {
      size_t parent = (i + tree_size) / 2;
      tree_[parent] = std::min(vec_[i - 1], vec_[i]);
    }
    if (vec_.size() % 2) {
      size_t parent = (tree_size + vec_.size() - 1) / 2;
      tree_[parent] = vec_[vec_.size() - 1];
    }
    for (size_t i = (tree_size - 1) / 2; i > 0; --i) {
      tree_[i] = std::min(tree_[i * 2], tree_[i * 2 + 1]);
    }
  }

  uint64_t minimum(size_t a, size_t b) const {
    uint64_t min = std::min(vec_[a], vec_[b]);
    a += tree_.size();
    b += tree_.size();
    a /= 2;
    b /= 2;
    while (a <= b) {
      if (a % 2 == 1) min = std::min(min, tree_[a++]);
      if (b % 2 == 0) min = std::min(min, tree_[b--]);
      a /= 2;
      b /= 2;
    }
    return min;
  }

  uint64_t set(size_t i, uint64_t v) {
    vec_[i] = v;
    size_t sibling = i % 2 == 1 ? i - 1 : i + 1;
    size_t parent = (i + tree_.size()) / 2;
    tree_[parent] = v;
    if (sibling < vec_.size() && vec_[sibling] < v) {
      tree_[parent] = vec_[sibling];
    }
    for (; parent > 0; parent /= 2) {
      tree_[parent] = std::min(tree_[parent * 2], tree_[parent * 2 + 1]);
    }
  }
};