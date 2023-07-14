#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define WRITE 1
#define READ 0

int main()
{
    // p2c: parent to child
    // c2p: child to parent
    int p2c[2], c2p[2];

    pipe(p2c);
    pipe(c2p);

    int pid = fork();

    if (pid > 0)
    {   // parent process
        close(p2c[READ]);
        close(c2p[WRITE]);

        char byte = 'R';
        write(p2c[WRITE], &byte, 1);
        read(c2p[READ], &byte, 1);
        printf("%d: received pong\n", getpid());

        close(p2c[WRITE]);
        close(c2p[READ]);
    }
    else if (pid == 0)
    {   // child process
        close(p2c[WRITE]);
        close(c2p[READ]);

        char byte;
        read(p2c[READ], &byte, 1);
        printf("%d: received ping\n", getpid());
        write(c2p[WRITE], &byte, 1);
        
        close(p2c[READ]);
        close(c2p[WRITE]);
    }
    else 
    {
        printf("create child process error.\n");
        exit(1);
    }

    exit(0);
}