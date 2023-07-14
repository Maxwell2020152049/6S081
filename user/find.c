#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path, char* file)
{
  char buf[512], *p;
  char tmp1[512], tmp2[512], tmp3[512];
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
    strcpy(tmp1, fmtname(path));
    strcpy(tmp2, fmtname(file));

    if (strcmp(tmp1, tmp2) == 0)
    {
      printf("%s\n", buf);
    }
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      strcpy(tmp1, fmtname(buf));
      strcpy(tmp2, fmtname("."));
      strcpy(tmp3, fmtname(".."));

      if (strcmp(tmp1, tmp2) && strcmp(tmp1, tmp3))
      {
        printf("%s %s\n", buf, file);
        find(buf, file);
      }

      // strcpy(tmp1, fmtname(buf));
      // strcpy(tmp2, fmtname(file));

      // if (strcmp(tmp1, tmp2) == 0)
      // {
      //   printf("%s\n", buf);
      // }

      // if (strcmp(fmtname(buf), ".") && strcmp(fmtname(buf), ".."))
      //   find(buf, file);
      // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
      // printf("file = %s\n", file);
      // printf("%d %d\n", strlen(fmtname(buf)), strlen(file));
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: number of arguments must be three.\n");
    fprintf(2, "type as: find {dir_name} {file_name}\n");

    exit(1);
  }
  
  find(argv[1], argv[2]);

  exit(0);
}
