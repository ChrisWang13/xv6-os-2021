#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" // MAXARG

// Read lines from the standard input, one char
// at a time, fork and exec when meet '\n', 
// Example: find . b | xargs grep hello
//        : sh < xargstest.sh
//        : echo ff ff ff | xargs echo ee ee ee

// Usage of xargs: cmd1 ... | xargs cmd2 ...
// Output of cmd1...:
// val1
// val2
// val3
// Output of xargs: cmd2 ... val1 + cmd2 ... val2 + cmd2 ... val3

int 
main(int argc, char* argv[])
{
  char buf;
  char* back_argv[MAXARG];
	char read_input_line[20];
  char* cmd = argv[1];

  int index_back_argv = 0;
	int index_col = 0;

  while(read(0, &buf, 1) == 1) { 
    // buf = '\n' <==> left input is finished.
    // buf = ' '  <==> same with other characters.
		if(buf == '\n') {
			// 1. Copy from argv[1] to argv[argc - 1]
      // to back_argv[0 ... ].
			for(int i = 1; i < argc; ++i) {
      	back_argv[index_back_argv++] = argv[i];
      }
			// 2. Append the backlines.
			back_argv[index_back_argv] = malloc(strlen(read_input_line) + 1);
      strcpy(back_argv[index_back_argv], read_input_line);
			back_argv[++index_back_argv] = 0;
			// exec implementation
			// for(argc = 0; argv[argc]; ++argc)
			if(fork() == 0) {
				exec(cmd, back_argv);
				exit(0);
			}
			wait(0);	
			// init for next '\n' line
			index_col = 0;  
			index_back_argv = 0;	
			memset(read_input_line, 0, sizeof read_input_line);
		}
    else read_input_line[index_col++] = buf;
  }
  exit(0);
}
