/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:32:06 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/10 14:18:56 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <errno.h>

void	setup_infile(t_pipex *pipex, char **argv)
{
	if (ft_strncmp(argv[INFILE], "/dev/urandom", 12) == 0)
	{
		handle_urandom();
		pipex->fd_infile = open(URANDOM_PATH, O_RDONLY);
	}
	else
	{
		if (argv[INFILE] == NULL)
			return ;
		pipex->fd_infile = access(argv[INFILE], F_OK);
		if (pipex->fd_infile != -1)
			pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444);
	}
}

void	ft_execve(char *cmd, char **args, int p, t_pipex *pipex, char **envp)
{
	if (execve(cmd, args, envp) == -1)
	{
		if (access(cmd, X_OK) == -1)
		{
			if (access(cmd, F_OK) == -1)
			{
				if (args[0][0] == '/')
					custom_error2(args[0], "No such file or directory");
				else
					custom_error2(args[0], "command not found");
				if (p != 0)
				{
					ft_cleanup(pipex);
					exit(127);
				}
			}
			perror(args[0]);
			if (p != 0)
				exit(126);
		}
	}
}

void	child_process(int *fd, t_pipex *pipex, char **envp)
{
	dup2(pipex->fd_infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	unlink(URANDOM_PATH);
	if (!pipex->path_cmd1)
	{
		if (!pipex->args_cmd1[0])
			custom_error2("", "command not found");
		else
			custom_error2(pipex->args_cmd1[0], "command not found");
	}
	else
		ft_execve(pipex->path_cmd1, pipex->args_cmd1, 0, pipex, envp);
	// else if(execve(pipex->path_cmd1, pipex->args_cmd1, envp) == -1)
	// {
	// 		if (access(pipex->path_cmd2, X_OK) == -1)
	// 		{
	// 			if (access(pipex->path_cmd2, F_OK) == -1)
	// 			{
	// 				if (pipex->args_cmd2[0][0] == '/')
	// 				{
	// 					custom_error2(pipex->args_cmd2[0],
	// 							"No such file or directory");
	// 				}
	// 				custom_error2(pipex->args_cmd2[0],
	// 						"command not found");
	// 			}
	// 			perror(pipex->args_cmd2[0]);
	// 			exit(126);
	// 		}
	// }
}

void	parent_process(int *fd, t_pipex *pipex, char **envp, char **argv)
{
	pipex->fd_outfile = open(argv[OUTFILE],
			O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (pipex->fd_outfile == -1)
	{
		perror(argv[OUTFILE]);
		ft_cleanup(pipex);
		if (open(argv[OUTFILE], O_WRONLY) != 0)
			exit(2);
		exit(1);
	}
	else
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(pipex->fd_outfile, STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		if (!pipex->path_cmd2)
		{
			if (!pipex->args_cmd2[0])
				custom_error2("", "command not found");
			else
				custom_error2(pipex->args_cmd2[0], "command not found");
			ft_cleanup(pipex);
			exit(127);
		}
		else
			ft_execve(pipex->path_cmd2, pipex->args_cmd2, 1, pipex, envp);
		// else if (execve(pipex->path_cmd2, pipex->args_cmd2, envp) == -1)
		// {
		// 	if (access(pipex->path_cmd2, X_OK) == -1)
		// 	{
		// 		if (access(pipex->path_cmd2, F_OK) == -1)
		// 		{
		// 			if (pipex->args_cmd2[0][0] == '/')
		// 			{
		// 				custom_error(pipex->args_cmd2[0],
		// 						"No such file or directory", pipex, 127);
		// 			}
		// 			custom_error(pipex->args_cmd2[0],
		// 					"command not found", pipex, 127);
		// 		}
		// 		perror(pipex->args_cmd2[0]);
		// 		exit(126);
		// 	}
		// }
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
		setup_infile(pipex, argv);
		if (pipex->fd_infile == -1)
			perror(argv[INFILE]);
		else
			child_process(fd, pipex, envp);
	}
	else
	{
		waitpid(process, NULL, WNOHANG);
		parent_process(fd, pipex, envp, argv);
	}
}
