#ifndef TRB_TREE_H
#define TRB_TREE_H

#include "sys/heap.h"
#include <stdint.h>


//
// Node of red-black tree
typedef uint32_t dataregister_t;
typedef enum { BLACK, RED } colourbit_e;
typedef struct node
{
  void*  _data; // Buffer / Payload
  struct node *parent, *right, *left;

  dataregister_t _key : 31;
  colourbit_e _color : 1;
} node_s;


//
// Interfaces
void    traverse_LNR(node_s* root, void (*)(dataregister_t data));
node_s* best_fit(const node_s* root, dataregister_t query);
node_s* find(const node_s* root, dataregister_t query);
void    insert(node_s** root, node_s* node);
void    delete(node_s** root, node_s* node);

#endif // TRB_TREE_H