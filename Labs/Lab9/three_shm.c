#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1023

extern int etext, edata, end;

int main(void)
{
	int shmid[3];
	char* mem[3];

	printf("Addresses in parent\n");
	printf("program text(etext): \t\t%10p\ninitialized data (edata):\t%10p\nuninitialized data (end): \t%10p\n", &etext, &edata, &end);
	for (int i = 0; i < 3; i++)
	{
		if ((shmid[i] = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT|0666)) < 0)
		{
			perror("shmget fail");
			exit(1);
		}
		mem[i] = (char*) shmat(shmid[i], 0, 0);
		printf("shared mem %d: \t\t\t%10p\n", i, mem[i]);
	}
	for (int i = 0; i < SHM_SIZE; i++)
	{
		mem[0][i] = '0';
		mem[1][i] = '1';
		mem[2][i] = '2';
	}
	mem[0][1023] = 'a';
	mem[1][1023] = 'b';
	mem[2][1023] = 'c';
	
	for(int i = 0; i < 3; i++)
	{
		printf("%d: %s\n",i, mem[i]);
		shmdt(mem[i]);
		shmctl(shmid[i], IPC_RMID, (struct shmid_ds *) 0 );
	}
	
}

