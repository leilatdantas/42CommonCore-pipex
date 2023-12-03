/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:46:09 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/03 20:41:19 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_error(char *error_message)
{
	ft_putstr_fd(error_message, 2);
	exit(EXIT_FAILURE);
}

void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	check_args(t_pipex *pipex, char **argv, char **envp)
{
	pipex->args_cmd1 = ft_split_mod(argv[CMD1]);
	remove_spaces(pipex->args_cmd1);
	pipex->args_cmd2 = ft_split_mod(argv[CMD2]);
	remove_spaces(pipex->args_cmd2);
	pipex->path_cmd1 = get_path(pipex->args_cmd1[0], envp);
	pipex->path_cmd2 = get_path(pipex->args_cmd2[0], envp);
}

void	init_pipex(t_pipex *pipex)
{
	pipex->args_cmd1 = NULL;
	pipex->args_cmd2 = NULL;
	pipex->path_cmd1 = NULL;
	pipex->path_cmd2 = NULL;
	pipex->fd_infile = -1;
	pipex->fd_outfile = -1;
}

int	ft_read(char **line, int fd, char limiter)
{
	char	*buffer;
	char	c;
	int		i;

	i = 0;
	if (fd < 0)
		return (-1);
	buffer = ft_calloc(sizeof(char), 10000);
	if (!buffer)
		return (-1);
	while (read(fd, &c, 1) > 0 && c && c != limiter && i < 10000 - 2)
		buffer[i++] = c;
	buffer[i] = '\n';
	buffer[i + 1] = '\0';
	*line = buffer;
	return (i + 1);
}

void	handle_urandom(void)
{
	int		tmp_fd;
	int		urandom_fd;
	char	*buffer;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd < 0)
		return ;
	tmp_fd = open(URANDOM_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		return ;
	if (ft_read(&buffer, urandom_fd, '\0') == -1)
		return ;
	write(tmp_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(tmp_fd);
}

void	print_args_cmds(t_pipex pipex)
{
	int	i;

	i = 0;
	while (pipex.args_cmd1[i])
	{
		printf("%s\n", pipex.args_cmd1[i]);
		i++;
	}
	i = 0;
	while (pipex.args_cmd2[i])
	{
		printf("%s\n", pipex.args_cmd2[i]);
		i++;
	}
	printf("%s\n", pipex.path_cmd1);
	printf("%s\n", pipex.path_cmd2);
}

// void	ft_exec(t_pipex *pipex, char **envp, char **argv)
// {
// 	int	fd[2];
// 	int	process;

// 	if ((pipe(fd)) == -1)
// 		ft_error("pipe error\n");
// 	process = fork();
// 	if (process == -1)
// 		ft_error("fork error\n");
// 	if (process == 0)
// 	{
// 		if (ft_strncmp(argv[INFILE], "/dev/urandom", 12) == 0)
// 		{
// 			handle_urandom();
// 			pipex->fd_infile = open(URANDOM_PATH, O_RDONLY);
// 		}
// 		else
// 		{
// 			pipex->fd_infile = access(argv[INFILE], F_OK);
// 			if (pipex->fd_infile == -1)
// 				perror(argv[INFILE]);
// 			else
// 				pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444);
// 			if (pipex->fd_infile == -1)
// 				perror(argv[INFILE]);
// 			else
// 			{
// 				dup2(pipex->fd_infile, STDIN_FILENO);
// 				dup2(fd[1], STDOUT_FILENO);
// 				close(fd[0]);
// 				close(fd[1]);
// 				unlink(URANDOM_PATH);
// 				if (execve(pipex->path_cmd1, pipex->args_cmd1, envp) == -1)
// 				{
// 					if (pipex->path_cmd1 == NULL)
// 					{
// 						if (pipex->args_cmd2[0][0] == '/')
// 							custom_error2(pipex->args_cmd1[0],
// 								"no such file or directory");
// 						else
// 							custom_error2(pipex->args_cmd1[0],
// 								"command not found");
// 					}
// 					else
// 						perror(pipex->args_cmd1[0]);
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		wait(NULL);
// 		pipex->fd_outfile = open(argv[OUTFILE],
// 				O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 		if (pipex->fd_outfile == -1)
// 			perror(argv[OUTFILE]);
// 		dup2(fd[0], STDIN_FILENO);
// 		dup2(pipex->fd_outfile, STDOUT_FILENO);
// 		close(fd[1]);
// 		close(fd[0]);
// 		if (execve(pipex->path_cmd2, pipex->args_cmd2, envp) == -1)
// 		{
// 			if (pipex->args_cmd2[0][0] == '/')
// 				custom_error(pipex->args_cmd2[0],
// 					"no such file or directory", pipex, 127);
// 			custom_error(pipex->args_cmd2[0],
// 				"command not found", pipex, 127);
// 		}
// 	}
// }

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc < 5)
		ft_error("Usage: ./pipex file1 cmd1 cmd2 file2\n");
	init_pipex(&pipex);
	check_args(&pipex, argv, envp);
	// print_args_cmds(pipex);
	ft_exec(&pipex, envp, argv);
	ft_cleanup(&pipex);
	return (0);
}
