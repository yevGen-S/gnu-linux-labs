#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void signalHandler(int sig)
{
    signal(sig, signalHandler);
    printf("Signal %d recieved by %d\n", sig, getpid());
}

int main()
{
    void signalHandler(int);

    if (signal(SIGUSR1, signalHandler) == SIG_ERR)
    {
        exit(1);
    }

    if (signal(SIGUSR2, signalHandler) == SIG_ERR)
    {
        exit(1);
    }

    pid_t pid = 0;
    if ((pid = fork()) == 0)
    {
        printf("Hello, clild procces with pid: %d\n", getpid());
        
        for (int i = 0; ; ++i)
        {
            sleep(2);

            switch (i % 2)
            {
            case 0:
                printf("CHILD: Send to parent SIGUSR1 signal\n");
                kill(getppid(), SIGUSR1);
                break;
            
            case 1:
                printf("CHILD: Send to parent SIGUSR2 signal\n");
                kill(getppid(), SIGUSR2);
                i = 0;
                break;

            default:
                printf("CHILD: loop var error\n");
                exit(1);
                break;
            }
        }
    }
    else
    {
        printf("Hello, parent procces with pid: %d\n", getpid());
        
        for (int i = 0; ; ++i)
        {
            sleep(2);

            switch (i % 2)
            {
            case 0:
                printf("PARENT: Send to child SIGUSR2 signal\n");
                kill(pid, SIGUSR2);
                break;
            
            case 1:
                printf("PARENT: Send to child SIGUSR1 signal\n");
                kill(pid, SIGUSR1);
                i = 0;
                break;

            default:
                printf("PARENT: loop var error\n");
                exit(1);
                break;
            }
        }
    }

    return 0;
}
