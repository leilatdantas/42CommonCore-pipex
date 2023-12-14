/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:23:13 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/14 21:50:09 by lebarbos         ###   ########.fr       */
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
}	t_args;

typedef enum e_errors
{
	NO_COMAND	= 0,
	NO_FILE	= 2,
	BAD_FD = 9
}	t_errors;

typedef struct s_pipex
{
	int		fd_infile;
	int		fd_outfile;
	char	*path_cmd1;
	char	*path_cmd2;
	char	**args_cmd1;
	char	**args_cmd2;
}	t_pipex;

char	**ft_split_mod(char *s);
char	*get_path(char *command, char **envp);
void	ft_free_array(char **path);
void	custom_error(char *file, char *message, t_pipex *pipex, int error);
void	ft_cleanup(t_pipex *pipex);
void	remove_spaces(char **array);
void	ft_exec(t_pipex *pipex, char **envp, char **argv);
void	handle_urandom(void);
void	ft_free_memory(char **path, char *path_command);
int		ft_error(char *error_message);
char	*ft_strndup(const char *s, size_t len);
int		ft_countword(const char *input);
char	**remove_scape_symbol(char **s);
void	setup_outfile(t_pipex *pipex, char **argv);
void	setup_infile(t_pipex *pipex, char **argv);
char	*is_script(char *cmd);

#endif 
