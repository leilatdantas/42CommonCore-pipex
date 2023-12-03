/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:32:06 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/03 20:40:45 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	setup_input(t_pipex *pipex, char **argv)
{
	if (ft_strncmp(argv[INFILE], "/dev/urandom", 12) == 0)
	{
		handle_urandom();
		pipex->fd_infile = open(URANDOM_PATH, O_RDONLY);
		return ;
	}
	else
	{
		pipex->fd_infile = access(argv[INFILE], F_OK);
		if (pipex->fd_infile == -1)
			return ;
		else
			pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444);
	}
}

void	ft_exec(t_pipex *pipex, char **envp, char **argv)
{
	int	fd[2];
	int	process;

	if ((pipe(fd)) == -1)
		perror("pipe");
	process = fork();
	if (process == -1)
		perror("fork");
	if (process == 0)
	{
		// if (ft_strncmp(argv[INFILE], "/dev/urandom", 12) == 0)
		// {
		// 	handle_urandom();
		// 	pipex->fd_infile = open(URANDOM_PATH, O_RDONLY);
		// }
		// else
		// {
		// 	pipex->fd_infile = access(argv[INFILE], F_OK);
		// 	if (pipex->fd_infile == -1)
		// 		perror(argv[INFILE]);
		// 	else
		// 		pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444);
		// 	if (pipex->fd_infile == -1)
		// 		perror(argv[INFILE]);
		if (pipex->fd_infile == -1)
			perror(argv[INFILE]);
		else
		{
			dup2(pipex->fd_infile, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			unlink(URANDOM_PATH);
			if (execve(pipex->path_cmd1, pipex->args_cmd1, envp) == -1)
			{
				if (pipex->path_cmd1 == NULL)
				{
					if (pipex->args_cmd2[0][0] == '/')
						custom_error2(pipex->args_cmd1[0],
							"no such file or directory");
					else
						custom_error2(pipex->args_cmd1[0],
							"command not found");
				}
				else
					perror(pipex->args_cmd1[0]);
			}
		}
	}
	
	else
	{
		wait(NULL);
		pipex->fd_outfile = open(argv[OUTFILE],
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (pipex->fd_outfile == -1)
			perror(argv[OUTFILE]);
		dup2(fd[0], STDIN_FILENO);
		dup2(pipex->fd_outfile, STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		if (execve(pipex->path_cmd2, pipex->args_cmd2, envp) == -1)
		{
			if (pipex->args_cmd2[0][0] == '/')
				custom_error(pipex->args_cmd2[0],
					"no such file or directory", pipex, 127);
			custom_error(pipex->args_cmd2[0],
				"command not found", pipex, 127);
		}
	}
}