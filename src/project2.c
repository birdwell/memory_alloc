#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "./memory_algos/headers/firstfit.h"
#include "./memory_algos/headers/bestfit.h"
#include "./memory_algos/headers/nextfit.h"

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
    if (!strcmp("", memory[i]))
    {
      if (!found)
      {
        found = true;
        start = i;
        start_of_search = false;
      }
      size++;
      strcpy(last, memory[i]);
    }
    else if (strcmp("", memory[i]) && !strcmp("", last) && !start_of_search)
    {
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
  for (size_t i = 0; i < length; i++)
  {
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
    printf("NONE");
  }
}

void find(char **memory, int length, char *process_id) {
  int start = 0;
  int size = 0;
  bool found = false;

  for (int i = 0; i < length; i++)
  {
    if(!strcmp(memory[i], process_id)) {
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
    printf("NOT FOUND");
  }
}

void printMem(char **memory, int length) {
  for (size_t i = 0; i < length; i++)
  {
    // printf("mem[%zu] = %s \n", i, memory[i]);
  }
}

void release(char **memory, int length, char *process_id) {
  int reclaimed_memory = 0;
  int start = 0;
  bool found = false;
  for (int i = 0; i < length; i++)
  {
    if(!strcmp(memory[i], process_id)) {
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
  char filename[100];
  char linebuffer[MAX_BUFFER];
  FILE *fp;
  char *args[MAX_ARGS];
  char **arg;

  // Get Command Line Args
  strcpy(TYPE, argv[1]);
  MEMORY = atoi(argv[2]);
  strcpy(filename, argv[3]);

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

      if (strcmp("#", args[0]))
      {

        if (!strcmp("LIST", args[0]))
        {
          if (!strcmp("AVAILABLE", args[1]))
          {
            listAvailable(memory, MEMORY);
          }
          else if (!strcmp("ASSIGNED", args[1]))
          {
            listAllocated(memory, MEMORY);
          }
        } else if (!strcmp("REQUEST", args[0])) {
          if(!strcmp("FIRSTFIT", TYPE)) {
            ff_allocate(memory, MEMORY, args[1], atoi(args[2]));
          } else if (!strcmp("BESTFIT", TYPE)) {
            bf_allocate(memory, MEMORY, args[1], atoi(args[2]));
          } else if (!strcmp("NEXTFIT", TYPE)) {
            next_index = nf_allocate(memory, MEMORY, args[1], atoi(args[2]), next_index);
          }
        } else if (!strcmp("RELEASE", args[0])) {
          release(memory, MEMORY, args[1]);
        } else if (!strcmp("FIND", args[0])) {
          find(memory, MEMORY, args[1]);
        }
      }
    }
  }

  return 0;
}
