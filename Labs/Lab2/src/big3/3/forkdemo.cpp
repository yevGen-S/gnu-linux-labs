#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
main()
{
 int i;
  if (fork()) {    /* I must be the parent  */
      for(i=0; i<1000 ; i++)
         sleep(7); 
      }
       else {      /* I must be the child   */
        sleep(7);
	}
}      
