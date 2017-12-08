#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./headers/bestfit.h"

bool isEmpty (char **memory, int length) {
  bool isEmpty = false;
  for (int i = 0; i < length; i++)
  {
    if(!strcmp(memory[i],"")) {
      isEmpty = true;
    } else {
      isEmpty = false;
      break;
    }
  }

  return isEmpty;
}

void bf_allocate(char **memory, int memory_size, char *process_id, int process_size) {
  int bf_start = 0;
  int bf_size = 0;
  int bf_diff = 999999999;
  int p_size = process_size;
  bool is_memory_empty = isEmpty(memory, memory_size);

  int temp_start = 0;
  int temp_size = 0;
  bool currently_tracking = false;
  char last[16];

  if (is_memory_empty) {
    for (int i = 0; i < p_size; i++) {
      strcpy(memory[i], process_id);
    }
    printf("ALLOCATED %s %i \n", process_id, 0);
    is_memory_empty = false;
    return;
  }

  strcpy(last, "empty");
  for (int i = 0; i < memory_size; i++)
  {
    // Count up size if memory is empty
    // If current symbol is allocated and last wasn't allocated test
    if (!strcmp(memory[i], ""))
    {
      if (!currently_tracking) {
        currently_tracking = true;
        temp_start = i;
      }

      temp_size++;
      strcpy(last, memory[i]);
    } else if (strcmp(memory[i], "") && !strcmp(last, "")) {
      // This marks the end of an empty slot and the start of one that has been allocated.
      int temp_diff = temp_size - process_size;
      if (temp_size >= process_size && (temp_diff < bf_diff)) {
        bf_start = temp_start;
        bf_size = temp_size;
        bf_diff = temp_diff;
      }

      currently_tracking = false;
      temp_size = 0;
      strcpy(last, memory[i]);
    } else if (strcmp(memory[i], "")) {
      // This is going through an allocation.
      temp_size = 0;
      currently_tracking = false;
      strcpy(last, memory[i]);
    } 
  }

  // Make sure there wasn't a best fit slot at the end of the array
  if (temp_size >= process_size && ((temp_size - process_size) < bf_diff)) {
    bf_start = temp_start;
    bf_size = temp_size;
    bf_diff = temp_size - process_size;
  }

  if (bf_size > 0 && bf_size >= process_size) {
    for (size_t i = bf_start; i < bf_start + p_size; i++) {
      strcpy(memory[i], process_id);
    }
    printf("ALLOCATED %s %i \n", process_id, bf_start);
  } else if (bf_size == 0) {
    printf("FAIL REQUEST %s %i \n", process_id, p_size);
  }
}