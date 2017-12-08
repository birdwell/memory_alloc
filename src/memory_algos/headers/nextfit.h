#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef _nextfit_h
#define _nextfit_h

bool isMemEmpty(char **memory, int length)
{
  bool isEmpty = false;
  for (int i = 0; i < length; i++)
  {
    if (!strcmp(memory[i], ""))
    {
      isEmpty = true;
    }
    else
    {
      isEmpty = false;
      break;
    }
  }

  return isEmpty;
}

void find_nextfit() {

}

int nf_allocate(char **memory, int memory_size, char *process_id, int process_size, int next_index)
{
  int NEXT_INDEX;
  int return_index;
  if(isMemEmpty(memory, memory_size)) {
    NEXT_INDEX = 0;
  } else {
    NEXT_INDEX = next_index;
  }

  int nf_start = 0;
  int nf_size = 0;
  int nf_diff = 999999999;
  int p_size = process_size;
  bool is_memory_empty = isMemEmpty(memory, memory_size);

  int temp_start = 0;
  int temp_size = 0;
  bool currently_tracking = false;
  char last[16];

  strcpy(last, "empty");
  for (int i = NEXT_INDEX; i < memory_size; i++)
  {
    // Count up size if memory is empty
    // If current symbol is allocated and last wasn't allocated test
    if (!strcmp(memory[i], ""))
    {
      if (!currently_tracking)
      {
        currently_tracking = true;
        temp_start = i;
        // printf("CURRENTLY TRACKING SET ------------ tm_size: %i, tm_start: %i \n", temp_size, temp_start);
      }

      temp_size++;
      strcpy(last, memory[i]);
    }
    else if (strcmp(memory[i], "") && !strcmp(last, ""))
    {
      int temp_diff = temp_size - process_size;
      if (temp_size >= process_size && (temp_diff < nf_diff))
      {
        // printf("tm_size: %i, tm_start: %i \n", temp_size, temp_start);
        nf_start = temp_start;
        nf_size = temp_size;
        nf_diff = temp_diff;
      }

      currently_tracking = false;
      temp_size = 0;
      strcpy(last, memory[i]);
    }
    else if (strcmp(memory[i], ""))
    {
      temp_size = 0;
      currently_tracking = false;
      strcpy(last, memory[i]);
    }
  }

  if (temp_size >= process_size && ((temp_size - process_size) < nf_diff))
  {
    // printf(" LAST SEC ASSIGNMENT ");
    // printf("tm_size: %i, tm_start: %i \n", temp_size, temp_start);

    nf_start = temp_start;
    nf_size = temp_size;
    nf_diff = temp_size - process_size;
  }

    // Wrap to start search again only if it wasn't found
  if(nf_size == 0 && !is_memory_empty) {
    nf_diff = 999999999;
    nf_start = 0;
    temp_start = 0;
    temp_size = 0;
    currently_tracking = false;
    strcpy(last, "empty");
    for (int i = 0; i < NEXT_INDEX; i++)
    {
      // Count up size if memory is empty
      // If current symbol is allocated and last wasn't allocated test
      if (!strcmp(memory[i], ""))
      {
        if (!currently_tracking)
        {
          currently_tracking = true;
          temp_start = i;
          // printf("CURRENTLY TRACKING SET ------------ tm_size: %i, tm_start: %i \n", temp_size, temp_start);
        }

        temp_size++;
        strcpy(last, memory[i]);
      }
      else if (strcmp(memory[i], "") && !strcmp(last, ""))
      {
        int temp_diff = temp_size - process_size;
        if (temp_size >= process_size && (temp_diff < nf_diff))
        {
          // printf("tm_size: %i, tm_start: %i \n", temp_size, temp_start);
          nf_start = temp_start;
          nf_size = temp_size;
          nf_diff = temp_diff;
        }

        currently_tracking = false;
        temp_size = 0;
        strcpy(last, memory[i]);
      }
      else if (strcmp(memory[i], ""))
      {
        temp_size = 0;
        currently_tracking = false;
        strcpy(last, memory[i]);
      }
    }
  }

  if (is_memory_empty)
  {
    for (int i = 0; i < p_size; i++)
    {
      strcpy(memory[i], process_id);
    }
    printf("ALLOCATED %s %i \n", process_id, 0);
    is_memory_empty = false;
    return_index = p_size;
  }
  else if (nf_size > 0 && nf_size >= process_size)
  {
    // printf("nf_size  %i, nf_start %i, nf_end %i \n", nf_size, nf_start, nf_start + p_size);
    for (size_t i = nf_start; i < nf_start + p_size; i++)
    {
      strcpy(memory[i], process_id);
    }
    printf("ALLOCATED %s %i \n", process_id, nf_start);
    return_index = nf_start + p_size;
  }
  else if (nf_size == 0)
  {
    printf("FAIL REQUEST %s %i \n", process_id, p_size);
  }

  return return_index;
}

#endif