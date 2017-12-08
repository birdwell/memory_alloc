#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./headers/nextfit.h"

bool isMemEmpty(char **memory, int length) {
  bool isEmpty = false;
  for (int i = 0; i < length; i++) {
    if (!strcmp(memory[i], "")) {
      isEmpty = true;
    } else {
      isEmpty = false;
      break;
    }
  }
  return isEmpty;
}


int nf_allocate(char **memory, int memory_size, char *process_id, int size, int next_index)
{
  int NEXT_INDEX;
  int return_index;
  if(isMemEmpty(memory, memory_size)) {
    NEXT_INDEX = 0;
  } else {
    NEXT_INDEX = next_index;
  }
  
  int currentSize = 0;
  int start = 0;
  bool hasStarted = false;
  bool found = false;

  for (int i = NEXT_INDEX; i < memory_size; i++) {
    if (!strcmp(memory[i], "")) {
      if (!hasStarted) {
        hasStarted = true;
        start = i;
      }

      currentSize++;

      if (currentSize == size) {
        found = true;
        break;
      }
    } else {
      hasStarted = false;
      currentSize = 0;
      found = false;
    }
  }

  if (found) {
    for (int i = start; i < start + size; i++) {
      strcpy(memory[i], process_id);
    }
    printf("ALLOCATED %s %i \n", process_id, start);
    return start + size;
  } else {
    // Loop back around if the space wasn't found from the next index spot
    currentSize = 0;
    start = 0;
    hasStarted = false;

    for (int i = 0; i < NEXT_INDEX; i++) {
      if (!strcmp(memory[i], "")) {
        if (!hasStarted) {
          hasStarted = true;
          start = i;
        }

        currentSize++;

        if (currentSize == size) {
          found = true;
          break;
        }
      } else {
        hasStarted = false;
        currentSize = 0;
        found = false;
      }
    }
    
    if (found) {
      for (int i = start; i < start + size; i++) {
        strcpy(memory[i], process_id);
      }
      printf("ALLOCATED %s %i \n", process_id, start);
      return start + size;
    } else {
      printf("FAIL REQUEST %s %i \n", process_id, size);
    }
  }

  return next_index;
}