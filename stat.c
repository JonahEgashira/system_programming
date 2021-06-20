#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  struct stat sbuf;

  if (argc == 1) {
    printf("%s filename\n", argv[0]);
    exit(0);
  }
  if (stat(argv[1], &sbuf) < 0) {
    perror("stat");
    exit(-1);
  }
  printf("mode: 0x%o\n", sbuf.st_mode);
}