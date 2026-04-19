

#ifndef LIST_H
#define LIST_H

struct Node {
  int data;
  struct Node *next;
};



struct Node *createNode(int data);

void addNode(struct Node **head, int data);

void addNodeAtBeginning(struct Node **head, int data);

void addNodeAfter(struct Node *prevNode, int data);

void displayList(struct Node *head);

int search(struct Node *head, int key);

void freeList(struct Node **head);

#endif
