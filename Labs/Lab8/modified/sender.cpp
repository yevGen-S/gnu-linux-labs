/*
** sender.cpp -- writes to a message queue
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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

	int c;

	if ((key = ftok(".", 'B')) == -1)
	{
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
	{
		perror("msgget");
		exit(1);
	}

	printf("What kind of type of messages you want to send now? (1 or 2)\n");
	scanf("%ld", &(buf.mtype));
	
	printf("Enter lines of text, ^D to quit:\n");

	// /* we don't really care in this case */
	// buf.mtype = 1;

	while ((c = getchar()) != '\n' && c != EOF);

	while(fgets(buf.mtext, sizeof(buf.mtext), stdin) != NULL)
	{
		int len = strlen(buf.mtext);

		/* ditch newline at end, if it exists */
		if (buf.mtext[len-1] == '\n')
		{
			buf.mtext[len-1] = '\0';
		}

		/* +1 for '\0' */
		if (msgsnd(msqid, &buf, len+1, 0) == -1)
		{
			perror("msgsnd");
		}

		printf("What kind of type of messages you want to send now? (1 or 2)\n");
		scanf("%ld", &(buf.mtype));
		
		while ((c = getchar()) != '\n' && c != EOF);
	}

	if (msgctl(msqid, IPC_RMID, NULL) == -1)
	{
		perror("msgctl");
		exit(1);
	}

	return 0;
}
