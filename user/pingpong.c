#include "kernel/types.h"
#include "user/user.h"

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

/* Parent send, child receive*/

int
main(int argc, char *argv[]) 
{
    int pid;
    int pfd1[2];
    int pfd2[2];
    char buf[5];
    if ((pipe(pfd1) < 0) || (pipe(pfd2)) < 0 ) {
        panic("Failed to create pipe\n");
    }    

    pid = fork();
    if (pid == -1)
        panic("fork");

    if (pid == 0) {
        // child
        close(pfd1[1]);
        close(pfd2[0]);
        if (read(pfd1[0], (char *)buf, 5) < 0) {
            panic("Error when read from pipe\n");
        }
        printf("%d: received ping\n", getpid());
        close(pfd1[0]);
        if (write(pfd2[1], "pong\n", 5) < 0){
            panic("Error when write to pipe\n");
        }
        close(pfd2[1]);
        exit(0);
    }
    else {
        // parent
        close(pfd1[0]);
        close(pfd2[1]);
        if (write(pfd1[1], "ping\n", 5) < 0){
            panic("Error when write to pipe\n");
        }
        close(pfd1[1]);
        if (read(pfd2[0], (char *)buf, 5) < 0) {
            panic("Error when read from pipe\n");
        }
        printf("%d: received pong\n", getpid());
        close(pfd2[0]); //close read end in parent
        exit(0);
    }
}