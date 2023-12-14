/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:21:55 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/14 22:36:20 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	setup_outfile(t_pipex *pipex, char **argv)
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
}

void	setup_infile(t_pipex *pipex, char **argv)
{
	if (ft_strncmp(argv[INFILE], "/dev/urandom", 12) == 0)
	{
		handle_urandom();
		pipex->fd_infile = open(URANDOM_PATH, O_RDONLY);
	}
	else
		pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444);
	if (pipex->fd_infile == -1)
	{
		perror(argv[INFILE]);
		ft_cleanup(pipex);
		exit(1);
	}
	
}