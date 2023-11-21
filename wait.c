#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int	main()
{
	pid_t	pid;

	pid = fork();
	if (pid == - 1) //error
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if	(pid == 0) //child process
	{
		printf("I am the child process.\n");
		sleep(2);
		exit(EXIT_SUCCESS);
	}
	else
	{
		printf("I am the parent process.\n");
		wait(NULL);
		printf("Child process terminated after 2s delay.\n");
	}
	return(EXIT_SUCCESS);
}