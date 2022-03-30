#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// sieveProc reads from left process one 
// integer (int p), print p, then read 
// the rest all numbers (n) from left process (Line ?)
// if(n % p != 0) send n to right_fd[1], 
// by calling new fork, and creating right_fd.

void 
sieveProc(int *left_fd)
{
  close(left_fd[1]);  // close left-side write.
  int p;              // first prime to print.
  int n;              // rest all numbers.
  int right_fd[2];
  // failed to read new data, exit.
  if(read(left_fd[0], &p, 4) != 4) {
    exit(1);
  }
  // print prime p in the current process.
  printf("prime %d\n", p);
  // create new pipe and fork the right process.
  pipe(right_fd);
  if(fork() == 0) {
    sieveProc(right_fd);
    exit(0);
  }
  // parent first read from left_fd[0]
  // then write to right_fd[1].
  close(right_fd[0]);
  while(read(left_fd[0], &n, 4) != 0) {
    if(n % p != 0) {
      write(right_fd[1], &n, 4);
    }
  }
  close(left_fd[0]);
  close(right_fd[1]);
  wait(0); 
  exit(0);
}


int 
main(int argc, char* argv[])
{
  int fd[2];
  pipe(fd);
  if(fork() == 0) {
    sieveProc(fd);
    exit(0);
  }
  // parent feed 2 to 35.
  close(fd[0]);
  for(int i = 2; i <= 35 ; ++i) {
    write(fd[1], &i, 4);
  }
  close(fd[1]);
  // initial parent process wait for
  // all subprocess to terminate.
  wait(0);
  exit(0);
}