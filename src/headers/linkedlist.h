#ifndef _linkedlist_h
#define _linkedlist_h
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
  struct Process process;
  struct node *next;
} node_t;

void print_list(node_t *head)
{
  node_t *current = head;

  while (current != NULL)
  {
    printf("%s\n", current->process.id);
    current = current->next;
  }
}

void push(node_t *head, struct Process process)
{
  node_t *current = head;
  while (current->next != NULL)
  {
    current = current->next;
  }

  /* now we can add a new variable */
  current->next = malloc(sizeof(node_t));
  current->next->process = process;
  current->next->next = NULL;
}

int remove(node_t **head, char tag)
{
  int i = 0;
  int retval = -1;
  node_t *current = *head;
  node_t *temp_node = NULL;

  for (int i = 0; i < n - 1; i++)
  {
    if (current->next == NULL)
    {
      return -1;
    }
    current = current->next;
  }

  temp_node = current->next;
  retval = temp_node->val;
  current->next = temp_node->next;
  free(temp_node);

  return retval;
}

#endif 