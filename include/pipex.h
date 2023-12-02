/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:23:13 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/01 22:09:38 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <stdio.h>
# include <time.h>
# include "../libft/libft.h"
# include <stdbool.h>
# include <fcntl.h>

typedef enum e_args
{
	PROGRAM,
	INFILE,
	CMD1,
	CMD2,
	OUTFILE

} t_args;

typedef struct	s_pipex
{
	int		fd_infile;
	int		fd_outfile;
	char	*path_cmd1;
	char	*path_cmd2;
	char	**args_cmd1;
	char	**args_cmd2;
	char	*name_program;
	// bool	invalid_file;
}	t_pipex;

char **ft_split_mod(const char *s);

#endif 