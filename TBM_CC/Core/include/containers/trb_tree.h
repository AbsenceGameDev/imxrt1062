/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 * @todo: We need key hashing
 */
#ifndef TRB_TREE_H
#define TRB_TREE_H

#pragma once

#include "sys/heap.h"
#include <stdarg.h>
#include <stdint.h>

#define NULLT(type) ((type*)0)
#define MAP_MAX_SIZE 0x10000
#define ENDKEY       0x1111 /* Reserved special key to mark the end of the map, used for a variadic , only usable for integer types, will need to rethink this */


//
// Node of red-black tree
typedef enum {EGRANDPARENT, EPARENT, EUNCLE, ESIBLING} dataorder_e;
typedef enum { BLACK, RED } colourbit_e;

#define DEFINE_KEY_VALUE_PAIR(typename, keydatatype, valuedatatype) \
typedef struct typename##_keyval \
{ \
  keydatatype   _key : 31; \
  valuedatatype _data; \
} typename##_keyval_s;
/* typename##_keyval_m = {._key = ENDKEY}; special endkey to mark the end of the map, so the vararg iteration doesn't produce UB */ 
// typedef struct typename##_keyval typename##_keyval_s; 


#define DEFINE_MAP_NODE(typename, keydatatype, valuedatatype) \
DEFINE_KEY_VALUE_PAIR(typename, keydatatype, valuedatatype) \
typedef struct typename##_node \
{ \
  typename##_keyval_s _pair; \
  struct typename##_node *parent, *right, *left; \
  colourbit_e   _color : 1; \
} typename##_node_s; 


