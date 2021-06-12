#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *paths[] = {"/bin/", "/usr/bin/", NULL};

int main() {
  char buf[1024];
  char *cmdv[100];
  char path0[1024];
  char *cp;
  char *tp;
  int i, index;
  int status;

  do {
    for (i = 0; i < 100; i++) {
      cmdv[i] = NULL;
    }
    printf("%% ");
    cp = fgets(buf, sizeof(buf), stdin);

    for (i = 0; i < 100; i++) {
      /* find word */
      while (*cp == ' ') cp++;
      tp = cp;
      while (*tp != ' ' && *tp != '\0' && *tp != '\n') tp++;
      if (*tp == '\0' || *tp == '\n') {
        *tp = '\0';
        cmdv[i] = cp;
        cmdv[++i] = NULL;
        break;
      }
      *tp = '\0';

      cmdv[i] = cp;
      if (cmdv[i] == NULL) break;
      cp = ++tp;
    }

    if (!strcmp("exit", cmdv[0])) {
      fprintf(stderr, "Terminating Shell\n");
      exit(0);
    }

    if (fork() == 0) {
      if (*cmdv[0] == '/') {
        /* */
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
      if (*cmdv[0] == '.') {
        /* */
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

      index = 0;
      do {
        strcpy(path0, paths[index++]);
        strcat(path0, cmdv[0]);
        /* */
        fprintf(stderr, "Path: %s\n", path0);
        for (i = 0; i < 100; i++) {
          if (cmdv[i] == NULL) continue;
          fprintf(stderr, "argv[%d]= %s\n", i, cmdv[i]);
        }

        if (execv(path0, cmdv) < 0) {
          fprintf(stderr, "Exec fail\n");
        }
      } while (paths[index] != NULL);

    } else {
      wait(&status);
    }
  } while (1);
}
