#ifndef _buddysystem_h
#define _buddysystem_h

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

bool add_buddy(buddy_t *tree, int size, int size_of_layer, char *process_id);
buddy_t *new_buddy(int memory_space);
void printBuddy(buddy_t *root);
void allocate_buddy(buddy_t *tree, int size, int size_of_layer, char *process_id);
void release_buddy(buddy_t *tree, char *process_id);
bool remove_buddy(buddy_t *tree, char *process_id);
void clean_tree(buddy_t *tree);
bool search_tree(buddy_t *tree, char *process_id);
void find_buddy(buddy_t *tree, char *process_id);
void buddy_list_allocated(buddy_t *tree, int size);
void buddy_list_free(buddy_t *tree, int start);
bool find_free(buddy_t *tree, int start, bool isFull);
void print_buddy(buddy_t *tree);

#endif