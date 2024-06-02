/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#include "mocktests_trb_tree.h"


#define TESTCASE(typename, key, val, key2, val2) \
  typename##_keyval_s typename##keypair1 = {._key = key, ._data = val};  \
  typename##_keyval_s typename##keypair2 = {._key = key2, ._data = val2};  \
  typename##_keyval_s typename##end_keypair;  \
  typename##_map_s* typename##global_map_testcompile = typename##_new_map(typename##keypair1, typename##keypair2, typename##end_keypair); \
  typename##_node_s* typename##root = NULLT(typename##_node_s); \
  typename##_delete(&typename##root, typename##global_map_testcompile->root); \

void test_simplecreation_trb_tree()
{
  TESTCASE(dri, 0, 0, 1, 1);
  TESTCASE(drf, 0, 0.0f, 1, 1.0f);  
  TESTCASE(drd, 0, 0.0, 1, 1.0);


  mock_struct1 mtest1 = {.a = 0, .b = 0.0f,  .c = 2.0};
  mock_struct2 mtest2 = {.d = '0', .e = 0x8, .f = mtest1};
  mock_struct3 mtest3 = {.g = 0x120120,      .h = mtest2};

  TESTCASE(mocktest1, 0, mtest1, 1, mtest1);
  // TESTCASE(mocktest2, 0, mtest2, 1, mtest2);
  // TESTCASE(mocktest3, 0, mtest3, 1, mtest3);
}