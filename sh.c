#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *paths[] = {"/bin/", "/usr/bin/", NULL};
char buf[1024];
int fds[2] = {};
char *cmdv[100];
char *tmp[100];
char *cmds[100][1024];
char path0[1024];
char *cp;
char *tp;
int i, j, k, idx;
int status;

void func(int depth) {
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
    if (*cp == '>') {
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

  for (i = 0; i < 5; i++) {
    for (j = 0; cmds[i][j] != NULL; j++) {
      printf("cmds%d: %s\n", i, cmds[i][j]);
    }
  }

  if (!strcmp("exit", cmdv[0])) {
    fprintf(stderr, "Terminating Shell\n");
    exit(0);
  }

  if (fork() == 0) {
    /* child */
    if (*cmdv[0] == '/') {
      /* absolute path */
      fprintf(stderr, "Path: %s\n", cmdv[0]);
      for (i = 0; i < 100; i++) {
        if (cmdv[i] == NULL) continue;
        fprintf(stderr, "argv[%d]= %s\n", i, cmdv[i]);
      }

      if (execv(cmdv[0], cmdv) < 0) {
        /* not successful */
        fprintf(stderr, "Exec fail: path: %s\n", cmdv[0]);
        exit(1);
      }
    }

    if (*cmdv[0] == '.') {
      /* relative path */
      fprintf(stderr, "Path: %s\n", cmdv[0]);
      for (i = 0; i < 100; i++) {
        if (cmdv[i] == NULL) continue;
        fprintf(stderr, "argv[%d]= %s\n", i, cmdv[i]);
      }

      if (execv(cmdv[0], cmdv) < 0) {
        fprintf(stderr, "Exec fail: path: %s\n", cmdv[0]);
        exit(1);
      }
    }

    idx = 0;

    do {
      /* check all paths */
      strcpy(path0, paths[idx++]);
      strcat(path0, cmdv[0]);
      fprintf(stderr, "Path: %s\n", path0);
      for (i = 0; i < 100; i++) {
        if (cmdv[i] == NULL) continue;
        fprintf(stderr, "argv[%d]= %s\n", i, cmdv[i]);
      }

      /* exec commands */

      if (execv(path0, cmdv) < 0) {
        /* error */
        fprintf(stderr, "Exec fail\n");
      }
    } while (paths[idx] != NULL);

  } else {
    wait(&status);
  }
}

int main() {
  do {
    /* wait for user input */
    for (i = 0; i < 100; i++) {
      cmdv[i] = NULL;
    }
    printf("%% ");
    cp = fgets(buf, sizeof(buf), stdin);

    func(0);

  } while (1);
}
