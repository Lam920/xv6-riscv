#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int time = 0;

    if(argc != 2){
        fprintf(2, "Usage: sleep $time\n");
        exit(1);
    }

    time = atoi(argv[1]);
    printf("time to sleep: %d\n", time);
    if (sleep(time) < 0) {
        fprintf(2, "sleep %d failed\n", time);
    }


     exit(0);
}