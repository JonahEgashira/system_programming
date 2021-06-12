#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd, newfd;

  fd = open(argv[1], O_CREAT | O_RDWR, 0644);
  printf("%d", fd);
  if (fd < 0) {
    perror("open");
    exit(-1);
  }

  close(1);

  newfd = dup(fd);
  if (newfd < 0) {
    perror("dup");
    exit(-1);
  }
  printf("I'm writing data onto the stdout (File Descriptor #1)\n");
}