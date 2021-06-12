#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int i;
  for (i = 1; i < argc; i++) {
    printf("%s", argv[i]);
    printf("%s", " ");
  }
  printf("\n");
  exit(0);
}