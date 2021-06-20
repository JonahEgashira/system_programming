#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  char pathname[1024];

  memset(pathname, '\0', sizeof(pathname));

  getcwd(pathname, 1024);
  fprintf(stdout, "%s\n", pathname);
}