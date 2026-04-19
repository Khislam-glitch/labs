
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

struct Node *createNode(int data) {
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  if (newNode == NULL) {
    fprintf(stderr, "[ERROR] createNode: memory allocation failed.\n");
    return NULL;
  }
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

void addNode(struct Node **head, int data) {
  if (head == NULL) {
    fprintf(stderr, "[ERROR] addNode: head pointer is NULL.\n");
    return;
  }
  struct Node *newNode = createNode(data);
  if (newNode == NULL)
    return;

  if (*head == NULL) {

    *head = newNode;
    return;
  }

  /* Walk to the last node */
  struct Node *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = newNode;
}

void addNodeAtBeginning(struct Node **head, int data) {
  if (head == NULL) {
    fprintf(stderr, "[ERROR] addNodeAtBeginning: head pointer is NULL.\n");
    return;
  }
  struct Node *newNode = createNode(data);
  if (newNode == NULL)
    return;

  newNode->next = *head;
  *head = newNode;
}

void addNodeAfter(struct Node *prevNode, int data) {
  if (prevNode == NULL) {
    fprintf(stderr, "[ERROR] addNodeAfter: prevNode is NULL.\n");
    return;
  }
  struct Node *newNode = createNode(data);
  if (newNode == NULL)
    return;

  newNode->next = prevNode->next;
  prevNode->next = newNode;
}

void displayList(struct Node *head) {
  if (head == NULL) {
    printf("List is empty.\n");
    return;
  }
  printf("HEAD -> ");
  struct Node *current = head;
  while (current != NULL) {
    printf("[%d]", current->data);
    if (current->next != NULL)
      printf(" -> ");
    current = current->next;
  }
  printf(" -> NULL\n");
}

int search(struct Node *head, int key) {
  struct Node *current = head;
  while (current != NULL) {
    if (current->data == key)
      return 1;
    current = current->next;
  }
  return 0;
}

void freeList(struct Node **head) {
  if (head == NULL)
    return;
  struct Node *current = *head;
  while (current != NULL) {
    struct Node *temp = current->next;
    free(current);
    current = temp;
  }
  *head = NULL;
}
