/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:23:13 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/02 18:04:10 by lebarbos         ###   ########.fr       */
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
# include <string.h>

# define URANDOM_PATH ".pipex_urandom"

typedef enum e_args
{
	PROGRAM,
	INFILE,
	CMD1,
	CMD2,
	OUTFILE

} t_args;

typedef	enum e_errors
{
	NO_COMAND	= 0,
	NO_FILE	= 2,
	BAD_FD = 9
} t_errors;

typedef struct	s_pipex
{
	int		fd_infile;
	int		fd_outfile;
	char	*path_cmd1;
	char	*path_cmd2;
	char	**args_cmd1;
	char	**args_cmd2;
	int	error;
	// bool	invalid_file;
}	t_pipex;

char **ft_split_mod(const char *s);

#endif 