/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 * 
 * @note We currenlty have no way to inform the user of an assertion failure, I will add assertions to the codebase when this becomes relevant
 */

#include "trb_tree.h"

typedef enum {EGRANDPARENT, EPARENT, EUNCLE, ESIBLING} dataorder_e;

// Fwd declarations
void insert_record(node_s*);
void delete_record(node_s*);
void nested_ptr_swap(node_s**, node_s**);

static inline dataregister_t get_key(const node_s* this) { return this->_key; }
static inline void set_key(node_s* this, dataregister_t data) { this->_key = data; }

static inline colourbit_e get_color(const node_s* this) { return this == NULL ? BLACK : this->_color; }
static inline void set_color(node_s* this, colourbit_e color) { this->_color = color; }

static inline node_s* get_node(const node_s* node, dataorder_e dataorder)
{
  node_s* parent = node != NULL ? node->parent : NULL;
  if (parent == NULL) { return NULL; }

  node_s* gp = parent->parent;
  switch (dataorder)
  {
    case EGRANDPARENT: return gp;
    case EPARENT:      return parent;
    case EUNCLE:       return gp == NULL ? NULL : parent == gp->left ? gp->right : gp->left;
    case ESIBLING:     return node == parent->left ? parent->right : parent->left;
  }

  return NULL;
}


//
// Helper functions
// This function gets the maximum node in a subtree
static node_s* maximum_node(node_s* node) 
{
  while (node->right != NULL) 
  {
    node = node->right;
  }
  return node;
}

// This function swaps two nodes
static void swap_node(node_s* lhs, node_s* rhs) 
{
  // Swap color
  colourbit_e c = get_color(rhs);
  set_color(rhs, get_color(lhs));
  set_color(lhs, c);

  if (lhs->left == rhs) 
  {
    rhs->parent = lhs->parent;
    lhs->parent = rhs;

    lhs->left = rhs->left;
    rhs->left = lhs;
    
    rhs->right = lhs->right;
    lhs->right = NULL;
    rhs->right->parent = rhs;
    if(lhs->left != NULL) { lhs->left->parent = lhs; }

    if (rhs->parent == NULL)      { return; } 
    if (rhs->parent->left == lhs) { rhs->parent->left = rhs; } 
    else                          { rhs->parent->right = rhs; }

    return;
  }

  node_s* lleft = lhs->left; node_s* rleft = rhs->left;
  if (lleft != NULL && rleft != NULL) { nested_ptr_swap(&lleft->parent, &rleft->parent); } 
  else if (lleft != NULL) { lleft->parent = rhs; } 
  else if (rleft != NULL) { rleft->parent = lhs; }

  node_s* lright = lhs->right; node_s* rright = rhs->right;
  if (lright != NULL && rright != NULL) { nested_ptr_swap(&lright->parent, &rright->parent); } 
  else if (lright != NULL) { lright->parent = rhs; } 
  else if (rright != NULL) { rright->parent = lhs; }

  node_s** rhs_parentref = rhs->parent == NULL ? NULL :
    rhs->parent->left == rhs ? &rhs->parent->left : &rhs->parent->right;
  node_s** lhs_parentref = lhs->parent == NULL ? NULL :
    lhs->parent->left == lhs ? &lhs->parent->left : &lhs->parent->right;

  if (lhs->parent != NULL && rhs->parent != NULL) { nested_ptr_swap(lhs_parentref, rhs_parentref); } 
  else if (lhs->parent != NULL) { *lhs_parentref = rhs; } 
  else if (rhs->parent != NULL) { *rhs_parentref = lhs; }

  // Swap itself
  nested_ptr_swap(&lhs->parent, &rhs->parent);
  nested_ptr_swap(&lhs->left, &rhs->left);
  nested_ptr_swap(&lhs->right, &rhs->right);
}

// This function swaps two addresses
void nested_ptr_swap(node_s** lhs, node_s** rhs) 
{
  void* temp = *rhs;
  *rhs       = *lhs;
  *lhs       = temp;
}

// This function replaces a node with another node
static void replace_node(node_s** root_node, node_s* deprecated, node_s* new_node) 
{
  // If: deprecated is root_node,             Then: Set new_node as root_node
  // Else if: deprecated is on parent's left, Then: Set new_node as parent's left
  //                                          Else: Set new_node as parent's right
  if (deprecated->parent == NULL)                  { *root_node = new_node; } 
  else if (deprecated == deprecated->parent->left) { deprecated->parent->left = new_node; } 
  else                                             { deprecated->parent->right = new_node; }

  if (new_node == NULL) { return; }
  new_node->parent = deprecated->parent;
}

