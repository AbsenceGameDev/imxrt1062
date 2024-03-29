/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 * 
 * @note We currenlty have no way to inform the user of an assertion failure, I will add assertions to the codebase when this becomes relevant
 */

#include "trb_tree.h"

void test_trb_tree()
{
  // @todo tomorrow: Fix the below code failing to compile due to failing to recognize dri_new_map as an actual function
  dri_keyval_s key1 = {._key = 0x0, ._data = 0x0};
  dri_keyval_s key2 = {._key = 0x1, ._data = 0x2}; 
  dri_keyval_s key3 = {._key = 0x2, ._data = 0x4};
  dri_map_s* global_map_testcompile = dri_new_map(key1, key2, key3);
}