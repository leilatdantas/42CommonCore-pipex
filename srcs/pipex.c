/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:46:09 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/03 19:25:39 by lebarbos         ###   ########.fr       */
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

void	ft_cleanup(t_pipex *pipex)
{
	if (pipex->fd_infile >= 0)
		close(pipex->fd_infile);
	if (pipex->fd_outfile >= 0)
		close(pipex->fd_outfile);
	if (pipex->path_cmd1)
		free(pipex->path_cmd1);
	if (pipex->path_cmd2)
		free(pipex->path_cmd2);
	ft_free_array(pipex->args_cmd1);
	ft_free_array(pipex->args_cmd2);
	unlink(URANDOM_PATH);
}

// char	*get_path(char *command, char **envp)
// {
// 	int		i;
// 	char	**path;
// 	char	*path_aux;
// 	char	*path_command;

// 	i = 0;
// 	if (!envp[0])
// 		path_aux = ft_strdup("/usr/bin:/bin:/usr/sbin:/sbin");
// 	else
// 	{
// 		while (ft_strnstr(envp[i], "PATH=", 5) == 0)
// 			i++;
// 		path_aux = ft_strdup(envp[i] + 5);
// 	}
// 	path = ft_split(path_aux, ':');
// 	free(path_aux);
// 	i = -1;
// 	while (path[++i])
// 	{
// 		path_aux = ft_strjoin(path[i], "/");
// 		path_command = ft_strjoin(path_aux, command);
// 		if (!path_command)
// 			return (NULL);
// 		free(path_aux);
// 		if (command[0] == '/' || command[0] == '.')
// 		{
// 			if (ft_strnstr(command, ".sh", ft_strlen(command)))
// 				path_command = command;
// 			path_command = ft_strdup(command);
// 		}
// 		if (access(path_command, F_OK | X_OK) == 0)
// 		{
// 			i = 0;
// 			ft_free_array(path);
// 			return (path_command);
// 		}
// 		free(path_command);
// 	}
// 	ft_free_array(path);
// 	return (NULL);
// }

// char	*ft_get_path_aux(char **envp);
// char	**ft_split_path(char *path_aux);
// char	*ft_check_command_location(char *command, char *path_i);
// char	*ft_find_command_in_paths(char *command, char **path);
// void	ft_free_memory(char **path, char *path_command);

// char	*get_path(char *command, char **envp)
// {
// 	int		i;
// 	char	*path_aux;
// 	char	**path;
// 	char	*path_command;

// 	i = 0;
// 	path_aux = ft_get_path_aux(envp);
// 	path = ft_split_path(path_aux);
// 	free(path_aux);
// 	while (path[i])
// 	{
// 		path_command = ft_check_command_location(command, path[i]);
// 		if (path_command != NULL)
// 			return (path_command);
// 		i++;
// 	}
// 	ft_free_memory(path, path_command);
// 	return (NULL);
// }

// char	*ft_get_path_aux(char **envp)
// {
// 	int		i;
// 	char	*path_aux;

// 	i = 0;
// 	if (!envp[0])
// 		path_aux = strdup("/usr/bin:/bin:/usr/sbin:/sbin");
// 	else
// 	{
// 		while (strncmp(envp[i], "PATH=", 5) != 0)
// 			i++;
// 		path_aux = strdup(envp[i] + 5);
// 	}
// 	return (path_aux);
// }

// char	**ft_split_path(char *path_aux)
// {
// 	return (ft_split(path_aux, ':'));
// }

// char	*ft_check_command_location(char *command, char *path_i)
// {
// 	char	*path_aux;
// 	char	*path_command;

// 	path_aux = ft_strjoin(path_i, "/");
// 	path_command = ft_strjoin(path_aux, command);
// 	free(path_aux);
// 	if (command[0] == '/' || command[0] == '.')
// 	{
// 		if (ft_strnstr(command, ".sh", strlen(command)))
// 			path_command = command;
// 		path_command = strdup(command);
// 	}
// 	if (access(path_command, F_OK | X_OK) == 0)
// 		return (path_command);
// 	free(path_command);
// 	return (NULL);
// }