static void rotate_right(node_s* node) 
{
  node_s* child_r = node->left;
  node_s* parent  = node->parent;

  if (child_r->right != NULL) { child_r->right->parent = node; }

  node->parent    = child_r;
  node->left      = child_r->right;
  child_r->right  = node;
  child_r->parent = parent;

  if (parent == NULL) { return; }

  switch (parent->left == node)
  {
  case 1 /*TRUE*/ : parent->left  = child_r; break;    
  case 0 /*FALSE*/: parent->right = child_r; break;
  }
}

static void rotate_left(node_s* node) 
{
  node_s* child_r = node->right;
  node_s* parent  = node->parent;

  if (child_r->left != NULL) { child_r->left->parent = node; }

  node->right     = child_r->left;
  node->parent    = child_r;
  child_r->left   = node;
  child_r->parent = parent;

  if (parent == NULL) { return; }
  
  switch (parent->left == node)
  {
  case 1 /*TRUE*/ : parent->left  = child_r; break;    
  case 0 /*FALSE*/: parent->right = child_r; break;
  }
}

//
// Traverse arbitrary binary tree in inorder fashion
void traverse_LNR(node_s* node, void (*inlinefunc)(dataregister_t data)) 
{
  if (node == NULL) { return; }

  traverse_LNR(node->left, func);
  inlinefunc(get_key(node));
  traverse_LNR(node->right, func);
}


//
// Best fit
node_s* best_fit(const node_s* node, dataregister_t query) 
{
  if (node == NULL)           { return NULL; }
  if (get_key(node) == query) { return (node_s*)node; }
  if (get_key(node) < query)  { return best_fit(node->right, query); }

  node_s* possiblefit = best_fit(node->left, query);
  if (possiblefit == NULL || get_key(possiblefit) < query) { return (node_s*)node; }
  
  return possiblefit;
}

//
// Find
node_s* find(const node_s* node, dataregister_t query) 
{
  if (node == NULL)            { return NULL; }
  if (get_key(node) == query)  { return (node_s*)node; }
  return get_key(node) > query ? find(node->left, query) : find(node->right, query);
}

//
// Insertion
void insert(node_s** root, node_s* node) 
{
  // If root is NULL, set n as root and return
  if (*root == NULL) 
  {
    set_color(node, BLACK);
    (*root) = node;
    return;
  }
  node_s* realroot = *root;

  // Standard BST insertion
  // While loop that finds the correct position for the node, this is by doing a standard BST insertion which can be eaplined by these steps:
  // 1. If the node is less than the current node, go left
  // 2. If the node is greater than the current node, go right
  // 3. Settle on the deepest node closest in value to the value of the node, assign to y and then assign y to parent
  node_s* closest_node, *resultbuffer = realroot;
  while (resultbuffer != NULL) 
  {
    closest_node = resultbuffer;
    resultbuffer = (get_key(node) < get_key(resultbuffer)) ? resultbuffer->left : resultbuffer->right;
  }
  set_color(node, RED);

  node_s* parent = get_node(node, EPARENT); 
  parent = closest_node;
  
  // Assign node to parent, decide if it should be left or right child

  switch (get_key(node) < get_key(parent))
  {
  case 1 /*TRUE*/ : parent->left = node; break;    
  case 0 /*FALSE*/: parent->right = node; break;
  }

  // Correct the red-black tree
  insert_record(node);

  // Correct root node's position
  while (realroot->parent != NULL) { realroot = realroot->parent; }
}

void insert_record(node_s* node) 
{
  node_s* parent = get_node(node, EPARENT);

  // Case 1
  // If: Parent is NULL, Then: Set color to black and return
  if (parent == NULL) 
  {
    set_color(node, BLACK);
    return;
  }

  // Case 2
  // If: Parent is black, Then: Return
  if (get_color(parent) == BLACK) { return; }

  node_s* uncle       = get_node(node, EUNCLE);
  node_s* grandparent = get_node(node, EGRANDPARENT);
  
  // Case 3
  // If: Uncle is red, Then: Recolor parent, uncle and grandparent, insert record at grandparent
  if ((uncle != NULL) && (get_color(uncle) == RED)) 
  {
    set_color(parent, BLACK);
    set_color(uncle, BLACK);
    set_color(grandparent, RED);
    insert_record(grandparent);
    return;
  }

  // Case 4
  // If: Node is right child, parent is left child,       Then: Rotate left at parent
  // Else if: Node is left child, parent is right child,  Then: Rotate right at parent
  if ((node == parent->right) && (parent == grandparent->left)) 
  {
    rotate_left(parent);
    node = node->left;
  } 
  else if ((node == parent->left) && (parent == grandparent->right)) 
  {
    rotate_right(parent);
    node = node->right;
  }

  // Refresh pointers in case we have rotated
  parent      = get_node(node, EPARENT); 
  grandparent = get_node(node, EGRANDPARENT);

  // Set color of parent to black and grandparent to red
  set_color(parent, BLACK);
  set_color(grandparent, RED);

  // Case 5
  // If: Node is left child, parent is left child,     Then: Rotate right at grandparent
  // Else: Node is right child, parent is right child, Then: Rotate left at grandparent
  if (node == parent->left) { rotate_right(grandparent); } 
  else                      { rotate_left(grandparent); }
}

