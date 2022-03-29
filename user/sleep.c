#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char* argv[])
{
  if(isNumber(argv[1]) == 0) {
    fprintf(2, "Invalid input\n");
    exit(1);
  }
  int time = atoi(argv[1]);
  sleep(time);
  exit(0);
}