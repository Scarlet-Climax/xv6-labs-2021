#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
Write a program that uses UNIX system calls to ''ping-pong'' a byte between two processes over a pair of pipes, one for each direction.
The parent should send a byte to the child;
the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit;
the parent should read the byte from the child, print "<pid>: received pong", and exit.
*/

int main() {
  int p2c[2], c2p[2];
  if (pipe(p2c) < 0 || pipe(c2p) < 0) {
    printf("pingpong: pipe failed\n");
    exit(1);
  }
  int pid = fork();
  if (pid > 0) {  // parent
    close(p2c[0]);
    close(c2p[1]);
    write(p2c[1], "a", 1);
    close(p2c[1]);
    read(c2p[0], 0, 1);
    close(c2p[0]);
    printf("%d: received pong\n", getpid());
  } else {
    close(p2c[1]);
    close(c2p[0]);
    read(p2c[0], 0, 1);
    close(p2c[0]);
    printf("%d: received ping\n", getpid());
    write(p2c[1], "a", 1);
    close(p2c[1]);
  }
  exit(0);
}