#pragma once

#include <stddef.h>

#include <cstdint>
#include <string>
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
        delete (left);
        delete (right);
      }
    }

    bool has_left() const { return left != nullptr; }

    bool has_right() const { return left != nullptr; }

    uint64_t minimum(size_t a, size_t b) const {
      //std::cerr << "node " << value << ", " << size << " min " << a << ", " << b << std::endl;
      if (left == nullptr) {
        //std::cerr << " -> " << value << std::endl;
        return value;
      }
      if (a == 0 && b == size - 1) {
        //std::cerr << " -> " << value << std::endl;
        return value;
      }
      size_t left_s = left->size;
      if (a < left_s) {
        if (b < left_s) {
          //std::cerr << " -> left" << std::endl;
          return left->minimum(a, b);
        }
        //std::cerr << " -> both" << std::endl;
        return std::min(left->minimum(a, left_s - 1),
                        right->minimum(0, b - left_s));
      }
      //std::cerr << " -> right" << std::endl;
      return right->minimum(a - left_s, b - left_s);
    }

    void set(size_t i, uint64_t v) {
      if (left == nullptr) {
        value = v;
        return;
      }
      size_t left_s = left->size;
      if (left_s > i) {
        left->set(i, v);
      } else {
        right->set(i - left_s, v);
      }
      value = std::min(left->value, right->value);
    }

    void to_string(std::string& ret, std::string& indent) const {
      ret.append(indent)
          .append(std::to_string(value))
          .append(", ")
          .append(std::to_string(size))
          .append(", ")
          .append(std::to_string(reinterpret_cast<uint64_t>((left))))
          .append(", ")
          .append(std::to_string(reinterpret_cast<uint64_t>((right))))
          .push_back('\n');
      if (left != nullptr) {
        indent.append("l ");
        left->to_string(ret, indent);
        indent.pop_back();
        indent.pop_back();
      }
      if (right != nullptr) {
        indent.append("r ");
        right->to_string(ret, indent);
        indent.pop_back();
        indent.pop_back();
      }
    }
  };

  node* root;

 public:
  Tree_RMQ(V_T& vec) { root = new node(vec, 0, vec.size() - 1); }

  ~Tree_RMQ() {
    if (root != nullptr) {
      delete (root);
    }
  }

  uint64_t minimum(size_t a, size_t b) const {
    //std::cerr << "minimum " << a << ", " << b << std::endl;
    return root->minimum(a, b);
  }

  void set(size_t i, uint64_t v) { root->set(i, v); }

  std::string to_string() const {
    std::string ret = "tree:\n";
    std::string indent = "";
    if (root != nullptr) {
      root->to_string(ret, indent);
    }
    return ret;
  }
};