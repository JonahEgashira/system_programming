#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pid;
  int status;
  int fd;

  pid = fork();
  if (pid == 0) {
    close(1);
    fd = open("foo", O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
      fprintf(stderr, "Can not open: foo\n");
      exit(-1);
    }

    execl("/bin/ls", "ls");
    exit(0);
  }
  wait(&status);
  printf("I'm Parent, child PID is %d\n", pid);
}