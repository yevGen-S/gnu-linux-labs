#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <locale.h>
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int main()
{
    setlocale(LC_ALL, "rus");
    char u_char = 'J';
    key_t key;
    int smid;

    if ((key = ftok(".", u_char)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    smid = semget(key, 1, IPC_EXCL | 0666);
    fprintf(stderr, "id семафора: %d\n", smid);
    int prcCnt = semctl(smid, 0, GETNCNT);
    fprintf(stderr, "Ожидающих процессов: %d\n", prcCnt);
}
