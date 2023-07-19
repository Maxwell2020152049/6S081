#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define WRITE 1
#define READ 0
#define INT_SIZE 4

void sieve(int pl[2])
{
    int tmp;
    int prime;
    int pid;
    int pr[2];

    read(pl[READ], (void*)&tmp, INT_SIZE);

    if (tmp == -1)
        exit(0);

    printf("prime %d\n", tmp);
    prime = tmp;

    if (pipe(pr) == -1)
    {
        fprintf(2, "pipe() error.\n");
    }

    pid = fork();

    if (pid > 0)
    {
        close(pr[READ]);

        while (read(pl[READ], (void*)&tmp, INT_SIZE) != 0)
        {
            if (tmp % prime != 0)
                write(pr[WRITE], (void*)&tmp, INT_SIZE);
        }

        close(pl[READ]);
        close(pr[WRITE]);

        wait(0);

        exit(0);
    }
    else if (pid == 0)
    {
        close(pr[WRITE]);
        close(pl[READ]);

        sieve(pr);

        close(pr[READ]);

        exit(0);
    }
    else if (pid == -1)
    {
        fprintf(2, "fork() error.\n");
    }
}

int main()
{
    int p[2];
    int i;
    int pid;

    if (pipe(p) == -1)
    {
        fprintf(2, "pipe() error.\n");
    }

    pid = fork();

    if (pid == 0)
    {
        close(p[WRITE]);

        sieve(p);
    }
    else if (pid > 0)
    {
        close(p[READ]);

        int num_wr;
        for (i = 2; i <= 35; i++)
        {
            num_wr = write(p[WRITE], (void*)&i, INT_SIZE);
            if (num_wr == -1)
                fprintf(2, "write() error.\n");
        }

        i = -1;
        num_wr = write(p[WRITE], (void*)&i, INT_SIZE);
        if (num_wr == -1)
            fprintf(2, "write() error.\n");

        close(p[WRITE]);

        wait(0);
    }
    else if (pid == -1)
    {
        fprintf(2, "fork() error.\n");
    }

    exit(0);
}