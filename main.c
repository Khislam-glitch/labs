#include "list.h"
#include <stdio.h>

int main() {
  struct Node *head = NULL;

  addNode(&head, 10);           // Adding 10 to the end
  addNodeAtBeginning(&head, 5); // Adding 5 at the beginning
  addNodeAfter(head->next, 7);  // Adding 7 after the first element
  addNode(&head, 20);           // Adding 20 to the end

  printf(" Linked List : ");
  displayList(head);

  int key = 20;
  if (search(head, key))
    printf("%d found in the list .\n", key);
  else
    printf("%d not found in the list .\n", key);

  freeList(&head);

  if (head == NULL)
    printf("List successfully freed!\n");

  return 0;
}
