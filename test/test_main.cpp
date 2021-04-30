#include "gtest/gtest.h"
#include "test_vector.h"
#include "test_binary_search_tree.h"
#include "test_red_black_tree.h"
#include "test_avl_tree.h"
#include "test_2_3_tree.h"
#include "test_2_3_4_tree.h"
#include "test_b_tree_32.h"
#include "test_map.h"
#include "test_multimap.h"
#include "test_set.h"
#include "test_multiset.h"


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}