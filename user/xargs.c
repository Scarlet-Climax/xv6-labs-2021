#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
void Run(char* argv[]) {
  // printf("DEBUG: Running: ");
  // for (char** a = argv; *a; a++) {
  //     printf("%s ", *a);
  // }
  // printf("\n");
  if (fork() > 0) {
    wait(0);
    return;
  } else {
    exec(argv[0], argv);
  }
}

char* nextarg(char** bufp) {  // if eol return 0;
  char c, *st = *bufp, *ret = 0;
  while ((c = **bufp)) {
    if (c == ' ') {
      *(*bufp)++ = 0;
      return ret;
    }
    *(*bufp)++ = c;
    ret = st;
  }
  return ret;
}
int nextline(char buf[]) {  // if eof return 0;
  char c;
  int st = 0;
  while (read(0, &c, 1)) {
    if (c == '\n') {
      *buf++ = 0;
      return st;
    }
    *buf++ = c;
    st = 1;
  }
  return st;
}
int main(int argc, char* argv[]) {
  if (argc <= 2) {
    fprintf(2, "xargs: must specify the command\n");
    exit(1);
  } else {
    char *argvn[MAXARG], buf[512], *p;
    int pos = argc - 1;
    memmove(argvn, argv + 1, sizeof(char*) * (argc - 1));
    while (nextline(buf)) {
      p = buf;
      pos = argc - 1;
      for (; (argvn[pos] = nextarg(&p));) {
        // printf("DEBUG: argvn[%d]=%s\n", pos, argvn[pos]);
        pos++;
      }
      Run(argvn);
    }
  }
  exit(0);
}
