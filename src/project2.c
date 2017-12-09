#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "./memory_algos/headers/firstfit.h"
#include "./memory_algos/headers/bestfit.h"
#include "./memory_algos/headers/nextfit.h"
#include "./memory_algos/headers/buddysystem.h"

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \n"

void listAvailable(char **memory, int length)
{
  char last[16];
  int size = 0;
  int start = 0;
  bool found = false;
  bool printed = false;
  bool start_of_search = true;

  for (int i = 0; i < length; ++i) {
    if (!strcmp("", memory[i])) {
      if (!found) {
        found = true;
        start = i;
        start_of_search = false;
        printed = false;
      }

      size++;
      strcpy(last, memory[i]);
    } else if (strcmp("", memory[i]) && !strcmp("", last) && !start_of_search) {
      printf("(%i,%i) \n", size, start);
      strcpy(last, memory[i]);
      
      found = false;
      size = 0;
      printed = true;
    }
  }

  if (!printed)
  {
    printf("(%i,%i) \n", size, start);
  }
}

void listAllocated(char **memory, int length)
{
  // (A, n, x) A - process label, n - allocated bytes, x - starting point
  // none if there is now allocations
  char last[16] = "";
  int start = 0;
  int size = 0;
  bool started = false;
  bool empty = true;

  for (size_t i = 0; i < length; i++) {
    // Check if it's non-empty
    if(strcmp(memory[i], "")) {
      if(strcmp(memory[i], last)) {
        if (started) {
          printf("(%s,%i,%i)\n", last, size, start);
          empty = false;
        }

        strcpy(last, memory[i]);
        start = i;
        started = true;
        size = 1;
      } else if (!strcmp(memory[i], last)) {
        size++;
        strcpy(last, memory[i]);
      }
    } else if (!strcmp(memory[i], "")) {
      if (started) {
        printf("(%s,%i,%i)\n", last, size, start);
        empty = false;
      }

      size = 0;
      strcpy(last, "");
      started = false;
    }
  }

  if (empty) {
    printf("NONE \n");
  }
}

void find(char **memory, int length, char *process_id) {
  int start = 0;
  int size = 0;
  bool found = false;

  for (int i = 0; i < length; i++) {
    if (!strcmp(memory[i], process_id)) {
      if (!found) {
        start = i;
        found = true;
      }
      size++;
    }
  }

  if(found) {
    printf("(%s, %i, %i) \n", process_id, size, start);
  } else {
    printf("NOT FOUND \n");
  }
}

/*
* Params: memory allocation array, size of memory
* Purpose: Print out array'c contents
*/
void printMem(char **memory, int length) {
  for (size_t i = 0; i < length; i++) {
    printf("mem[%zu] = %s \n", i, memory[i]);
  }
}

void release(char **memory, int length, char *process_id) {
  int reclaimed_memory = 0;
  int start = 0;
  bool found = false;
  for (int i = 0; i < length; i++) {
    if (!strcmp(memory[i], process_id)) {
      if (!found) {
        found = true;
        start = i;
      }
      reclaimed_memory++;
      strcpy(memory[i], "");
    }
  }

  if(found) {
    printf("FREE %s %i %i \n", process_id, reclaimed_memory, start);
  } else {
    printf("FAIL RELEASE %s \n", process_id);
  }
}

int main(int argc, char *argv[])
{
  char TYPE[20];
  int MEMORY;
  int next_index = 0;
  bool allocation = false;
  buddy_t *buddy_system = malloc(sizeof(buddy_t));

  char filename[100];
  char linebuffer[MAX_BUFFER];
  FILE *fp;
  char *args[MAX_ARGS];
  char **arg;

  // Get Command Line Args
  strcpy(TYPE, argv[1]);
  MEMORY = atoi(argv[2]);
  strcpy(filename, argv[3]);


  if (!strcmp(TYPE, "BUDDY")) {
    /* set sizeues explicitly, alternative would be calloc() */
    // ROOT
    buddy_system->size = 0;
    buddy_system->memory_space = MEMORY;
    buddy_system->is_alloc = false;
    strcpy(buddy_system->process_id, "root");
    buddy_system->left = NULL;
    buddy_system->right = NULL;
  }

  // Setup Memory Allocation Array
  char **memory;
  memory = malloc(MEMORY * sizeof(char *));
  for (int i = 0; i < MEMORY; i++)
    memory[i] = malloc((16) * sizeof(char));

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("Error opening file.");
    return -1;
  }

  while (!feof(fp))
  {
    if (fgets(linebuffer, MAX_BUFFER, fp))
    {
      arg = args;
      *arg++ = strtok(linebuffer, SEPARATORS);
      while ((*arg++ = strtok(NULL, SEPARATORS)));


      // Command Guard - Skip Comments
      if (strcmp("#", args[0]))
      {
        // --- List Commands ---
        if (!strcmp("LIST", args[0]))
        {
          if (!strcmp("AVAILABLE", args[1]))
          {
            if (!strcmp(TYPE, "BUDDY")) {
              buddy_list_free(buddy_system, 0);
            } else {
              listAvailable(memory, MEMORY);
            }
          }
          else if (!strcmp("ASSIGNED", args[1]))
          {
            if (!strcmp(TYPE, "BUDDY")) {
              buddy_list_allocated(buddy_system, 0);
            } else {
              listAllocated(memory, MEMORY);
            }
          }
        } else if (!strcmp("REQUEST", args[0])) {
          // --- Request Commands ---
          // Each algorithm has it's own request function
          if(!strcmp("FIRSTFIT", TYPE)) {
            ff_allocate(memory, MEMORY, args[1], atoi(args[2]));
          } else if (!strcmp("BESTFIT", TYPE)) {
            bf_allocate(memory, MEMORY, args[1], atoi(args[2]));
          } else if (!strcmp("NEXTFIT", TYPE)) {
            next_index = nf_allocate(memory, MEMORY, args[1], atoi(args[2]), next_index);
          } else if (!strcmp("BUDDY",TYPE)) {
            allocate_buddy(buddy_system, atoi(args[2]), MEMORY, args[1]);
          }
        } else if (!strcmp("RELEASE", args[0])) {
          // --- Release Command ---
          if (!strcmp("BUDDY", TYPE)) {
            release_buddy(buddy_system, args[1]);
          } else {
            release(memory, MEMORY, args[1]);
          }
        } else if (!strcmp("FIND", args[0])) {
          // --- Find Command ---
          if (!strcmp("BUDDY", TYPE)) {
            find_buddy(buddy_system, args[1]);
          } else {
            find(memory, MEMORY, args[1]);
          }
        }
      }
    }
  }
  // print_buddy(buddy_system);
  free(memory);
  return 0;
}
