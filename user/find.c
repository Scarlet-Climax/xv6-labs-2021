#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*
Write a simple version of the UNIX find program: find all the files in a directory tree with a specific name. Your solution should be in the file user/find.c.
*/
char *fname;
void statdir(char *path, int flag) {
  int fd;
  struct stat st;
  char buf[512], *p;
  struct dirent de;
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    goto err1;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    goto err2;
  }
  switch (st.type) {
    case T_FILE:
      if (!flag)
        printf("%s\n", path);
      break;
    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        fprintf(2, "find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;
        if (strcmp(de.name, ".") == 0 ||
            strcmp(de.name, "..") == 0)
          continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        statdir(buf, strcmp(de.name, fname));
      }
      break;
  }
err2:
  close(fd);
err1:
  return;
}
int main(int argc, char *argv[]) {
  if (argc <= 2) {
    fprintf(2, "find: must specify dir and name\n");
    exit(1);
  } else {
    fname = argv[2];
    statdir(argv[1], strcmp(argv[1], fname));
  }
  exit(0);
}