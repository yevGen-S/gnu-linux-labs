#include "p.h"

void sigintHandler(int sig){
	char buf[20];
	sprintf(buf, "/tmp/fifo %d", getpid());
    
    const int result = remove(buf);
	write(0, "Ahhh! SIGINT!\n", 15);
	fprintf(stderr, "Removal status %d %s\n", result, buf);
	exit(0);
}

int main(void)
{
    system("./run_server");

    struct sigaction sa;
    sa.sa_handler = sigintHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    int	n, privateFifo, publicFifo;
    static char buffer[PIPE_BUF];
    struct message msg;
    
    /* Make the name for the private FIFO */
    sprintf(msg.fifo_name, "/tmp/fifo %d", getpid());
    
    /* Generate the private FIFO */
    if (mknod(msg.fifo_name, S_IFIFO | 0666, 0)<0)
    {
            perror(msg.fifo_name);
            exit(1);
    }

    /* OPEN the public FIFO for writing */
    if ((publicFifo=open(PUBLIC, O_WRONLY))==-1)\
    {
        perror(PUBLIC);
        exit(2);
    }

    while(1)
    {
        /* FOREVER */
        write(fileno(stdout), "\ncmd>", 6);

        /* Clear first */
        memset(msg.cmd_line, 0x0, B_SIZ);

        /* Get cmd */
        n = read(fileno(stdin), msg.cmd_line, B_SIZ);

        if(!strncmp("quit", msg.cmd_line, n-1))
        {
            break;
        }

        /* to PUBLIC */
        write(publicFifo, (char *) &msg, sizeof(msg));

        /* OPEN private FIFO to read returned command oputput */
        if((privateFifo = open(msg.fifo_name, O_RDONLY))==-1)
        {
            perror(msg.fifo_name);
            exit(3);
        }

        /* READ private FIFO and display on standard error */
        while((n=read(privateFifo, buffer, PIPE_BUF))>0)
        {
            write(fileno(stderr), buffer, n);
        }

        close(privateFifo);
    }

    close(publicFifo);
    unlink(msg.fifo_name);
}
