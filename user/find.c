#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
    static char *p;
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}

void
find(char *path, char *fname)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;


    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_DEVICE:
    case T_FILE:
        fprintf(2, "require an directory path\n");
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
        }
        // make path of each directory and file will be relative with first dir
        // Ex: mkdir lambt9, echo > lambt9/test.txt
        // File name will be: ./lambt9/test.txt (Append '/' to end of dir path)
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

        // Compare file name want to find with file in each folder
        if (strcmp(fmtname(buf), fname) == 0) {
            printf("%s\n", buf);
        }
        if (st.type == T_DIR) {
            if ((strcmp(fmtname(buf), ".") == 0) || (strcmp(fmtname(buf), "..") == 0)) {
                continue;
            }
            find(buf, fname);
        }
        }
        break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "Usage: find [dirpath] [filename]");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
