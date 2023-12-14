/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 19:25:10 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/14 21:30:58 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_get_path_aux(char **envp)
{
	int		i;
	char	*path_aux;

	i = 0;
	path_aux = NULL;
	if (!envp[0])
		path_aux = ft_strdup("/usr/bin:/bin");
	else
	{
		while (envp[i] && (ft_strncmp(envp[i], "PATH=", 5) != 0))
			i++;
		if (envp[i] && envp[i][5] != '\0')
			path_aux = ft_strdup(envp[i] + 5);
	}
	return (path_aux);
}

char	*ft_check_command_location(char *command, char *path_i)
{
	char	*path_aux;
	char	*path_command;

	path_command = NULL;
	path_aux = ft_strjoin(path_i, "/");
	path_command = ft_strjoin(path_aux, command);
	free(path_aux);
	if (command[0] == '/' || (ft_strncmp(command, "./", 2) == 0))
		path_command = ft_strdup(command);
	else if (ft_strnstr(command, ".sh", ft_strlen(command))
		&& ft_strchr(command, '/'))
		path_command = ft_strdup(command);
	if (access(path_command, F_OK) == 0)
		return (path_command);
	free(path_command);
	return (NULL);
}

char	*find_path(char *path_aux, char *command)
{
	char	**path;
	char	*path_command;
	int		i;

	i = 0;
	if (!path_aux)
		return (NULL);
	else
	{
		path = ft_split(path_aux, ':');
		while (path[i])
		{
			path_command = ft_check_command_location(command, path[i]);
			if (path_command != NULL)
			{
				ft_free_array(path);
				return (path_command);
			}
			i++;
		}
		ft_free_memory(path, path_command);
	}
	return (NULL);
}

char	*get_path(char *command, char **envp)
{
	char	*path_aux;
	char	*path_command;

	path_command = NULL;
	if (command == NULL)
		return (NULL);
	if ((command[0] == '.' || command[0] == '/'))
		return (ft_strdup(command));
	else
		path_aux = ft_get_path_aux(envp);
	path_command = find_path(path_aux, command);
	free(path_aux);
	return (path_command);
}
