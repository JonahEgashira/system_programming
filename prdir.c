#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct dirent *pDirent;
  DIR *pDir;

  if (argc != 2) {
    printf("Usage: testprog <dirname>\n");
    return 1;
  }

  pDir = opendir(argv[1]);
  if (pDir == NULL) {
    printf("Error opening Directory '%s'\n", argv[1]);
    return 1;
  }
  while ((pDirent = readdir(pDir)) != NULL) {
    printf("%s %llu\n", pDirent->d_name, pDirent->d_ino);
  }
  closedir(pDir);
  return 0;
}