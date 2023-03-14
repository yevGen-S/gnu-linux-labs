/* The program gener_mq.cpp */
/* Message queue generation */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX 5

int main(void)
{
    FILE* fin;

    char	buffer[PIPE_BUF];
    char	u_char = 'A';
    
    int	i;
    int n;
    int mid[MAX];
    
    key_t	key;
    
    for (i = 0; i < MAX; ++i, ++u_char)
    {
        key = ftok(".", u_char);
        
        if ((mid[i] = msgget(key, IPC_CREAT | 0660)) == -1)
        {
            perror("Queue create");
            exit(1);
        }
    }

    /* Run the ipcs command */
    fin = popen("ipcs", "r");
    
    while ((n = read(fileno(fin), buffer, PIPE_BUF)) > 0)
    {
        /*Display the ipcs output */
        write(fileno(stdout), buffer, n);	
    }
        
    pclose(fin);
    
    for (i = 0; i < MAX; ++i)
    {
        /* remove */
        msgctl(mid[i], IPC_RMID, NULL);
    }                

    exit(0);
}








