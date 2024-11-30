#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/fcntl.h"


 /* xargs execute command with all input*/

 void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}


int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

int
main(int argc, char *argv[])
{
    int i;
    int is_input = 0;
    char *xargs_argv[MAXARG];
    int xargc;

    char c;
    int j = 0;
    char input[128] = {0};

    xargs_argv[0] = argv[1];
    for(i = 2; i < argc; i++){
        xargs_argv[i - 1] = argv[i];
    }
    xargc = i - 1;
    //read input from stdin
    while(read(0, &c, 1) == 1) {
        is_input = 1;
        if (c != '\n') {
            input[j++] = c;
        }
        else {
            input[j] = '\0';
            // printf("Get input: %s\n", input);
            xargs_argv[xargc] = input;
            if (fork1() == 0) {
                exec(xargs_argv[0], xargs_argv);
            }
            wait(0);
            memset(input, 0, 128);
            j = 0;
        }
    }

    if (is_input == 0) {
        exec(xargs_argv[0], xargs_argv);
    }
    exit(0);
}