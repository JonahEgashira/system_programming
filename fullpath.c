#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define ull unsigned long long

char fullPath[100][1024];
char searchPath[1024];
int depth;

void rec(ull child_ino, int d) {
  struct dirent *pDirent;
  DIR *pDir;
  int root = 0;

  strcat(searchPath, "../");
  pDir = opendir(searchPath);
  ull current_ino = 0;

  while ((pDirent = readdir(pDir)) != NULL) {
    if (strcmp(pDirent->d_name, ".") == 0 && pDirent->d_ino == 2) {
      root = 1;
    }
    if (strcmp(pDirent->d_name, ".") == 0) {
      current_ino = pDirent->d_ino;
    }
  }
  closedir(pDir);

  pDir = opendir(searchPath);

  while ((pDirent = readdir(pDir)) != NULL) {
    if (pDirent->d_ino == child_ino) {
      strcpy(fullPath[d], pDirent->d_name);
      if (root) {
        break;
      }
      rec(current_ino, d + 1);
    }
  }
  closedir(pDir);
  if (root) {
    depth = d;
    return;
  }
}

int main(int argc, char *argv[]) {
  struct dirent *pDirent;
  DIR *pDir;

  ull child_ino = 0;
  pDir = opendir(".");
  while ((pDirent = readdir(pDir)) != NULL) {
    if (pDirent->d_name[0] == '.') {
      child_ino = pDirent->d_ino;
      break;
    }
  }

  rec(child_ino, 0);
  printf("Full Path: ");
  for (int i = depth; i >= 0; i--) {
    printf("/");
    printf("%s", fullPath[i]);
  }
  printf("\n");
}