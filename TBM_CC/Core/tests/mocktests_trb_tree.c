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
  typename##_delete(&typename##root, typename##global_map_testcompile->root); 

#define TESTCASE_NO_DELETE(typename, key, val, key2, val2) \
  typename##_keyval_s typename##keypair1 = {._key = key, ._data = val};  \
  typename##_keyval_s typename##keypair2 = {._key = key2, ._data = val2};  \
  typename##_keyval_s typename##end_keypair;  \
  typename##_map_s* typename##global_map_testcompile = typename##_new_map(typename##keypair1, typename##keypair2, typename##end_keypair);

/** 
 * @todo Copy my redblack tree type into it's own type, sompile it for a PC and not arm, and see if I still get crashes upon the bewlo bugs, 
 *  ^ if yes then there is an inherent problem wiht the desing of my red-black tree, 
 *    otherwise there might be problems with the memory layout of the rb-tree on the teensy  
 * @todo If above does not yield any more insight: connect (or solder) a jtag conenctor to the board and actually debug the below bugs using a jlink probe. 
*/
void test_simplecreation_trb_tree()
{
  /** @note all primary datatypes pass the test */
  TESTCASE(dri_8, 0, -10, 1, -5);
  TESTCASE(dri_16, 0, -10, 1, -5);
  
  TESTCASE_NO_DELETE(dri_32, 0, -10, 1, -5);
  dri_32_keyval_s _pair = {._key = 4, ._data = 5};
  dri_32_insert(&dri_32global_map_testcompile->root, _pair);

  TESTCASE(dri_64, 0, -10, 1, -5);

  TESTCASE(drui_8, 0, 5, 1, 3);
  TESTCASE(drui_16, 0, 5, 1, 3);
  TESTCASE(drui_32, 0, 5, 1, 3);
  TESTCASE(drui_64, 0, 5, 1, 3);

  TESTCASE(drf, 0, 0.0f, 1, 1.0f);  
  TESTCASE(drd, 0, 0.0, 1, 1.0);
}
void test_complexcreation_trb_tree()
{
  /** @bug mock_struct2 & mocktest3_alt does not pass the test */
  mock_struct1 mtest1 = {.a = 0, .b = 0.0f,   .c = 2.0};
  mock_struct2 mtest2 = {.d = 0, .e = 0x8,    .f = mtest1};
  //mock_struct2_alt mtest2_alt = {.d = 0,      .f = mtest1};
  mock_struct3 mtest3 = {.g = 0x120120,       .h = mtest2};
  //mock_struct3_alt mtest3_alt = {.g = 2.0,    .h = mtest2};

  TESTCASE(mocktest1, 0, mtest1, 1, mtest1);
  // TESTCASE(mocktest2, 0, mtest2, 1, mtest2);              // Error < Breaks execution
  // TESTCASE(mocktest2_alt, 0, mtest2_alt, 1, mtest2_alt);  // Error < Breaks execution
  TESTCASE(mocktest3, 0, mtest3, 1, mtest3);
  // TESTCASE(mocktest3_alt, 0, mtest3_alt, 1, mtest3_alt);  // Error < Breaks execution
}