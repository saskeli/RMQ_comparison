#pragma once

#include <stddef.h>

#include <cstdint>
#include <string>
#include <vector>

class Inv_Tree_RMQ {
 private:
  typedef std::vector<uint64_t> V_T;
  class node {
   public:
    uint64_t value;
    node* parent = nullptr;
    node* left_sibling = nullptr;
    node* right_sibling = nullptr;
    bool is_left = false;

    node(uint64_t v) : value(v) {}
  };

  std::vector<node*> leaves_;

 public:
  Inv_Tree_RMQ(V_T& vec) {
    size_t tree_size_ = 64 - __builtin_clzll(vec.size());
    tree_size_ = uint64_t(1) << tree_size_;
    leaves_ = std::vector<node*>(tree_size_, nullptr);

    std::vector<node*> a_vec;
    std::vector<node*> b_vec;
    for (size_t i = 0; i < tree_size_; ++i) {
      uint64_t v = i >= vec.size() ? UINT64_MAX : vec[i];
      leaves_[i] = new node(v);
      a_vec.push_back(leaves_[i]);
    }

    while (a_vec.size() > 1) {
      a_vec[0]->right_sibling = a_vec[1];
      for (size_t i = 1; i < a_vec.size() - 1; ++i) {
        a_vec[i]->left_sibling = a_vec[i - 1];
        a_vec[i]->right_sibling = a_vec[i + 1];
      }
      a_vec[a_vec.size() - 1]->left_sibling = a_vec[a_vec.size() - 2];
      for (size_t i = 0; i < a_vec.size(); i += 2) {
        node* p = new node(std::min(a_vec[i]->value, a_vec[i + 1]->value));
        a_vec[i]->parent = p;
        a_vec[i]->is_left = true;
        a_vec[i + 1]->parent = p;
        b_vec.push_back(p);
      }
      std::swap(a_vec, b_vec);
      b_vec.clear();
    }
  }

  ~Inv_Tree_RMQ() {
    std::vector<node*> p_vec;
    while (leaves_.size()) {
      for (size_t i = 0; i < leaves_.size(); ++i) {
        if (i % 2) {
          p_vec.push_back(leaves_[i]->parent);
        }
        delete leaves_[i];
      }
      std::swap(p_vec, leaves_);
      p_vec.clear();
    }
  }

  uint64_t minimum(size_t a, size_t b) const {
    uint64_t min = UINT64_MAX;
    node* a_ptr = leaves_[a];
    node* b_ptr = leaves_[b];
    while (a_ptr != b_ptr) {
      if (a_ptr == nullptr or b_ptr == nullptr or b_ptr->right_sibling == a_ptr)
          [[unlikely]] {
        return min;
      }
      min = std::min(min, a_ptr->value);
      bool is_left = a_ptr->is_left;
      a_ptr = reinterpret_cast<node*>(
          reinterpret_cast<uint64_t>(a_ptr->parent) * is_left +
          reinterpret_cast<uint64_t>(a_ptr->parent->right_sibling) *
              (!is_left));
      min = std::min(min, b_ptr->value);
      is_left = b_ptr->is_left;
      b_ptr = reinterpret_cast<node*>(
          reinterpret_cast<uint64_t>(b_ptr->parent->left_sibling) * is_left +
          reinterpret_cast<uint64_t>(b_ptr->parent) * (!is_left));
    }
    return std::min(min, a_ptr->value);
  }

  void set(size_t i, uint64_t v) {
    node* n_ptr = leaves_[i];
    n_ptr->value = v;
    while (n_ptr->parent != nullptr) {
      uint64_t s_v = n_ptr->is_left ? n_ptr->right_sibling->value
                                    : n_ptr->left_sibling->value;
      n_ptr->parent->value = std::min(s_v, n_ptr->value);
      n_ptr = n_ptr->parent;
    }
  }
};