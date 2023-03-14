#include "p.h"
#include <sys/wait.h>

void sigintHandler(int sig){
	char buf[20];
    sprintf(buf, "/tmp/PUBLIC");
    
    const int result = remove(buf);
	write(0, "Ahhh! SIGINT!\n", 15);
	fprintf(stderr, "Removal status %d   %s\n", result, buf);
	exit(0);
}

int main(void)
{
    struct sigaction sa;
    sa.sa_handler = sigintHandler;
    sa.sa_flags = 0; 
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    int n, done, dummyfifo, privatefifo, publicfifo;
    static char buffer[PIPE_BUF];
    FILE *fin;
    struct message msg;
    
    /* Создание очереди типа public FIFO */
    mknod(PUBLIC, S_IFIFO | 0666, 0);
    
    /* Открыть public FIFO на чтение и запись */
    if (((publicfifo=open(PUBLIC, O_RDONLY))==-1) ||(dummyfifo=open(PUBLIC, O_WRONLY | O_NDELAY))==-1)
    {
        perror(PUBLIC);
        exit(1);
    }
    
    /* Сообщение можно прочитатьиз public конвейера */
    while(read(publicfifo, (char *) &msg, sizeof(msg))>0){
        /* Очистка счетчиков | флагов */
        n = done = 0;

        /* Попытка открытия private FIFO */
        do {

            if ((privatefifo=open(msg.fifo_name, O_WRONLY | O_NDELAY)) == -1)
            {
                /* Задержка по времени  */
                sleep(3);
            }
            else /* Открытие успешно */
            {
                if (fork() == 0)
                {
                    dup2(fileno(fin), 1);
                    system(msg.cmd_line);
                    exit(0);
                }

                wait(0);

                /* Исполнение shell cmd, полученной от клиента */
                fin = popen(msg.cmd_line, "r");

                /* Подготовка очередного вывода */
                write(privatefifo, "\n", 1);

                while ((n = read(fileno(fin), buffer, PIPE_BUF)) > 0)
                {
                    /* Выводв private FIFO кклиенту */
                    write(privatefifo, buffer, n);
                    /* Очисткабуфера */
                    memset(buffer, 0x0, PIPE_BUF);
                }

                pclose(fin);
                close(privatefifo);
                
                /* Записьпроизведенауспешно */
                done = 1;
            }
        } while (++n < 5 && !done);

        /* Указаниенане удачный исход */
        if(!done)
        {
            write(fileno(stderr), "\nNOTE: SERVER ** NEVER ** accessed private FIFO\n", 48);
        }
    }

    return 0;
}
