#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char * argv[])
{
    int p1[2], p2[2];

    pipe(p1);
    pipe(p2);

    if (fork() == 0)
    {   // in child node
        int cpid = getpid();
        char rec_byte;
        close(p1[1]);
        close(p2[0]);
        read(p1[0], &rec_byte, 1);
        close(p1[0]);
        printf("%d: received ping\n", cpid);
        write(p2[1], &rec_byte, 1);
        close(p2[1]);
        
    }
    else
    {   // in parent node
        int ppid = getpid();
        char byte, rec_byte;
        close(p1[0]);
        close(p2[1]);
        write(p1[1], &byte, 1);
        close(p1[1]);
        read(p2[0], &rec_byte, 1);
        close(p2[0]);
        printf("%d: received pong\n", ppid);

    }
    exit(0);
}