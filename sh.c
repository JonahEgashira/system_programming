#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char buf[1024];
int fds[2] = {};
char *cmdv[100];
char *cmds[100][1024];
char *cp;
char *tp;
int i, j, k;
int cmd_n;
int status;
pid_t ret;

void parse_input() {
  j = 0;
  k = 0;
  for (i = 0; i < 100; i++) {
    /* find word */
    while (*cp == ' ') cp++;
    tp = cp;
    while (*tp != ' ' && *tp != '\0' && *tp != '\n') tp++;

    if (*tp == '\0' || *tp == '\n') {
      *tp = '\0';
      cmds[j][k] = cp;
      cmds[j][++k] = NULL;
      cmdv[i] = cp;
      cmdv[++i] = NULL;
      break;
    }
    *tp = '\0';

    /* pipe */
    if (*cp == '|') {
      j++;
      cmds[j][k] = NULL;
      k = 0;
      cp = ++tp;
      continue;
    }

    cmdv[i] = cp;
    cmds[j][k++] = cp;
    if (cmdv[i] == NULL) break;
    cp = ++tp;
  }

  cmd_n = j + 1;
}

void func(int depth) {
  /* exec commands */
  if (depth == cmd_n - 1) {
    if (execvp(cmds[0][0], cmds[0]) < 0) {
      /* error */
      fprintf(stderr, "Exec fail\n");
    }
  } else {
    pipe(fds);

    if (fork() == 0) {
      close(fds[0]);
      dup2(fds[1], 1);
      close(fds[1]);

      func(depth + 1);
    } else {
      close(fds[1]);
      dup2(fds[0], 0);
      close(fds[0]);

      if (execvp(cmds[cmd_n - depth - 1][0], cmds[cmd_n - depth - 1]) < 0) {
        /* error */
        fprintf(stderr, "Exec fail\n");
      }
    }
  }
}

void init_shell() {
  printf("\033[H\033[J");
  printf("\n\n\n\n**********************************");
  printf("\n\n\n\t*****SEA SHELL*****");
  printf("\n\n\n\n**********************************");
  printf("\n\n");
  printf("Welcome!\n");
}

int main() {
  init_shell();
  do {
    /* wait for user input */
    for (i = 0; i < 100; i++) {
      cmdv[i] = NULL;
      for (j = 0; j < 1024; j++) {
        cmds[i][j] = NULL;
      }
    }
    printf("%% ");
    cp = fgets(buf, sizeof(buf), stdin);

    parse_input();
    if (!strcmp("exit", cmds[0][0])) {
      fprintf(stderr, "Terminating Shell\n");
      exit(0);
    }

    ret = fork();

    if (ret == 0) {
      func(0);
    } else {
      wait(&status);
    }

  } while (1);
}
