#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void Pipe(int p[]) {
  if (pipe(p) < 0) {
    printf("pingpong: pipe failed\n");
    exit(1);
  }
}

int main() {
  int pipeL[2];
  Pipe(pipeL);
  if (fork() > 0) {
    close(pipeL[0]);
    for (int i = 2; i <= 35; i++) {
      write(pipeL[1], &i, 4);
    }
    close(pipeL[1]);
    wait(0);
    exit(0);
  } else {
    close(pipeL[1]);
    for (;;) {
      int p;
      if (read(pipeL[0], &p, 4)) {
        printf("prime %d\n", p);
      } else {
        close(pipeL[0]);
        exit(0);
      }
      int pipeR[2];
      Pipe(pipeR);
      if (fork() > 0) {
        int i;
        while (read(pipeL[0], &i, 4)) {
          if (i % p != 0) {
            write(pipeR[1], &i, 4);
          }
        }
        close(pipeL[0]);
        close(pipeR[1]);
        wait(0);
        exit(0);
      } else {
        close(pipeL[0]);
        close(pipeR[1]);
        pipeL[0] = pipeR[0];
      }
    }
  }
}