//
// Removal
void delete(node_s** root, node_s* node) 
{
  if (node == NULL) { return; }
  if (node->parent == NULL && node->left == NULL && node->right == NULL) 
  {
    *root = NULL;
    return;
  }

  if (node->left != NULL && node->right != NULL) 
  {
    // Copy key/value from predecessor and then delete it instead
    node_s* pred = maximum_node(node->left);
    swap_node(node, pred);
  }

  node_s* child = node->right == NULL ? node->left : node->right;
  // If: Node is black, Then: Delete node
  if (get_color(node) == BLACK) 
  {
    set_color(node, get_color(child));
    delete_record(node);
  }
  
  // Replace node with child
  replace_node(root, node, child);

  // If: Parent is NULL and child is not NULL, Then: Set child to black
  if (node->parent == NULL && child != NULL) 
  {
    set_color(child, BLACK);
  }

  // Correct the root node's position
  if (*root == NULL) { return; }
  while ((*root)->parent != NULL) { *root = (*root)->parent;}
}

void delete_record(node_s* node) 
{
  node_s* parent  = get_node(node,EPARENT);
  node_s* sibling = get_node(node,ESIBLING);

  // Case 1
  if (parent == NULL) { return; }

  // Case 2
  // If: Red sibling
  // Then: Swap colors of sibling and parent, rotate at parent
  if (get_color(sibling) == RED) 
  {
    set_color(parent, RED);
    set_color(sibling, BLACK);
    if (node == parent->left)        { rotate_left(parent); } 
    else /* node == parent->right */ { rotate_right(parent); }
  }

  // In case we rotated the parent in above conditional block, refresh the pointers
  parent  = get_node(node,EPARENT);
  sibling = get_node(node,ESIBLING);

  // Case 3, All black
  // If: Black parent, black sibling, black sibling children
  // Then: Recolor sibling to red and delete parent and exit function
  if (get_color(parent)         == BLACK &&
      get_color(sibling)        == BLACK &&
      get_color(sibling->left)  == BLACK &&
      get_color(sibling->right) == BLACK)
  {
    set_color(sibling, RED);
    delete_record(parent);
    return;
  }

  // Case 4, 
  // If: Red parent, black sibling, black sibling children
  // Then: Swap colors of sibling and parent and exit function
  if (
      get_color(parent)         == RED &&
      get_color(sibling)        == BLACK &&
      get_color(sibling->left)  == BLACK &&
      get_color(sibling->right) == BLACK)
  {
    set_color(sibling, RED);
    set_color(node->parent, BLACK);
    return;
  }

  // Case 5, 
  // If: Node is left child, sibling is black, sibling has red left child & black right child
  // Then: Rotate right at sibling, swap colors of sibling and its left child
  // Else if: Node is right child, sibling is black, sibling has red right child & black left child
  // Then: Rotate left at sibling after swapping colors of sibling and its right child
  if (node == parent->left &&
      get_color(sibling)        == BLACK &&
      get_color(sibling->left)  == RED &&
      get_color(sibling->right) == BLACK)
  {
    set_color(sibling, RED);
    set_color(sibling->left, BLACK);
    rotate_right(sibling);
  } 
  else if (node == parent->right &&
    get_color(sibling)        == BLACK &&
    get_color(sibling->right) == RED &&
    get_color(sibling->left)  == BLACK)
  {
    set_color(sibling, RED);
    set_color(sibling->right, BLACK);
    rotate_left(sibling);
  }

  // We refresh the sibling pointer in case we rotated the sibling in above if-else chain
  sibling = get_node(node, ESIBLING);

  set_color(sibling, get_color(parent));
  set_color(parent, BLACK);
  
  // Case 6
  // If: Node is left child,  Then: Set sibling's right child to black, rotate left at parent
  // Else: Set sibling's left child to black, rotate right at parent
  switch (node == parent->left)
  {
  case 1 /* TRUE */:
    set_color(sibling->right, BLACK);
    rotate_left(parent);
    break;

  case 0 /* FALSE */:
    set_color(sibling->left, BLACK);
    rotate_right(parent);
    break;    
  }
}