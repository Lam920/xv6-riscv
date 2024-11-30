
#include "kernel/types.h"
#include "user/user.h"

#define MAX_NUM 280

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

void child_exec(int pfd[2]) {
    //close write end
    close(pfd[1]);

    int cpid;
    int cpipe[2];
    int prime = 0;
    int num_from_pipe = 0;

    if (read(pfd[0], &num_from_pipe, sizeof(int)) == 0) {
        close(pfd[0]);
        wait(0);
        exit(0);
    }
    if (prime == 0) {
        printf("prime %d\n", num_from_pipe);
        prime = num_from_pipe;
    }
    // }

    if (num_from_pipe) {
        if (pipe(cpipe) < 0) {
            panic("Cannot create pipe");
            exit(1);
        }
        // printf("pfd0: %d and pfd1: %d\n", cpipe[0], cpipe[1]);
        cpid = fork();
        if (cpid == 0) {
            close(pfd[0]);
            child_exec(cpipe);
        }
        else {
            close(cpipe[0]);
            while(1) {
                // if (read(pfd[0], &num_from_pipe, sizeof(int)) < 0) {
                //     close(pfd[0]);
                //     close(cpipe[1]);
                //     panic("Read from pipe failed");
                //     exit(1);
                // }
                if (read(pfd[0], &num_from_pipe, sizeof(int)) == 0) {
                    break;
                }
                if (num_from_pipe % prime != 0) {
                    //printf("Write to pipe after: %d\n", num_from_pipe);
                    if (write(cpipe[1], &num_from_pipe, sizeof(int)) < 0) {
                        close(cpipe[1]);
                        panic("Write to pipe failed");
                        exit(1);
                    }
                }
            }
            close(cpipe[1]);
        }
    }

    close(pfd[0]);
    wait(0);
}

int main() {
    int pfd[2];
    int i;
    int pid;
    if (pipe(pfd) < 0) {
        panic("Cannot create pipe");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        panic("fork() failed");
        exit(1);
    }

    if (pid == 0) { //child
        child_exec(pfd);
    }
    else { //parent: First process fed into pipeline
        // close read end
        close(pfd[0]);
        for (i = 2; i < MAX_NUM; i++) {
            write(pfd[1], &i, sizeof(int));
        }
        close(pfd[1]);
    }

    wait(0);
    exit(0);
}