#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char* argv[])
{
  // fd[0] for reading
  // fd[1] for writing
  int fd[2];
  pipe(fd);
  // child (receiver -> sender)
  // parent (sender -> receiver)
  char buf_child[1];
  char buf_parent[1];
  char send_parent = 'A';
  if(fork() == 0) {
    if(read(fd[0], buf_child, 1) != 1) {
      fprintf(2, "Failed to receive from parent");
      exit(1);
    }
    close(fd[0]);
    printf("%d: received ping\n",getpid());
    if(write(fd[1], buf_child, 1) != 1) {
      fprintf(2, "Failed to write to parent");
      exit(1);
    }
    close(fd[1]);
    exit(0);
  }
  // parent side
  if(write(fd[1], &send_parent, 1) != 1) {
    fprintf(2, "Failed to write to child");
    exit(1);
  }
  close(fd[1]);
  // wait for child to send data (terminate)
  wait(0); 
  if(read(fd[0], buf_parent, 1) != 1) {
    fprintf(2, "Failed to read from child");
    exit(1);
  }
  printf("%d: received pong\n",getpid());
  close(fd[0]);
  exit(0);
}