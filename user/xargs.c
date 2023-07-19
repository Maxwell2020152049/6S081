#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARG 32
#define ARGLEN 32

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(2, "Usage: xargs {cmd} {args}\n");
        exit(1);
    }

    char args[MAXARG][ARGLEN];
    int i, j;
    int cnt = argc - 1;
    int num_rd;
    int idx = 0;
    int pid;
    char* arg[MAXARG];

    for (j = 1; j < argc; j++)
    {
        for (i = 0; argv[j][i] != '\0'; i++)
            args[j - 1][i] = argv[j][i];    
        args[j - 1][i] = '\0';
    }
    
    // for (int i = 0; i < cnt; i++)
    //     printf("%s\n", args[i]);

    while (1)
    {
        num_rd = read(0, &args[cnt][idx++], 1);
        if (num_rd == -1)
        {
            fprintf(2, "read() error\n");
            exit(1);
        }
        else if (num_rd == 0)
        {
            break;
        }
        
        if (args[cnt][idx - 1] == '\n')
        {
            // printf("%s\n", args[cnt]);
            args[cnt][idx - 1] = '\0';
            cnt++;
            idx = 0;
        }
    }

    // for (int i = 0; i < cnt; i++)
    //     printf("%d: %s", i, args[i]);

    for (int i = 0; i < cnt; i++)
        arg[i] = args[i];

    arg[cnt] = 0;

    pid = fork();

    if (pid > 0)
    {
        wait(0);
    }
    else if (pid == 0)
    {
        exec(arg[0], arg);
    }
    else if (pid == -1)
    {
        fprintf(2, "fork() error.\n");
    }

    exit(0);
}