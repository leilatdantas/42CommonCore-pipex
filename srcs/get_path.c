/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 19:25:10 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/03 19:56:30 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_get_path_aux(char **envp);
char	**ft_split_path(char *path_aux);
char	*ft_check_command_location(char *command, char *path_i);
char	*ft_find_command_in_paths(char *command, char **path);
void	ft_free_memory(char **path, char *path_command);

void	ft_free_memory(char **path, char *path_command)
{
	ft_free_array(path);
	free(path_command);
}

char	*ft_get_path_aux(char **envp)
{
	int		i;
	char	*path_aux;

	i = 0;
	if (!envp[0])
		path_aux = strdup("/usr/bin:/bin:/usr/sbin:/sbin");
	else
	{
		while (strncmp(envp[i], "PATH=", 5) != 0)
			i++;
		path_aux = strdup(envp[i] + 5);
	}
	return (path_aux);
}

char	**ft_split_path(char *path_aux)
{
	return (ft_split(path_aux, ':'));
}

char	*ft_check_command_location(char *command, char *path_i)
{
	char	*path_aux;
	char	*path_command;

	path_aux = ft_strjoin(path_i, "/");
	path_command = ft_strjoin(path_aux, command);
	free(path_aux);
	if (command[0] == '/' || command[0] == '.')
	{
		if (ft_strnstr(command, ".sh", strlen(command)))
			path_command = command;
		path_command = strdup(command);
	}
	if (access(path_command, F_OK | X_OK) == 0)
		return (path_command);
	free(path_command);
	return (NULL);
}

char	*get_path(char *command, char **envp)
{
	int		i;
	char	*path_aux;
	char	**path;
	char	*path_command;

	i = 0;
	path_aux = ft_get_path_aux(envp);
	path = ft_split_path(path_aux);
	free(path_aux);
	while (path[i])
	{
		path_command = ft_check_command_location(command, path[i]);
		if (path_command != NULL)
			return (path_command);
		i++;
	}
	ft_free_memory(path, path_command);
	return (NULL);
}
