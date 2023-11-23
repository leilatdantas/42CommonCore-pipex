/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:06:10 by lebarbos          #+#    #+#             */
/*   Updated: 2023/11/22 18:45:01 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main()
{
	int fd_child[2];
	int fd_parent[2];
	int pid;
	if (pipe(fd_child) == -1)
		return (1);
	if (pipe(fd_parent) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return 6;
	if (pid == 0)
	{
		close(fd_parent[1]);
		close(fd_child[0]);
		int x;
		if (read(fd_parent[0], &x, sizeof(x)) == -1)
			return 2;
		printf("Child process read %d from parent process\n", x);
		x = x * 4;
		if (write(fd_child[1], &x, sizeof(x)) == -1)
			return 3;
		printf("Child process wrote %d in the read file of the pipe\n", x);
		close(fd_parent[0]);
		close(fd_child[1]);
	}
	else
	{
		close(fd_parent[0]);
		close(fd_child[1]);
		srand(time(NULL));
		int y = rand() % 10;
		if (write(fd_parent[1], &y, sizeof(y)) == -1)
			return 4; 
		printf("Number generated and written by the parent process: %d\n", y);
		if (read(fd_child[0], &y, sizeof(y)) == -1)
			return 5;
		printf("Number after multiplication on child process: %d\n", y);
		close(fd_child[0]);
		close(fd_parent[1]);
	}
	return (0);
}