// void	ft_free_memory(char **path, char *path_command)
// {
// 	ft_free_array(path);
// 	free(path_command);
// }

void    custom_error(char *file, char *message, t_pipex *pipex, int error)
{
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	ft_cleanup(pipex);
	exit(error);
}

void    custom_error2(char *file, char *message)
{
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

void remove_spaces(char **array) {
	if (array == NULL) {
		return;
	}

	size_t i = 0;
	size_t j = 0;

	while (array[i] != NULL) {
		// Verifica se a string contém apenas espaços
		int is_only_spaces = 1;
		for (size_t k = 0; k < strlen(array[i]); ++k) {
			if (array[i][k] != ' ' && array[i][k] != '\t' && array[i][k] != '\n') {
				is_only_spaces = 0;
				break;
			}
		}

		if (!is_only_spaces) {
			// Se a string não for apenas espaços, mantenha-a no array
			array[j++] = array[i];
		}
		else
			free(array[i]);
		++i;
	}
	// Adiciona NULL no final do array
	array[j] = NULL;
}

void    check_args(t_pipex *pipex, char **argv, char **envp)
{
	pipex->args_cmd1 = ft_split_mod(argv[CMD1]);
	remove_spaces(pipex->args_cmd1);
	pipex->args_cmd2 = ft_split_mod(argv[CMD2]);
	remove_spaces(pipex->args_cmd2);
	pipex->path_cmd1 = get_path(pipex->args_cmd1[0], envp);
	pipex->path_cmd2 = get_path(pipex->args_cmd2[0], envp);
}

void    init_pipex(t_pipex *pipex)
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

void    handle_urandom(void)
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

void     print_args_cmds(t_pipex pipex)
{
	int i = 0;
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

void ft_exec(t_pipex *pipex, char **envp, char **argv)
{
	int fd[2];
	int process;

	if((pipe(fd)) == -1)
		ft_error("pipe error\n");
	process = fork();
	if(process == -1)
		ft_error("fork error\n");
	if(process == 0)
	{
		if (ft_strncmp(argv[INFILE], "/dev/urandom", 12) == 0)
		{
			handle_urandom();
			pipex->fd_infile = open(URANDOM_PATH, O_RDONLY);
		}
		else
		{
			if((pipex->fd_infile = access(argv[INFILE], F_OK) == -1))
				perror(argv[INFILE]);
			else if ((pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444))
					== -1)
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
							custom_error2(pipex->args_cmd1[0], "no such file or directory");
						else
							custom_error2(pipex->args_cmd1[0], "command not found");   
					}
					else
						perror(pipex->args_cmd1[0]);
				}
			}
		}
	}
	else
	{
	//parent process
	// waitpid(process, NULL, WNOHANG);
	wait(NULL);
	if ((pipex->fd_outfile = open(argv[OUTFILE],
			O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
	perror(argv[OUTFILE]);
	dup2(fd[0], STDIN_FILENO);
	dup2(pipex->fd_outfile, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	if (execve(pipex->path_cmd2, pipex->args_cmd2, envp) == -1)
	{
		if (pipex->args_cmd2[0][0] == '/')
			 custom_error(pipex->args_cmd2[0], "no such file or directory", pipex, 127);
		custom_error(pipex->args_cmd2[0], "command not found", pipex, 127);
	}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex pipex;
	// int     process;

	if (argc < 5)
		ft_error("Usage: ./pipex file1 cmd1 cmd2 file2\n");
	init_pipex(&pipex);
	check_args(&pipex, argv, envp);
	// print_args_cmds(pipex);
	// process = fork();
	// if (process == -1)
	//     ft_error("fork error\n");
	// if (process == 0)
	ft_exec(&pipex, envp, argv);
	// wait(NULL);
	ft_cleanup(&pipex);
	return (0);
}
