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

  struct buddy *left;
  struct buddy *right;
} buddy_t;

void insert(buddy_t *tree, int size);
void print_tree(buddy_t *current);
void printDFS(buddy_t *current);
void add_buddy(buddy_t *tree, int size);

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

  add_buddy(buddy_system, 200);

  // insert(buddy_system, 5);
  // insert(buddy_system, 8);
  // insert(buddy_system, 4);
  // insert(buddy_system, 3);

  // printDFS(buddy_system);
  printf("\n");
}

void add_buddy(buddy_t *tree, int size) {
  if(tree->left == NULL && tree->right == NULL) {
    int space_works;
    int levels = 0;
    bool fits = true;
    int upper = tree->memory_space;
    int lower = next_pow_of_2(tree->memory_space);
    while (fits)
    {
      if (size <= upper && size > lower) {
        printf("1st - Upper: %i, Lower: %i \n", upper, lower);

        fits = true;
        space_works = upper;
      } else if (size <= lower) {
        printf("2nd init: Upper: %i, Lower: %i \n", upper, lower);

        int temp_upper = upper;
        upper = lower;
        lower = next_pow_of_2(upper);

        levels++;
        fits = true;
        space_works = upper;
        printf("2nd new: Upper: %i, Lower: %i \n", upper, lower);
      } else if (size > upper) {
        fits = false;
      }
      if (levels == 10) {
        fits = false;
      }
      printf("Level: %i, Size: %i \n", levels, space_works);
    }
  }
}

void insert(buddy_t *tree, int size)
{
  if (tree->size == 0)
  {
    /* insert on current (empty) position */
    tree->size = size;
  }
  else
  {
    if (size < tree->size)
    {
      /* insert left */
      if (tree->left != NULL)
      {
        insert(tree->left, size);
      }
      else
      {
        tree->left = malloc(sizeof(buddy_t));
        /* set sizeues explicitly, alternative would be calloc() */
        tree->left->size = size;
        tree->left->left = NULL;
        tree->left->right = NULL;
      }
    }
    else
    {
      if (size >= tree->size)
      {
        /* insert right */
        if (tree->right != NULL)
        {
          insert(tree->right, size);
        }
        else
        {
          tree->right = malloc(sizeof(buddy_t));
          /* set sizeues explicitly, alternative would be calloc() */
          tree->right->size = size;
          tree->right->left = NULL;
          tree->right->right = NULL;
        }
      }
    }
  }
}

/* depth-first search */
void printDFS(buddy_t *current)
{
  /* change the code here */
  if (current == NULL)
    return; /* security measure */
  if (current->left != NULL)
    printDFS(current->left);
  if (current != NULL)
    printf("%d ", current->size);
  if (current->right != NULL)
    printDFS(current->right);
}