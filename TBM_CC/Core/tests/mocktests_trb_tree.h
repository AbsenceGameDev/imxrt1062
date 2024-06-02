/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#include "trb_tree.h"

// Some rudimentary testing
typedef struct 
{
  int a;
  float b;
  double c;
} mock_struct1;

typedef struct 
{
  char d;
  short e;
  mock_struct1 f;
} mock_struct2;

typedef struct 
{
  long g;
  mock_struct2 h;
} mock_struct3;


DEFINE_MAP_TYPE(mocktest1, dataregister_t, mock_struct1);
DEFINE_MAP_TYPE(mocktest2, dataregister_t, mock_struct2);
DEFINE_MAP_TYPE(mocktest3, dataregister_t, mock_struct3);


void test_simplecreation_trb_tree();