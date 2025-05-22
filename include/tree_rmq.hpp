#pragma once

#include <stddef.h>

#include <cstdint>
#include <vector>

class Tree_RMQ {
 private:
  typedef std::vector<uint64_t> V_T;
  class node {
   public:
    uint64_t value;
    size_t size;
    node* left = nullptr;
    node* right = nullptr;

    node(V_T& vec, size_t a, size_t b) {
      if (a == b) {
        value = vec[a];
        size = 1;
      } else {
        size_t mid = (a + b) / 2;
        left = new node(vec, a, mid);
        right = new node(vec, mid + 1, b);
        value = std::min(left->value, right->value);
        size = left->size + right->size;
      }
    }

    ~node() {
      if (left != nullptr) {
        left->~node();
        delete (left);
        right->~node();
        delete (right);
      }
    }

    uint64_t minimum(size_t a, size_t b) const {
      if (a == 0 && b == size) {
        return value;
      }
      size_t left_s = left->size;
      if (b < left_s) {
        return left->minimum(a, b);
      }
      if (a >= left_s) {
        return right->minimum(a - left_s, b - left_s);
      }
      return std::min(left->minimum(a, left_s), right->minimum(0, b - left_s));
    }

    void set(size_t i, uint64_t v) {
      if (left == nullptr) {
        value = v;
        return;
      }
      size_t left_s = left->size;
      if (left->size) {

      }
    }
  };

  node* root;

 public:
  Tree_RMQ(V_T& vec) { root = new node(vec, 0, vec.size() - 1); }

  ~Tree_RMQ() {
    if (root != nullptr) {
      root->~node();
      delete (root);
    }
  }

  uint64_t minimum(size_t a, size_t b) const { return root->minimum(a, b); }

  void set(size_t i, uint64_t v) { root->set(i, v); }
};