/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
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
  long d;
  mock_struct1 f;
} mock_struct2_alt;

typedef struct 
{
  long g;
  mock_struct2 h;
} mock_struct3;

// typedef struct 
// {
//   double g;
//   mock_struct2 h;
// } mock_struct3_alt;


DEFINE_MAP_TYPE(mocktest1, dataregister_t, mock_struct1);
DEFINE_MAP_TYPE(mocktest2, dataregister_t, mock_struct2);
DEFINE_MAP_TYPE(mocktest2_alt, dataregister_t, mock_struct2_alt);
DEFINE_MAP_TYPE(mocktest3, dataregister_t, mock_struct3);
// DEFINE_MAP_TYPE(mocktest3_alt, dataregister_t, mock_struct3_alt);

void test_simplecreation_trb_tree();
void test_complexcreation_trb_tree();