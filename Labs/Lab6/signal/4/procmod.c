#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void pHandler(int sig)
{
    printf("PARENT: recieved %d\n", sig) ;
    
    if (sig == SIGQUIT)
    {
        exit(1);
    }
}

void cHandler(int sig)
{
    printf("CHILD: recieved %d\n", sig) ;
    
    if (sig == SIGQUIT)
    {
        exit(1);
    }
}

int main()
{
    void pHandler(int);
    void cHandler(int);

    struct sigaction sa;

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    pid_t cPid = 0;
    pid_t pPid = getpid();

    if ((cPid = fork()) == 0)
    {
        printf("Hello, clild procces with pid: %d\n", getpid());
        
        sa.sa_handler = cHandler;
        if (
            sigaction(SIGUSR1, &sa, NULL) == -1 ||
            sigaction(SIGUSR2, &sa, NULL) == -1
            )
        {
            printf("CHILD: signal error");
            perror(SIGQUIT);
            exit(1);
        }

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
        
        sa.sa_handler = pHandler;
        if (
            sigaction(SIGUSR1, &sa, NULL) == -1 ||
            sigaction(SIGUSR2, &sa, NULL) == -1
            )   
        {
            printf("CHILD: signal error");
            perror(SIGQUIT);
            exit(1);
        }

        for (int i = 0; ; ++i)
        {
            sleep(2);

            switch (i % 2)
            {
            case 0:
                printf("PARENT: Send to child SIGUSR1 signal\n");
                kill(cPid, SIGUSR2);
                break;
            
            case 1:
                printf("PARENT: Send to child SIGUSR2 signal\n");
                kill(cPid, SIGUSR1);
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