#define DEFINE_MAP_BOILERPLATE(typename, keydatatype, valuedatatype) \
/* Traverse arbitrary binary tree, LNR, LeftChain->Node->RightChain */\
static void typename##_traverse_LNR(typename##_node_s* node, void (*typename##_inlinefunc)(keydatatype key));\
static void typename##___insert(typename##_node_s** root, typename##_node_s* node);\
static void typename##_insert_record(typename##_node_s* node); \
static void typename##_delete(typename##_node_s** root, typename##_node_s* node); \
static void typename##_delete_record(typename##_node_s* node); \
static void typename##_nested_ptr_swap(typename##_node_s**, typename##_node_s**);\
\
\
inline keydatatype typename##_get_key(const typename##_node_s* this) { return this->_pair._key; } \
inline void typename##_set_key(typename##_node_s* this, keydatatype key) { this->_pair._key = key; } \
inline valuedatatype typename##_get_value(const typename##_node_s* this) { return this->_pair._data; } \
inline void typename##_set_value(typename##_node_s* this, valuedatatype data) { this->_pair._data = data; } \
inline void typename##_set_keyvalpair(typename##_node_s* this, typename##_keyval_s pair) { this->_pair = pair; } \
\
\
static inline void typename##_insert(typename##_node_s** root, typename##_keyval_s first_keyval_pair) \
{\
    typename##_node_s* newnode = (typename##_node_s*)malloc_(sizeof(typename##_node_s)); /* Allocate memory for the node */ \
    typename##_set_keyvalpair(newnode, first_keyval_pair); \
    typename##___insert(root, newnode);\
}\
\
\
static inline colourbit_e typename##_get_color(const typename##_node_s* this) { return this == NULLT(typename##_node_s) ? BLACK : this->_color; } \
static inline void typename##_set_color(typename##_node_s* this, colourbit_e color) { this->_color = color; }\
\
\
static inline typename##_node_s* typename##_get_node(const typename##_node_s* node, dataorder_e dataorder)\
{\
  typename##_node_s* parent = node != NULLT(typename##_node_s) ? node->parent : NULLT(typename##_node_s);\
  if (parent == NULLT(typename##_node_s)) { return NULLT(typename##_node_s); }\
\
  typename##_node_s* gp = parent->parent;\
  switch (dataorder)\
  {\
    case EGRANDPARENT: return gp;\
    case EPARENT:      return parent;\
    case EUNCLE:       return gp == NULLT(typename##_node_s) ? NULLT(typename##_node_s) : parent == gp->left ? gp->right : gp->left;\
    case ESIBLING:     return node == parent->left ? parent->right : parent->left;\
  }\
\
  return NULLT(typename##_node_s);\
}\
\
\
/* Helper functions */\
/* This function gets the maximum node in a subtree */\
static typename##_node_s* typename##_maximum_node(typename##_node_s* node) \
{\
  while (node->right != NULLT(typename##_node_s)) \
  {\
    node = node->right;\
  }\
  return node;\
}\
\
\
/* This function swaps two nodes */\
static void typename##_swap_node(typename##_node_s* lhs, typename##_node_s* rhs)\
{\
  /* Swap color */\
  colourbit_e colour = typename##_get_color(rhs);\
  typename##_set_color(rhs, typename##_get_color(lhs));\
  typename##_set_color(lhs, colour);\
\
  if (lhs->left == rhs) \
  {\
    rhs->parent = lhs->parent;\
    lhs->parent = rhs;\
\
    lhs->left = rhs->left;\
    rhs->left = lhs;\
    \
    rhs->right = lhs->right;\
    lhs->right = NULLT(typename##_node_s);\
    rhs->right->parent = rhs;\
    if(lhs->left != NULLT(typename##_node_s)) { lhs->left->parent = lhs; }\
\
    if (rhs->parent == NULLT(typename##_node_s))      { return; } \
    if (rhs->parent->left == lhs) { rhs->parent->left = rhs; } \
    else                          { rhs->parent->right = rhs; }\
\
    return;\
  }\
\
  typename##_node_s* lleft = lhs->left; typename##_node_s* rleft = rhs->left;\
  if (lleft != NULLT(typename##_node_s) && rleft != NULLT(typename##_node_s)) { typename##_nested_ptr_swap(&lleft->parent, &rleft->parent); } \
  else if (lleft != NULLT(typename##_node_s)) { lleft->parent = rhs; } \
  else if (rleft != NULLT(typename##_node_s)) { rleft->parent = lhs; }\
\
  typename##_node_s* lright = lhs->right; typename##_node_s* rright = rhs->right;\
  if (lright != NULLT(typename##_node_s) && rright != NULLT(typename##_node_s)) { typename##_nested_ptr_swap(&lright->parent, &rright->parent); } \
  else if (lright != NULLT(typename##_node_s)) { lright->parent = rhs; } \
  else if (rright != NULLT(typename##_node_s)) { rright->parent = lhs; }\
\
  typename##_node_s** rhs_parentref = rhs->parent == NULLT(typename##_node_s) ? NULLT(typename##_node_s*) : \
    rhs->parent->left == rhs ? &rhs->parent->left : &rhs->parent->right;\
  typename##_node_s** lhs_parentref = lhs->parent == NULLT(typename##_node_s) ? NULLT(typename##_node_s*) : \
    lhs->parent->left == lhs ? &lhs->parent->left : &lhs->parent->right;\
\
  if (lhs->parent != NULLT(typename##_node_s) && rhs->parent != NULLT(typename##_node_s)) { typename##_nested_ptr_swap(lhs_parentref, rhs_parentref); } \
  else if (lhs->parent != NULLT(typename##_node_s)) { *lhs_parentref = rhs; } \
  else if (rhs->parent != NULLT(typename##_node_s)) { *rhs_parentref = lhs; }\
\
  /* Swap itself */\
  typename##_nested_ptr_swap(&lhs->parent, &rhs->parent);\
  typename##_nested_ptr_swap(&lhs->left, &rhs->left);\
  typename##_nested_ptr_swap(&lhs->right, &rhs->right);\
}\
\
\
/* This function swaps two addresses */\
static inline void typename##_nested_ptr_swap(typename##_node_s** lhs, typename##_node_s** rhs)\
{\
  void* temp = *rhs;\
  *rhs       = *lhs;\
  *lhs       = temp;\
}\
\
\
/* This function replaces a node with another node */\
static void typename##_replace_node(typename##_node_s** root_node, typename##_node_s* deprecated, typename##_node_s* new_node) \
{\
  /* If: deprecated is root_node,             Then: Set new_node as root_node */\
  /* Else if: deprecated is on parent's left, Then: Set new_node as parent's left */\
  /*                                          Else: Set new_node as parent's right */\
  if (deprecated->parent == NULLT(typename##_node_s))                  { *root_node = new_node; } \
  else if (deprecated == deprecated->parent->left) { deprecated->parent->left = new_node; } \
  else                                             { deprecated->parent->right = new_node; }\
\
  if (new_node == NULLT(typename##_node_s)) { return; }\
  new_node->parent = deprecated->parent;\
}\
\
\
static void typename##_rotate_right(typename##_node_s* node) \
{\
  typename##_node_s* child_r = node->left;\
  typename##_node_s* parent  = node->parent;\
\
  if (child_r->right != NULLT(typename##_node_s)) { child_r->right->parent = node; }\
\
  node->parent    = child_r;\
  node->left      = child_r->right;\
  child_r->right  = node;\
  child_r->parent = parent;\
\
  if (parent == NULLT(typename##_node_s)) { return; }\
\
  switch (parent->left == node)\
  {\
  case 1 /*TRUE*/ : parent->left  = child_r; break;\
  case 0 /*FALSE*/: parent->right = child_r; break;\
  }\
}\
\
\
static void typename##_rotate_left(typename##_node_s* node) \
{\
  typename##_node_s* child_r = node->right;\
  typename##_node_s* parent  = node->parent;\
\
  if (child_r->left != NULLT(typename##_node_s)) { child_r->left->parent = node; }\
\
  node->right     = child_r->left;\
  node->parent    = child_r;\
  child_r->left   = node;\
  child_r->parent = parent;\
\
  if (parent == NULLT(typename##_node_s)) { return; }\
  \
  switch (parent->left == node)\
  {\
  case 1 /*TRUE*/ : parent->left  = child_r; break;    \
  case 0 /*FALSE*/: parent->right = child_r; break;\
  }\
}\
\
\
static inline void typename##_traverse_LNR(typename##_node_s* node, void (*typename##_inlinefunc)(keydatatype data)) \
{\
  if (node == NULLT(typename##_node_s)) { return; } \
\
  typename##_traverse_LNR(node->left, typename##_inlinefunc); \
  typename##_inlinefunc(typename##_get_key(node));\
  typename##_traverse_LNR(node->right, typename##_inlinefunc); \
}\
\
\
inline typename##_node_s* typename##_best_fit(const typename##_node_s* node, keydatatype query)\
{\
  if (node == NULLT(typename##_node_s))           { return NULLT(typename##_node_s); } \
  if (typename##_get_key(node) == query) { return (typename##_node_s*)node; } \
  if (typename##_get_key(node) < query)  { return typename##_best_fit(node->right, query); } \
\
  typename##_node_s* possiblefit = typename##_best_fit(node->left, query); \
  if (possiblefit == NULLT(typename##_node_s) || typename##_get_key(possiblefit) < query) { return (typename##_node_s*)node; } \
\
  return possiblefit; \
}\
\
\
inline typename##_node_s* typename##_find(const typename##_node_s* node, keydatatype query)\
{\
  if (node == NULLT(typename##_node_s))            { return NULLT(typename##_node_s); }\
  if (typename##_get_key(node) == query)  { return (typename##_node_s*)node; }\
  return typename##_get_key(node) > query ? typename##_find(node->left, query) : typename##_find(node->right, query);\
}\
\
\
static inline void typename##___insert(typename##_node_s** root, typename##_node_s* node) \
{\
  /* If root is NULLT(typename##_node_s), set n as root and return */ \
  if (*root == NULLT(typename##_node_s)) \
  {\
    typename##_set_color(node, BLACK); \
    (*root) = node; \
    return; \
  }\
  typename##_node_s* realroot = *root; \
\
  /* Standard BST insertion */ \
  /* While loop that finds the correct position for the node, this is by doing a standard BST insertion which can be eaplined by these steps: */ \
  /* 1. If the node is less than the current node, go left */ \
  /* 2. If the node is greater than the current node, go right */ \
  /* 3. Settle on the deepest node closest in value to the value of the node, assign to y and then assign y to parent */ \
  typename##_node_s* closest_node, *resultbuffer = realroot; \
  while (resultbuffer != NULLT(typename##_node_s)) \
  {\
    closest_node = resultbuffer; \
    resultbuffer = (typename##_get_key(node) < typename##_get_key(resultbuffer)) ? resultbuffer->left : resultbuffer->right; \
  }\
  typename##_set_color(node, RED); \
\
  typename##_node_s* parent = typename##_get_node(node, EPARENT); \
  parent = closest_node; \
\
  /* Assign node to parent, decide if it should be left or right child */ \
  switch (typename##_get_key(node) < typename##_get_key(parent)) \
  {\
  case 1 /*TRUE*/ : parent->left = node; break; \
  case 0 /*FALSE*/: parent->right = node; break; \
  }\
\
  /* Correct the red-black tree */ \
  typename##_insert_record(node); \
\
  /* Correct root node's position */ \
  while (realroot->parent != NULLT(typename##_node_s)) { realroot = realroot->parent; } \
} \
\
\
static inline void typename##_insert_record(typename##_node_s* node) \
{ \
  typename##_node_s* parent = typename##_get_node(node, EPARENT); \
\
  /* Case 1 */ \
  /* If: Parent is NULLT(typename##_node_s), Then: Set color to black and return */ \
  if (parent == NULLT(typename##_node_s)) \
  {\
    typename##_set_color(node, BLACK); \
    return; \
  }\
\
  /* Case 2 */ \
  /* If: Parent is black, Then: Return */ \
  if (typename##_get_color(parent) == BLACK) { return; } \
\
  typename##_node_s* uncle       = typename##_get_node(node, EUNCLE); \
  typename##_node_s* grandparent = typename##_get_node(node, EGRANDPARENT); \
\
  /* Case 3 */ \
  /* If: Uncle is red, Then: Recolor parent, uncle and grandparent, insert record at grandparent */ \
  if ((uncle != NULLT(typename##_node_s)) && (typename##_get_color(uncle) == RED)) \
  {\
    typename##_set_color(parent, BLACK);    \
    typename##_set_color(uncle, BLACK);     \
    typename##_set_color(grandparent, RED); \
    typename##_insert_record(grandparent);  \
    return;                      \
  }\
\
  /* Case 4 */ \
  /* If: Node is right child, parent is left child,       Then: Rotate left at parent */ \
  /* Else if: Node is left child, parent is right child,  Then: Rotate right at parent */ \
  if ((node == parent->right) && (parent == grandparent->left)) \
  {\
    typename##_rotate_left(parent); \
    node = node->left; \
  }\
  else if ((node == parent->left) && (parent == grandparent->right))  \
  {\
    typename##_rotate_right(parent); \
    node = node->right; \
  }\
\
  /* Refresh pointers in case we have rotated */ \
  parent      = typename##_get_node(node, EPARENT); \
  grandparent = typename##_get_node(node, EGRANDPARENT); \
\
  /* Set color of parent to black and grandparent to red */ \
  typename##_set_color(parent, BLACK); \
  typename##_set_color(grandparent, RED); \
\
  /* Case 5 */ \
  /* If: Node is left child, parent is left child,     Then: Rotate right at grandparent */ \
  /* Else: Node is right child, parent is right child, Then: Rotate left at grandparent */ \
  if (node == parent->left) { typename##_rotate_right(grandparent); } \
  else                      { typename##_rotate_left(grandparent); } \
}\
\
\
/* Removal */ \
static void typename##_delete_record(typename##_node_s* node) \
{\
  typename##_node_s* parent  = typename##_get_node(node,EPARENT);\
  typename##_node_s* sibling = typename##_get_node(node,ESIBLING);\
\
  /* Case 1 */\
  if (parent == NULLT(typename##_node_s)) { return; }\
\
  /* Case 2 */\
  /* If: Red sibling */\
  /* Then: Swap colors of sibling and parent, rotate at parent */\
  if (typename##_get_color(sibling) == RED) \
  {\
    typename##_set_color(parent, RED);\
    typename##_set_color(sibling, BLACK);\
    if (node == parent->left)        { typename##_rotate_left(parent); } \
    else /* node == parent->right */ { typename##_rotate_right(parent); }\
  }\
\
  /* In case we rotated the parent in above conditional block, refresh the pointers */\
  parent  = typename##_get_node(node,EPARENT);\
  sibling = typename##_get_node(node,ESIBLING);\
\
  bool red_parent     = typename##_get_color(parent) != BLACK;\
  bool red_sibling    = typename##_get_color(sibling) != BLACK;\
  bool red_childleft  = typename##_get_color(sibling->left) == RED;\
  bool red_childright = typename##_get_color(sibling->left) == RED;\
  bool nodeisright    = node == parent->right;   \
\
  /* Case 3, All black */\
  /* If: Black parent, black sibling, black sibling children */\
  /* Then: Recolor sibling to red and delete parent and exit function */\
  if (!red_parent && !red_sibling && !red_childleft && !red_childright)\
  {\
    typename##_set_color(sibling, RED);\
    typename##_delete_record(parent);\
    return;\
  }\
\
  /* Case 4 */\
  /* If: Red parent, black sibling, black sibling children */\
  /* Then: Swap colors of sibling and parent and exit function */\
  if (red_parent && !red_sibling && !red_childleft && !red_childright)\
  {\
    typename##_set_color(sibling, RED);\
    typename##_set_color(node->parent, BLACK);\
    return;\
  }\
\
  /* Case 5 */\
  /* If: Node is left, sibling is black, sibling has red left child & black right child */\
  /* Then: Rotate right at sibling, swap colors of sibling and its left child */\
  /* Else if: Node is right, sibling is black, sibling has red right child & black left child */\
  /* Then: Rotate left at sibling after swapping colors of sibling and its right child */\
  if (!nodeisright && !red_sibling && red_childleft && !red_childright)\
  {\
    typename##_set_color(sibling, RED);\
    typename##_set_color(sibling->left, BLACK);\
    typename##_rotate_right(sibling);\
  } \
  else if (nodeisright && !red_sibling && !red_childleft && red_childright == true)\
  {\
    typename##_set_color(sibling, RED);\
    typename##_set_color(sibling->right, BLACK);\
    typename##_rotate_left(sibling);\
  }\
\
  /* We refresh the sibling pointer in case we rotated the sibling in above if-else chain */\
  sibling = typename##_get_node(node, ESIBLING);\
\
  typename##_set_color(sibling, typename##_get_color(parent));\
  typename##_set_color(parent, BLACK);\
  \
  /* Case 6 */\
  /* If: Node is left child,  Then: Set sibling's right child to black, rotate left at parent */\
  /* Else: Set sibling's left child to black, rotate right at parent */\
  switch (node == parent->left) \
  {\
  case 1 /* TRUE */: \
    typename##_set_color(sibling->right, BLACK); \
    typename##_rotate_left(parent); \
    break; \
\
  case 0 /* FALSE */: \
    typename##_set_color(sibling->left, BLACK); \
    typename##_rotate_right(parent); \
    break; \
  }\
}\
\
\
static inline void typename##_delete(typename##_node_s** root, typename##_node_s* node)\
{\
  if (node == NULLT(typename##_node_s)) { return; }\
  if (node->parent == NULLT(typename##_node_s) && node->left == NULLT(typename##_node_s) && node->right == NULLT(typename##_node_s)) \
  {\
    *root = NULLT(typename##_node_s);\
    return;\
  }\
\
  if (node->left != NULLT(typename##_node_s) && node->right != NULLT(typename##_node_s)) \
  {\
    /*Copy key/value from predecessor and then delete it instead*/ \
    typename##_node_s* pred = typename##_maximum_node(node->left);\
    typename##_swap_node(node, pred);\
  }\
\
  typename##_node_s* child = node->right == NULLT(typename##_node_s) ? node->left : node->right;\
  /* If: Node is black, Then: Delete node */ \
  if (typename##_get_color(node) == BLACK) \
  {\
    typename##_set_color(node, typename##_get_color(child));\
    typename##_delete_record(node);\
  }\
  \
  /* Replace node with child */ \
  typename##_replace_node(root, node, child);\
\
  /* If: Parent is NULLT(typename##_node_s) and child is not NULLT(typename##_node_s), Then: Set child to black */ \
  if (node->parent == NULLT(typename##_node_s) && child != NULLT(typename##_node_s)) \
  {\
    typename##_set_color(child, BLACK);\
  }\
\
  /* Correct the root node's position */\
  if (*root == NULLT(typename##_node_s)) { return; }\
  while ((*root)->parent != NULLT(typename##_node_s)) { *root = (*root)->parent;}  \
} \
\
static inline typename##_map_s* typename##_new_map(typename##_keyval_s first_keyval_pair, ... /* {2nd keyval_pair, 3rd keyval_pair, etc.. } */) \
{ \
  typename##_map_s* retmap = (typename##_map_s*)malloc_(sizeof(typename##_map_s)); \
\
  va_list ap; \
  typename##_keyval_s current_keyval_pair; \
\
  /* Insert the keyval pair into the map */ \
  typename##_node_s* root = NULLT(typename##_node_s); \
  typename##_insert(&root, first_keyval_pair);\
\
\
  va_start(ap, first_keyval_pair); \
\
\
  bool valid_data = true; \
  int64_t current_step = 0x0; \
  while (valid_data) \
  { \
    current_keyval_pair = va_arg(ap, typename##_keyval_s);   /* Pop out the element from the list. */ \
    if (current_keyval_pair._key == ENDKEY || current_step >= retmap->max_capacity) \
    { \
      /* skip if EOM or past max size */ \
      valid_data = false; \
      continue; \
    }\
\
    /* Insert the keyval pair into the map */ \
    typename##_insert(&root, current_keyval_pair);\
  }\
  va_end(ap); \
  retmap->root = root; \
  return retmap; \
}

#define DEFINE_MAP_TYPE_WIHTOUTNEW(typename, keydatatype, valuedatatype) \
DEFINE_MAP_NODE(typename, keydatatype, valuedatatype); \
typedef struct typename##_map \
{ \
  void*  _alloc;         /* Allocated memory */ \
  int32_t allocatedsize; /* Allocation size of the tree */ \
  int32_t element_count; /* Number of elements in the tree */ \
  int32_t max_capacity;  /* Max capacity */ \
  typename##_node_s* root; \
} typename##_map_s; 
/* = { ._alloc = NULLT(void), .allocatedsize = 0x0, .element_count = 0, .max_capacity = MAP_MAX_SIZE, .root = NULLT(typename##_node_s)};*/ 


#define DEFINE_MAP_TYPE(typename, keydatatype, valuedatatype) \
DEFINE_MAP_TYPE_WIHTOUTNEW(typename, keydatatype, valuedatatype) \
DEFINE_MAP_BOILERPLATE(typename, keydatatype, valuedatatype)
// DEFINE_MAP_FUNCTIONS(typename, keydatatype, valuedatatype)

typedef uint32_t dataregister_t; // Base integer key value
DEFINE_MAP_TYPE(dri, dataregister_t, int32_t); // defines 
DEFINE_MAP_TYPE(drf, dataregister_t, float);
DEFINE_MAP_TYPE(drd, dataregister_t, double);


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


void test_trb_tree();

#endif // TRB_TREE_H