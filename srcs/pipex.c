/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:46:09 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/06 10:00:48 by lebarbos         ###   ########.fr       */
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
	pipex->args_cmd2 = ft_split_mod(argv[CMD2]);
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
