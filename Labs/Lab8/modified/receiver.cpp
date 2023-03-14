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
	long type = 0;
	printf("What kind of type of messages you want to receive now? (1 or 2)\n");
	scanf("%ld", &type);

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
		if (msgrcv(msqid, &buf, sizeof(buf.mtext), type, 0) == -1)
		{
			perror("msgrcv");
			exit(1);
		}

		printf("spock: \"%s\"\n", buf.mtext);

		printf("What kind of type of messages you want to receive now? (1 or 2)\n");
		scanf("%ld", &type);
	}

	return 0;
}

