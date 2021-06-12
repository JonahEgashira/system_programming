#include <stdio.h>
#include <unistd.h>

char *cmd1[] = {"cat", "hoge", NULL};
char *cmd2[] = {"ls", NULL};
char **cmds[] = {cmd1, cmd2};

int main() {
  int i, j;
  for (i = 0; i < 2; i++) {
    for (j = 0; cmds[i][j] != NULL; j++) {
      printf("%s\n", cmds[i][j]);
    }
  }
}