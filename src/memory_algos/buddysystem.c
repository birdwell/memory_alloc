#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./headers/buddysystem.h"

/*
* BUDDY SYSTEM SETUP
*/

static inline int
is_pow_of_2(int x)
{
  return !(x & (x - 1));
}

static inline int
next_pow_of_2(int x)
{
  if (is_pow_of_2(x))
    return x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x + 1;
}

char depth[2056];
int di;

void Push(char c) {
  depth[di++] = ' ';
  depth[di++] = c;
  depth[di++] = ' ';
  depth[di++] = ' ';
  depth[di] = 0;
}

void Pop()
{
  depth[di -= 4] = 0;
}

void print_buddy(buddy_t *tree)
{
  printf("(%d) - p(%s) %i \n", tree->size, tree->process_id, tree->memory_space);

  if (tree->left)
  {
    printf("%s `--", depth);
    Push('|');
    print_buddy(tree->right);
    Pop();

    printf("%s `--", depth);
    Push(' ');
    print_buddy(tree->left);
    Pop();
  }
}

// --- END OF SETUP ---

// int main()
// {
//   int MEMORY_SIZE = 1024;
//   buddy_t *buddy_system = malloc(sizeof(buddy_t));
//   /* set sizeues explicitly, alternative would be calloc() */
//   // ROOT
//   buddy_system->size = 0;
//   buddy_system->memory_space = MEMORY_SIZE;
//   buddy_system->is_alloc = false;
//   strcpy(buddy_system->process_id, "root");
//   buddy_system->left = NULL;
//   buddy_system->right = NULL;
//   bool succesfull;
//   allocate_buddy(buddy_system, 200, buddy_system->memory_space, "A");
//   allocate_buddy(buddy_system, 128, buddy_system->memory_space, "B");
//   allocate_buddy(buddy_system, 120, buddy_system->memory_space, "C");
//   // allocate_buddy(buddy_system, 400, buddy_system->memory_space, "D");
//   Print(buddy_system);

//   // release_buddy(buddy_system, "B");
//   // release_buddy(buddy_system, "C");
//   // Print(buddy_system);

//   find_buddy(buddy_system, "A");
//   buddy_list_allocated(buddy_system, 0);
//   buddy_list_free(buddy_system, 0);
//   printf("\n");
//   free(buddy_system);
// }

void allocate_buddy(buddy_t *tree, int size, int size_of_layer, char *process_id) {
  bool success;
  success = add_buddy(tree, size, size_of_layer, process_id);
  if (success) {
    printf("ALLOCATED %s x \n", process_id);
  } else {
    printf("FAIL REQUEST %s %i \n", process_id, size);
  }
}

bool add_buddy(buddy_t *tree, int size, int size_of_layer, char *process_id)
{
  bool success = false;
  if (size > size_of_layer) {
    return false;
  } else if (size <= size_of_layer) {
    if (tree->left == NULL && tree->right == NULL) {
      if ((size > (size_of_layer / 2)) && !tree->is_alloc) {
        tree->size = size;
        tree->is_alloc = true;
        strcpy(tree->process_id, process_id);
        return true;
      } else if (size < size_of_layer && !tree->is_alloc) {
        buddy_t *next_left_tree = new_buddy(size_of_layer);
        buddy_t *next_right_tree = new_buddy(size_of_layer);
        tree->left = next_left_tree;
        tree->right = next_right_tree;

        success = add_buddy(tree->left, size, tree->left->memory_space, process_id);
        if(!success) 
          success = add_buddy(tree->right, size, tree->right->memory_space, process_id);
      }
    } else if (tree->left != NULL && tree->right != NULL) {
      if (size > size_of_layer) {
        tree->size = size;
        strcpy(tree->process_id, process_id);
        return true;
      } else {
        success = add_buddy(tree->left, size, tree->left->memory_space, process_id);
        if (!success)
          success = add_buddy(tree->right, size, tree->right->memory_space, process_id);
      }
    }
  }

  return success;
}

