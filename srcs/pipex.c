/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:32:06 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/22 10:55:50 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_execve(char *cmd, char **args, t_pipex *pipex, char **envp)
{
	if (execve(cmd, args, envp) == -1)
	{
		if (access(cmd, X_OK) == -1)
		{
			if (access(cmd, F_OK) == -1)
			{
				if (args[0][0] == '/' || is_script(args[0]))
					custom_error(args[0],
						"No such file or directory", pipex, 127);
				custom_error(args[0],
					"command not found", pipex, 127);
			}
			perror(args[0]);
			ft_cleanup(pipex);
			exit(126);
		}
	}
}

void	child_process(int *fd, t_pipex *pipex, char **envp)
{
	if (pipex->path_cmd1 == NULL)
	{
		if (pipex->args_cmd1 != NULL)
		{
			if (!is_script(pipex->args_cmd1[0]))
				pipex->path_cmd1 = ft_strdup(pipex->args_cmd1[0]);
		}
		else
		{
			ft_cleanup(pipex);
			ft_putstr_fd("command not found\n", 2);
			exit(127);
		}
	}
	dup2(pipex->fd_infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_execve(pipex->path_cmd1, pipex->args_cmd1, pipex, envp);
}

void	parent_process(int *fd, t_pipex *pipex, char **envp, int process)
{
	waitpid(process, NULL, WNOHANG);
	if (pipex->path_cmd2 == NULL)
	{
		if (pipex->args_cmd2 != NULL)
		{
			if (!is_script(pipex->args_cmd2[0]))
				pipex->path_cmd2 = ft_strdup(pipex->args_cmd2[0]);
		}
		else
		{
			ft_cleanup(pipex);
			ft_putstr_fd("command not found\n", 2);
			exit(127);
		}
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(pipex->fd_outfile, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	ft_execve(pipex->path_cmd2, pipex->args_cmd2, pipex, envp);
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
		child_process(fd, pipex, envp);
	}
	else
	{
		setup_outfile(pipex, argv);
		parent_process(fd, pipex, envp, process);
	}
}
