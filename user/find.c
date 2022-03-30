#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
locate_File_Name(char *path)
{
  char *p;
  // Find first character after last slash.
  // p initalize in end of file.
  for(p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  return p + 1;
}

// Example
// 1. find . b
// 2. find /a b
void
find(char *path, char* filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(strcmp(locate_File_Name(path), filename) == 0) {
      printf("%s\n", path);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
      // 1. avoid recursing into . and .. directory.
      // 2. when de.inum = 0, no need to search.
      if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;       // mark as end of file.
      // within the loop, recurse to add all dirents.
      find(buf, filename); 
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3) {
    fprintf(2, "invalid input");
  }
  find(argv[1], argv[2]);
  exit(0);
}


