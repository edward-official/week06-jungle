#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root = NULL;
  p->nil->color = RBTREE_BLACK;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void rotateLeft(rbtree *t, node_t *pLeft) {
  node_t *pRight = pLeft->right;
  // Need to move left-tree of pRight to the right-tree of pLeft
  pLeft->right = pRight->left;
  if(pRight->left != t->nil) pRight->left->parent = pLeft;
  // Conduct rotation
  pRight->parent = pLeft->parent;
  if(pLeft->parent == t->nil) t->root = pRight;
  else if(pLeft == pLeft->parent->left) pLeft->parent->left = pRight;
  else pLeft->parent->right = pRight;
  pRight->left = pLeft;
  pLeft->parent = pRight;
}

void rotateRight(rbtree *t, node_t *pRight) {
  node_t *pLeft = pRight->left;
  // Need to move right-tree of pLeft to the left-tree of pRight
  pRight->left = pLeft->right;
  if (pLeft->right != t->nil) pLeft->right->parent = pRight;
  // Conduct rotation
  pLeft->parent = pRight->parent;
  if (pRight->parent == t->nil) t->root = pLeft;
  else if (pRight == pRight->parent->left) pRight->parent->left = pLeft;
  else pRight->parent->right = pLeft;
  pLeft->right = pRight;
  pRight->parent = pLeft;
}

void fixup(rbtree *t, node_t *redNode) {
  node_t *uncle;
  // Parent is also RED
  while(redNode->parent->color == RBTREE_RED) {
    // CASE 1: Parent is left-child
    if(redNode->parent == redNode->parent->parent->left) {
      uncle = redNode->parent->parent->right;
      // Case 1-1: Uncle is also RED
      if(uncle->color == RBTREE_RED) {
        redNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        redNode->parent->parent->color = RBTREE_RED;
        redNode = redNode->parent->parent;
      }
      else {
        // CASE 1-2: Uncle is BLACK and redNode is right-child
        if(redNode == redNode->parent->right) {
          // Need to update redNode due to the rotation
          redNode = redNode->parent;
          rotateLeft(t, redNode);
        }
        // CASE 1-3: Uncle is BLACK and redNode is right-child
        redNode->parent->color = RBTREE_BLACK;
        redNode->parent->parent->color = RBTREE_RED;
        rotateRight(t, redNode->parent->parent);
      }
    }
    // CASE 2: Parent is right-child
    else {
      uncle = redNode->parent->parent->left;
      // Case 2-1: Uncle is also RED
      if (uncle->color == RBTREE_RED) {
        redNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        redNode->parent->parent->color = RBTREE_RED;
        redNode = redNode->parent->parent;
      }
      else {
        // CASE 2-2: Uncle is BLACK and redNode is left-child
        if (redNode == redNode->parent->left) {
          // Need to update redNode due to the rotation
          redNode = redNode->parent;
          rotateRight(t, redNode);
        }
        // CASE 2-3: Uncle is BLACK and redNode is right-child
        redNode->parent->color = RBTREE_BLACK;
        redNode->parent->parent->color = RBTREE_RED;
        rotateLeft(t, redNode->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t* rbtree_insert(rbtree *t, const key_t key) {
  // Generating node to append
  node_t *appendingNode = calloc(1, sizeof(node_t));
  appendingNode->color = RBTREE_RED;
  appendingNode->key = key;
  appendingNode->left = t->nil;
  appendingNode->right = t->nil;
  appendingNode->parent = t->nil;
  // If tree is empty, just place it on the root
  if(!t->root) {
    t->root = appendingNode;
    return t->root;
  }
  // If tree is not empty, append on the proper place
  node_t *traverse = t->root;
  node_t *parent = t->nil;
  // Find proper place
  while(traverse != t->nil) {
    parent = traverse;
    if(key < traverse->key) traverse = traverse->left;
    else traverse = traverse->right;
  }
  // Append under the place we found
  appendingNode->parent = parent;
  if(key < traverse->key) parent->left = appendingNode;
  else parent->right = appendingNode;
  // Fix up the tree
  return t->root;
}

node_t* rbtree_find(const rbtree *t, const key_t key) {
  if(!t->root) return NULL;
  node_t *traverse = t->root;
  while(traverse) {
    if(key == traverse->key) return traverse;
    if(key < traverse->key) traverse = traverse->left;
    else traverse = traverse->right;
  }
}

node_t *rbtree_min(const rbtree *t) {
  if(!t->root) return NULL;
  node_t *traverse = t->root;
  while(traverse->left) traverse = traverse->left;
  return traverse;
}

node_t *rbtree_max(const rbtree *t) {
  if(!t->root) return NULL;
  node_t *traverse = t->root;
  while(traverse->right) traverse = traverse->right;
  return traverse;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
