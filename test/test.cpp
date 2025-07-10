#include <cstdint>
#include <iostream>
#include <vector>

#include "../googletest/googletest/include/gtest/gtest.h"
#include "../include/seg_rmq.hpp"
#include "../include/tree_rmq.hpp"
#include "../include/inverted_tree_tmq.hpp"
#include "../include/util.hpp"

uint64_t bf(std::vector<uint64_t>& vec, size_t a, size_t b) {
  uint64_t ret = vec[a];
  for (size_t i = a + 1; i <= b; ++i) {
    ret = std::min(ret, vec[i]);
  }
  return ret;
}

void validate(std::vector<uint64_t>& vec, Seq_RMQ& s_rmq, Tree_RMQ& t_rmq, Inv_Tree_RMQ& i_rmq) {
  for (size_t a = 0; a < vec.size(); ++a) {
    for (size_t b = a; b < vec.size(); ++b) {
      ASSERT_EQ(bf(vec, a, b), s_rmq.minimum(a, b)) << a << ", " << b;
      ASSERT_EQ(bf(vec, a, b), t_rmq.minimum(a, b)) << a << ", " << b;
      ASSERT_EQ(bf(vec, a, b), i_rmq.minimum(a, b)) << a << ", " << b;
    }
  }
}

void run_test(size_t size, size_t modifications, uint64_t s_lim, uint64_t seed_a,
              uint64_t seed_b) {
  std::vector<uint64_t> vec(size);
  populate_random(vec, seed_a);
  for (size_t i = 0; i < size; ++i) {
    vec[i] = vec[i] % s_lim;
  }
  Seq_RMQ s_rmq(vec);
  Tree_RMQ t_rmq(vec);
  Inv_Tree_RMQ i_rmq(vec);
  //std::cerr << t_rmq.to_string() << std::endl;
  validate(vec, s_rmq, t_rmq, i_rmq);
  std::vector<uint64_t> mod(modifications * 2);
  populate_random(mod, seed_b);
  for (size_t i = 0; i < mod.size(); i += 2) {
    size_t loc = mod[i] % vec.size();
    uint64_t v = mod[i + 1] % s_lim;
    vec[loc] = v;
    s_rmq.set(loc, v);
    t_rmq.set(loc, v);
    i_rmq.set(loc, v);
    validate(vec, s_rmq, t_rmq, i_rmq);
  }
}

TEST(SegRMQ, test) {
  std::vector<uint64_t> vec = {1, 5, 2, 6, 5};
  Seq_RMQ rmq(vec);
  for (size_t a = 0; a < vec.size(); ++a) {
    for (size_t b = a; b < vec.size(); ++b) {
      ASSERT_EQ(bf(vec, a, b), rmq.minimum(a, b)) << a << ", " << b;
    }
  }
  rmq.set(2, 4);
  vec[2] = 4;
  for (size_t a = 0; a < vec.size(); ++a) {
    for (size_t b = a; b < vec.size(); ++b) {
      ASSERT_EQ(bf(vec, a, b), rmq.minimum(a, b)) << a << ", " << b;
    }
  }
}

TEST(TreeRMQ, test) {
  std::vector<uint64_t> vec = {1, 5, 2, 6, 5};
  Tree_RMQ rmq(vec);
  for (size_t a = 0; a < vec.size(); ++a) {
    for (size_t b = a; b < vec.size(); ++b) {
      ASSERT_EQ(bf(vec, a, b), rmq.minimum(a, b)) << a << ", " << b;
    }
  }
  rmq.set(2, 4);
  vec[2] = 4;
  for (size_t a = 0; a < vec.size(); ++a) {
    for (size_t b = a; b < vec.size(); ++b) {
      ASSERT_EQ(bf(vec, a, b), rmq.minimum(a, b)) << a << ", " << b;
    }
  }
}

TEST(InvTreeRMQ, test) {
  std::vector<uint64_t> vec = {1, 5, 2, 6, 5};
  Inv_Tree_RMQ rmq(vec);
  for (size_t a = 0; a < vec.size(); ++a) {
    for (size_t b = a; b < vec.size(); ++b) {
      ASSERT_EQ(bf(vec, a, b), rmq.minimum(a, b)) << a << ", " << b;
    }
  }
  rmq.set(2, 4);
  vec[2] = 4;
  for (size_t a = 0; a < vec.size(); ++a) {
    for (size_t b = a; b < vec.size(); ++b) {
      ASSERT_EQ(bf(vec, a, b), rmq.minimum(a, b)) << a << ", " << b;
    }
  }
}

TEST(Integration, t1) { run_test(10, 1, 20, 1337, 69); }

TEST(Integration, t2) { run_test(11, 11, 100, 69, 1337); }

TEST(Integration, t3) { run_test(1000, 10, 50000, 123456, 101010101); }

TEST(Integration, t4) { run_test(1337, 10, 300000, 9999999, 10199999); }
