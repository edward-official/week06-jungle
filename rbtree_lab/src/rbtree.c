#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if(!p) return NULL;
  // Allocate sentinel
  node_t *sentinel = (node_t *)calloc(1, sizeof(node_t));
  if(!sentinel) {
    free(p);
    return NULL;
  }
  sentinel->color  = RBTREE_BLACK;
  sentinel->left   = sentinel;
  sentinel->right  = sentinel;
  sentinel->parent = sentinel;
  p->nil  = sentinel;
  p->root = sentinel;
  return p;
}

void _free_rbtree(node_t *node, node_t *sentinel) {
  if(!node || node == sentinel) return;
  _free_rbtree(node->left, sentinel);
  _free_rbtree(node->right, sentinel);
  free(node);
}

void delete_rbtree(rbtree *t) {
  _free_rbtree(t->root, t->nil);
  free(t->nil);
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
  if(pLeft->right != t->nil) pLeft->right->parent = pRight;
  // Conduct rotation
  pLeft->parent = pRight->parent;
  if(pRight->parent == t->nil) t->root = pLeft;
  else if(pRight == pRight->parent->left) pRight->parent->left = pLeft;
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
      if(uncle->color == RBTREE_RED) {
        redNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        redNode->parent->parent->color = RBTREE_RED;
        redNode = redNode->parent->parent;
      }
      else {
        // CASE 2-2: Uncle is BLACK and redNode is left-child
        if(redNode == redNode->parent->left) {
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
  if(!appendingNode) return NULL;
  appendingNode->color = RBTREE_RED;
  appendingNode->key = key;
  appendingNode->left = t->nil;
  appendingNode->right = t->nil;
  appendingNode->parent = t->nil;
  // If tree is empty, just place it on the root
  if(t->root == t->nil) {
    t->root = appendingNode;
    t->root->color = RBTREE_BLACK;
    return appendingNode;
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
  if(key < parent->key) parent->left = appendingNode;
  else parent->right = appendingNode;
  // Fix up the tree
  fixup(t, appendingNode);
  return appendingNode;
}

node_t* rbtree_find(const rbtree *t, const key_t key) {
  if(t->root == t->nil) return NULL;
  node_t *traverse = t->root;
  while(traverse != t->nil) {
    if(key == traverse->key) return traverse;
    if(key < traverse->key) traverse = traverse->left;
    else traverse = traverse->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  if(!t->root) return NULL;
  node_t *traverse = t->root;
  while(traverse->left != t->nil) traverse = traverse->left;
  return traverse;
}

node_t *rbtree_max(const rbtree *t) {
  if(!t->root) return NULL;
  node_t *traverse = t->root;
  while(traverse->right != t->nil) traverse = traverse->right;
  return traverse;
}

void rbtree_transplant(rbtree *t, node_t *p1, node_t *p2) {
  if(p1->parent == t->nil) t->root = p2;
  else if(p1 == p1->parent->left) p1->parent->left = p2;
  else p1->parent->right = p2;
  p2->parent = p1->parent;
}

node_t *tree_minimum(const rbtree *t, node_t *p) {
  while(p->left != t->nil) p = p->left;
  return p;
}

void rbtree_delete_fixup(rbtree *t, node_t *doubleBlack) {
  while(doubleBlack != t->root && doubleBlack->color == RBTREE_BLACK) {
    // Case 1: doubleBlack is left-child.
    if(doubleBlack == doubleBlack->parent->left) {
      node_t *sibling = doubleBlack->parent->right;
      if(sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        doubleBlack->parent->color = RBTREE_RED;
        rotateLeft(t, doubleBlack->parent);
        sibling = doubleBlack->parent->right;
      }
      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        doubleBlack = doubleBlack->parent;
      }
      else {
        if(sibling->right->color == RBTREE_BLACK) {
          sibling->left->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          rotateRight(t, sibling);
          sibling = doubleBlack->parent->right;
        }
        sibling->color = doubleBlack->parent->color;
        doubleBlack->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        rotateLeft(t, doubleBlack->parent);
        doubleBlack = t->root;
      }
    }
    // Case 2: doubleBlack is right-child.
    else {
      node_t *sibling = doubleBlack->parent->left;
      if(sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        doubleBlack->parent->color = RBTREE_RED;
        rotateRight(t, doubleBlack->parent);
        sibling = doubleBlack->parent->left;
      }
      if(sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        doubleBlack = doubleBlack->parent;
      }
      else {
        if(sibling->left->color == RBTREE_BLACK) {
          sibling->right->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          rotateLeft(t, sibling);
          sibling = doubleBlack->parent->left;
        }
        sibling->color = doubleBlack->parent->color;
        doubleBlack->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        rotateRight(t, doubleBlack->parent);
        doubleBlack = t->root;
      }
    }
  }
  // Exited loop, Update Color
  doubleBlack->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *deletingNode) {
  node_t *successor = deletingNode, *rightChildOfSuccessor;
  color_t removedColor = deletingNode->color;
  // Case 1: One or less child from deletingNode
  if(deletingNode->left == t->nil) {
    rightChildOfSuccessor = deletingNode->right;
    rbtree_transplant(t, deletingNode, deletingNode->right);
  }
  // Case 1: One or less child from deletingNode
  else if(deletingNode->right == t->nil) {
    rightChildOfSuccessor = deletingNode->left;
    rbtree_transplant(t, deletingNode, deletingNode->left);
  }
  // Case 2: Two children from deletingNode
  else {
    successor = tree_minimum(t, deletingNode->right);
    removedColor = successor->color;
    rightChildOfSuccessor = successor->right;
    // Replace successor with it's right sub-tree. (Left sub-tree doesn't exist.)
    rbtree_transplant(t, successor, successor->right);
    // Replace deletingNode with rightChildOfSuccessor
    successor->right = deletingNode->right;
    successor->right->parent = successor;
    // Complete the replacement.
    rbtree_transplant(t, deletingNode, successor);
    successor->left = deletingNode->left;
    successor->left->parent = successor;
    successor->color = deletingNode->color;
  }
  if(removedColor == RBTREE_BLACK) rbtree_delete_fixup(t, rightChildOfSuccessor);
  free(deletingNode);
  return 0;
}

static void inorder_to_array(const rbtree *t, const node_t *x, key_t *arr, const size_t n, size_t *idx) {
  if(x == t->nil || *idx >= n) return;
  inorder_to_array(t, x->left, arr, n, idx);
  if(*idx < n) {
    arr[*idx] = x->key;
    (*idx)++;
  }
  inorder_to_array(t, x->right, arr, n, idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if(t == NULL || arr == NULL || n == 0) return 0;
  size_t idx = 0;
  inorder_to_array(t, t->root, arr, n, &idx);
  return (int)idx;
}
