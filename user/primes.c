#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int child(int prev_pipe[2])
{
    int next_pipe[2];

    int pri, num;
    pipe(next_pipe);

    if (read(prev_pipe[0], &pri, sizeof(int)) != sizeof(int))
        exit(0);
    
    if (fork() == 0){
        // call next child process
        close(next_pipe[1]);
        child(next_pipe);
        close(next_pipe[0]);
        exit(0);
    }
    else{
        close(next_pipe[0]);
        printf("prime %d\n", pri);
        while (read(prev_pipe[0], &num, sizeof(int)) == sizeof(int)){
            if ((num %  pri) > 0){
                write(next_pipe[1], &num, sizeof(int));
            }
        }
        close(prev_pipe[0]);
        close(next_pipe[1]);
    }
    wait(0);
    exit(0);
}

int main(int argc, char *argv[]){
    
    int prev_pipe[2];


    pipe(prev_pipe);


    if (fork() == 0){   
        // in child process, no need to write
        close(prev_pipe[1]);
        child(prev_pipe);
        close(prev_pipe[0]);
    }
    else{
        // in parent process, no need to read
        close(prev_pipe[0]);
        for (int i =2; i<= 35; i++)
        {
            write(prev_pipe[1], &i, sizeof(int));
        }
        close(prev_pipe[1]);
    }
    wait(0);
    exit(0);
    
}