/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:46:09 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/10 23:17:32 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_args(t_pipex *pipex, char **argv, char **envp)
{
	if (argv[CMD1][0] == '\0')
		pipex->args_cmd1 = NULL;
	else
	{
		pipex->args_cmd1 = ft_split_mod(argv[CMD1]);
		if (!pipex->args_cmd1)
			pipex->path_cmd1 = NULL;
		else
			pipex->path_cmd1 = get_path(pipex->args_cmd1[0], envp);
	}
	if (argv[CMD2][0] == '\0')
		pipex->args_cmd2 = NULL;
	else
	{
		pipex->args_cmd2 = ft_split_mod(argv[CMD2]);
		if (!pipex->args_cmd2)
			pipex->path_cmd2 = NULL;
		else
			pipex->path_cmd2 = get_path(pipex->args_cmd2[0], envp);
	}
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
