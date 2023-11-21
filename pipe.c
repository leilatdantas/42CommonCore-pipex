#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main()
{
	int fd[2];
	int	pid;
	char	buffer[14];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) //child process
	{
		close(fd[0]);
		write(fd[1], "Hello parent!", 14);
		close(fd[1]);
	}
	else //parent process
	{
		close(fd[1]);
		read(fd[0], buffer, 14);
		close(fd[0]);
		printf("Message from child: '%s'\n", buffer);
		exit(EXIT_SUCCESS);
	}
}