
/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Niranjan Varma
 * Email: varma@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "bst.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node
{
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst
{
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create()
{
  struct bst* tree = malloc(sizeof(struct bst));

  assert(tree);

  tree->root = NULL;

  return tree;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */

void free_node(struct bst_node* no){
    if(no!=NULL){
        free_node(no->right);
        free_node(no->left);
        free(no);
    }
}
void bst_free(struct bst* bst)
{
    free_node(bst->root);
    free(bst);
}
/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */

int size(struct bst_node* parent){
    if(parent == NULL){
        return 0;
    }
    else{
        return (size(parent->right)+ size(parent->left)+ 1);
    }
}
int bst_size(struct bst* bst)
{
    return size(bst->root);
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */

struct bst_node* nodemaker(int key, void* value){
    struct bst_node* new_node = malloc(sizeof(struct bst_node));

    new_node->value = value;
    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}
struct bst_node* insert(int key, void* value, struct bst_node* parent){
    if(parent==NULL){
        return nodemaker(key, value);
    }
    else if(parent->key < key){
        parent->right = insert(key, value, parent->right);
    }
    else{
        parent->left = insert(key, value, parent->left);
    }
    return parent;
}
void bst_insert(struct bst* bst, int key, void* value)
{
    bst->root = insert(key, value, bst->root);
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */

struct bst_node* find_min(struct bst_node* node){
    struct bst_node * p = NULL;
    while(node->left != NULL){
        p = node;
        node = node->left;
    }
    if (p !=NULL) {
        p->left = node->right;
    }
    return node;
}
struct bst_node* remove_(int key, struct bst_node* parent){
    struct bst_node* temp;

    if(parent == NULL){
        return parent; 
    }
    if(parent->key == key){ 
        if(parent->left == NULL){
            temp = parent->right;
            free(parent);
            return temp;
        }
        else if(parent-> right == NULL){
            temp = parent->left;
            free(parent);
            return temp;
        }
        else if((parent-> right == NULL)&& (parent-> left == NULL)){
            free(parent);
            return NULL;
        }
        else{
            temp = find_min(parent->right);
            
            //parent->right = remove_(temp-> key, parent->right);
            temp->left= parent->left;    

            
            if(parent->right == temp){ 
                free(parent);
                return temp;
            }
            else{
                temp->right = parent->right;
                free(parent);
                return temp;
            }
        }
    }
    else if(parent->key < key){
        parent->right = remove_(key, parent->right);
    }
    else{
         parent->left = remove_(key, parent->left);
    }
    return parent;
}


void bst_remove(struct bst* bst, int key)
{
    bst->root = remove_(key, bst->root);
}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key)
{
    struct bst_node* n = bst->root;

    while(n!= NULL){
        if(n->key == key){
            return n->value;
        }
        else if(n->key > key){
            n = n->left;
        }
        else{
            n = n->right;
        }
    }
    return NULL;
}
