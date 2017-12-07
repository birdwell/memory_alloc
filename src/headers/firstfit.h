#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef _firstfit_h
#define _firstfit_h

void ff_allocate(char **memory, int memory_size, char *process_id, int size) {
  int currentSize = 0;
  int start = 0;
  bool hasStarted = false;
  bool found = false;

  for (size_t i = 0; i < memory_size; i++)
  {
    if(!strcmp(memory[i],"")) {
      if (!hasStarted) {
        hasStarted = true;
        start = i;
      }

      currentSize++;

      if(currentSize == size) {
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

    // printf("current size: %i, start: %i \n", currentSize, start);
    for (int i = start; i < start + size; i++)
    {
      strcpy(memory[i], process_id);
    }
    printf("ALLOCATED %s %i \n", process_id, start);
  } else {
    printf("FAIL REQUEST %s %i \n", process_id, size);
  }
}

#endif // process