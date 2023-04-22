#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap* map = malloc(sizeof(TreeMap));
  if ( map == NULL) return NULL;
  map->root = NULL;
  map->current = NULL;
  map->lower_than = lower_than;
  return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  tree->current = tree->root;
  TreeNode *node = createTreeNode(key,value);
  TreeNode *aux = tree->current;
  
  int resultado;
  
  while ( tree->current != NULL){
    resultado = is_equal(tree,tree->current->pair->key,key);
    if( resultado == 1){
      return;
    } 
    aux = tree->current;
    
    if(tree->lower_than(key,tree->current->pair->key) == 1){
      tree->current =  tree->current->left;
    }
    else{
      tree->current = tree->current->right;
    }
  }
  
  if(tree->lower_than(aux->pair->key,key) == 0){
    aux->left = node;
    node->parent = aux;
  }
    else{
    aux->right = node;
    node->parent = aux;
  }
  tree->current = node;
  
}

TreeNode * minimum(TreeNode * x){
  while( x->left != NULL){
     x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current = tree->root;
  int resultado;
  
  while ( tree->current != NULL){
    resultado = is_equal(tree,key,tree->current->pair->key);
    if( resultado == 1){
      return tree->current->pair;
    } 
    else if(tree->lower_than(key,tree->current->pair->key) == 1){
      tree->current =  tree->current->left;
      }
    else{
      tree->current = tree->current->right;
      }
  }
  return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode *ub_node = NULL;
  TreeNode *auxNode = tree->root;
  
  int aux = is_equal(tree,auxNode->current->pair->key,key);
  
  while( aux != NULL){
    aux = is_equal(tree,auxNode->current->pair->key,key);
    if ( aux == 1){
      return auxNode->pair;
    }

    if ( tree->lower_than(key,auxNode->pair->key) == 1){
      ub_node = auxNode;
      auxNode = auxNode->left;
    }
    else{
      auxNode = auxNode->right;
    }
  }

  if ( ub_node != NULL){
    return ub_node->pair;
  }
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *minimo = minimum(tree->root);
  return minimo->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *aux = tree->current->parent;
  if ( tree->current == NULL){
    tree->current = minimum(tree->root);
    return tree->current->pair;
  }

  if ( tree->current->right != NULL){
    tree->current = minimum(tree->current->right);
    return tree->current->pair;
  }

  while ( aux != NULL && tree->current == aux->right){
    tree->current = aux;
    aux = tree->current->parent;
  }
  tree->current = aux;
  if ( tree->current != NULL){
    return tree->current->pair;
  }
  return NULL;
}
