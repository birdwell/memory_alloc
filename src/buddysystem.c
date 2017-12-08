#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
* BUDDY SYSTEM SETUP
*/

typedef struct buddy
{
  int size;
  int memory_space;
  bool is_alloc;
  char process_id[16];
  int start;

  struct buddy *left;
  struct buddy *right;
} buddy_t;

void add_buddy(buddy_t *tree, int size, int size_of_layer, char *process_id);

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

// --- END OF SETUP ---

int main()
{
  int MEMORY_SIZE = 1024;
  buddy_t *buddy_system = malloc(sizeof(buddy_t));
  /* set sizeues explicitly, alternative would be calloc() */
  // ROOT
  buddy_system->size = 0;
  buddy_system->memory_space = MEMORY_SIZE;
  buddy_system->is_alloc = false;
  strcpy(buddy_system->process_id, "root");
  buddy_system->left = NULL;
  buddy_system->right = NULL;

  add_buddy(buddy_system, 200, buddy_system->memory_space, "A");
  printBuddy(buddy_system);
  printf("\n");
}

void add_buddy(buddy_t *tree, int size, int size_of_layer, char *process_id)
{
  if (size > size_of_layer)
  {
    return;
  }
  else if (size <= size_of_layer)
  {
    if (tree->left == NULL && tree->right == NULL)
    {
      if (size > (size_of_layer / 2))
      {
        tree->size = size;
        strcpy(tree->process_id, process_id);
        return;
      }
      else
      {
        buddy_t *next_left_tree = new_buddy(size_of_layer);
        buddy_t *next_right_tree = new_buddy(size_of_layer);
        tree->left = next_left_tree;
        tree->right = next_right_tree;
        add_buddy(tree->left, size, tree->left->memory_space, process_id);
      }
    }
  }
}

buddy_t *new_buddy(int memory_space)
{
  buddy_t *new_buddy = malloc(sizeof(buddy_t));
  new_buddy->size = 0;
  new_buddy->memory_space = memory_space / 2;
  new_buddy->is_alloc = false;
  buddy_system->left = NULL;
  buddy_system->right = NULL;
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