#include <stdio.h>

int digits(int num) {
  if (num == 0)
    return 0;

  return 1 + digits(num / 10);
}

int main() {
  int num;
  printf("enter number :");
  scanf("%d", &num);
  printf("the number of digits of %d is : %d \n", num, digits(num));
  return 0;
}