void release_buddy(buddy_t *tree, char *process_id) {
  bool released = remove_buddy(tree, process_id);
  if (released) {
    clean_tree(tree);
  } else {
    printf("FAIL RELEASE %s \n", process_id);
  }
}

bool remove_buddy(buddy_t *tree, char *process_id) {
  bool found = false;
  // --- Found Tree Node ---
  if (!strcmp(tree->process_id, process_id)) {
    printf("FREE %s %i x \n", process_id, tree->size);

    strcpy(tree->process_id, "");
    tree->size = 0;
    tree->is_alloc = false; 
    found = true;
  } else {
    if (tree->left != NULL && !found) {
      found = remove_buddy(tree->left, process_id);
    } 

    if (tree->right != NULL && !found) {
      found = remove_buddy(tree->right, process_id);
    }
  }

  return found;
}

void find_buddy(buddy_t *tree, char *process_id) {
  bool found = search_tree(tree, process_id);
  if (!found) {
    printf("NOT FOUND \n");
  }
}

bool search_tree(buddy_t *tree, char *process_id) {
  bool found = false;
  if(!strcmp(tree->process_id, process_id)) {
    printf("(%s, %i, x) \n", tree->process_id, tree->size);
    found = true;
  } else {
    if (tree->left != NULL && !found) {
      found = search_tree(tree->left, process_id);
    }

    if (tree->right != NULL && !found) {
      found = search_tree(tree->right, process_id);
    }
  }

  return found;
} 

void buddy_list_allocated(buddy_t *tree, int start) {
  // (A, n, x)
  // if (tree->left == NULL && tree->right == NULL && tree->is_alloc == false && !strcmp(tree->process_id, "root"))
  // {
  //   printf("NONE \n");
  //   return;
  // }

  if (tree == NULL)
    return;
  
  buddy_list_allocated(tree->left, start);
  if (tree->is_alloc) {
    printf("%s, %i, %i \n", tree->process_id, tree->size, start);
  }
  if (tree->left != NULL) {
    buddy_list_allocated(tree->right, tree->left->memory_space + start);
  } else {
    buddy_list_allocated(tree->right, start);
  }
}

void buddy_list_free(buddy_t *tree, int start) {
  bool isFull = true;
  
  isFull = find_free(tree, start, isFull);
  if (isFull) {
    printf("FULL \n");
  }
}

bool find_free(buddy_t *tree, int start, bool isFull) {
  // (n1, x1)
  if (tree == NULL)
    return isFull;
  
  find_free(tree->right, start, isFull);
  if (!tree->is_alloc && strcmp(tree->process_id, "root")) {
    printf("%i, %i \n", tree->memory_space, start);
    isFull = false;
  }
  if (tree->left != NULL) {
    find_free(tree->right, tree->left->memory_space + start, isFull);
  } else {
    find_free(tree->right, start, isFull);
  }

  return isFull;
}

void clean_tree(buddy_t *tree) {
  if (tree == NULL)
    return;
  
  if ((tree->left != NULL && tree->left->is_alloc == false) && (tree->right != NULL && tree->right->is_alloc == false)) {
    tree->left = NULL;
    tree->right = NULL;
  } else {
    clean_tree(tree->left);
    clean_tree(tree->right);
  }
}

buddy_t *new_buddy(int memory_space)
{
  buddy_t *new_buddy = malloc(sizeof(buddy_t));
  new_buddy->size = 0;
  new_buddy->memory_space = memory_space / 2;
  new_buddy->is_alloc = false;
  new_buddy->left = NULL;
  new_buddy->right = NULL;
  return new_buddy;
}

void printBuddy(buddy_t *root)
{
  if (root == NULL)
    return;
  printBuddy(root->left);
  printf("%i\n", root->size);
  printBuddy(root->right);
}