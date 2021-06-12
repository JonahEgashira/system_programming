#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *paths[] = {"/bin/", "/usr/bin/", NULL};
  char path0[1024];
  int index = 0;
  while (paths[index] != NULL) {
    strcpy(path0, paths[index]);
    printf("%s\n", path0);
    index++;
  }
}