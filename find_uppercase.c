#include <stdio.h>

char find_upper(char *str) {
  if (*str == '\0')
    return '\0';
  if (*str >= 'A' && *str <= 'Z')
    return *str;
  return find_upper(str + 1);
}

int main() {
  char text[1000];
  printf("enter a text: ");
  fgets(text, sizeof(text), stdin);

  char result = find_upper(text);

  if (result != '\0')
    printf("the first uppercase letter is: %c\n", result);
  else
    printf("No uppercase letter found\n");

  return 0;
}
