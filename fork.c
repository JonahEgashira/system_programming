#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0) {
    printf("child\n");
    sleep(10);
  } else {
    printf("parent \n\t the pid of child process is %d\n", pid);
    printf("waiting for dying of child\n");
    wait(&status);
    printf("detecting the child death\n");
  }
}