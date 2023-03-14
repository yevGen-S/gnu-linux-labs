/*
** receiver.cpp -- reads from a message queue
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf
{
	long mtype;
	char mtext[200];
};

int main(void)
{
	struct my_msgbuf buf;
	int msqid;
	key_t key;

	/* same key as sender.cpp */
	if ((key = ftok(".", 'B')) == -1)
	{
		perror("ftok");
		exit(1);
	}

	/* connect to the queue */
	if ((msqid = msgget(key, 0644)) == -1)
	{
		perror("msgget");
		exit(1);
	}
	
	printf("spock: ready to receive messages, captain.\n");

	/* Spock never quits! */
	for(;;)
	{
		if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1)
		{
			perror("msgrcv");
			exit(1);
		}

		printf("spock: \"%s\"\n", buf.mtext);
	}

	return 0;
}

