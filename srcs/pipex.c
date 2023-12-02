/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:46:09 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/02 13:10:47 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void    ft_split_mod(char *input, char **tokens)
// {
//     char    *token;
//     char    *str;
//     int     token_count;
//     int     inside_quotes;
//     char    *quote_start;
    
//     str = ft_strdup(input);
//     inside_quotes = 0;
//     quote_start = NULL;
//     token_count = 0;
//     while (*str != '\0')
//     {
//         if(*str == '"' || *str == '\'')
//         {
//             if (inside_quotes)
//             {
//                 *str = '\0';
//                 tokens[token_count++] = quote_start;
//                 inside_quotes = 0;
//             }
//             else
//             {
//                 quote_start = str + 1;
//                 inside_quotes = 1;
//             }
//         }
//         else if (*str == ' ' && !inside_quotes)
//             *str = '\0';
//         else if (str == input || *(str - 1) == '\0')
//         {
//             tokens[token_count++] = str;
//         }
        
//     }
// }

int	ft_error(char *error_message)
{
	ft_putstr_fd(error_message, 2);
	exit(EXIT_FAILURE);
}

void    ft_free_array(char **array)
{
    int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	// free(array[i]);
	free(array);
}

void    ft_cleanup(t_pipex *pipex)
{
    if (pipex->fd_infile >= 0)
        close(pipex->fd_infile);
    if (pipex->fd_outfile >= 0)
        close(pipex->fd_outfile);
    if(pipex->path_cmd1)
        free(pipex->path_cmd1);
    if(pipex->path_cmd2)
        free(pipex->path_cmd2);
    ft_free_array(pipex->args_cmd1);
    ft_free_array(pipex->args_cmd2);
}

char    *find_path_aux(char *command)
{
    char *directories[] = {"/bin", "/usr/bin", "/usr/local/bin", NULL};
    char *path_command;

    for (int i = 0; directories[i] != NULL; i++) {
        path_command = ft_strjoin(directories[i], "/");
        path_command = ft_strjoin(path_command, command);

        if (access(path_command, F_OK) == 0) {
            return path_command;
        }

        free(path_command);
    }

    return (NULL);
}

char    *get_path(char *command, char **envp)
{
    int     i;
    char    **path;
    char    *path_aux;
    char    *path_command;

    i = 0;
    path_command = NULL;
    if (!envp)
        path_command = find_path_aux(command);
    if (command[0] == '/')
        return(ft_strdup(command));
    else
    {
        while(ft_strnstr(envp[i], "PATH=", 5) == 0)
            i++;
        path_aux = ft_strdup(envp[i] + 5);
        path = ft_split(path_aux, ':');
        free(path_aux);
        i = -1;
        while (path[++i])
        {
            path_aux = ft_strjoin(path[i], "/");
            path_command = ft_strjoin(path_aux, command);
            if (!path_command)
                return (NULL);
            free(path_aux);
            if (access(path_command, F_OK) == 0)
            {
                i = 0;
                ft_free_array(path);
                return (path_command);
            }
            free(path_command);
            // i++;
        }
        ft_free_array(path);
    }
    return(NULL);
}

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
    // ft_cleanup(pipex);
}

void    check_args(t_pipex *pipex, char **argv, char **envp)
{
    pipex->args_cmd1 = ft_split_mod(argv[CMD1]);
    pipex->args_cmd2 = ft_split_mod(argv[CMD2]);
    pipex->path_cmd1 = get_path(pipex->args_cmd1[0], envp);
    pipex->path_cmd2 = get_path(pipex->args_cmd2[0], envp);
    // if (pipex->path_cmd1 == NULL)
    // {
    //     // custom_error2(pipex->args_cmd1[0], "command not found");
    //     perror(pipex->args_cmd1[0]);
    // }
    // if (pipex->path_cmd2 == NULL)
    // {
    //     // custom_error2(pipex->args_cmd2[0], "command not found");
    //     perror(pipex->args_cmd2[0]);
    // }
    // if((pipex->fd_infile = access(argv[INFILE], F_OK) == -1))
    // {
    //     perror(argv[INFILE]);
    //     // custom_error2(argv[INFILE], "No such file or directory");
    // }
    // else if ((pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444)) == -1)
    //     perror(argv[INFILE]);
    // if ((pipex->fd_outfile = open(argv[OUTFILE],
    //     O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    //     perror(argv[OUTFILE]);
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
// input "grep Hello"  "wc -l"  output
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
        //child process
        if((pipex->fd_infile = access(argv[INFILE], F_OK) == -1))
            perror(argv[INFILE]);
        else if ((pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444)) == -1)
            perror(argv[INFILE]);
        dup2(pipex->fd_infile, STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        if (execve(pipex->path_cmd1, pipex->args_cmd1, envp) == -1)
        {
            if (pipex->path_cmd1 == NULL)
                custom_error2(pipex->args_cmd1[0], "command not found");
            // else if (pipex->path_cmd1 && pipex->)
            //     perror(pipex->args_cmd1[0]);
        }
        // {
        //     // ft_putstr_fd(pipex->args_cmd1[0], 2);
        //     // ft_putstr_fd(": ", 2);
        //     // ft_putstr_fd(strerror(NO_COMAND), 2);
        //     perror(pipex->args_cmd1[0]);
        //     // return ;
        // }
            
    }
    else
    {
        //parent process
        wait(NULL);
        if ((pipex->fd_outfile = open(argv[OUTFILE],
                O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
        perror(argv[OUTFILE]);
        dup2(fd[0], STDIN_FILENO);
        dup2(pipex->fd_outfile, STDOUT_FILENO);
        close(fd[1]);
        if (execve(pipex->path_cmd2, pipex->args_cmd2, envp) == -1)
        {
            // ft_putstr_fd(pipex->args_cmd2[0], 2);
            // ft_putstr_fd(": ", 2);
            // ft_putstr_fd(strerror(NO_COMAND), 2);
            if (pipex->path_cmd2 == NULL)
            {
                custom_error(pipex->args_cmd2[0], "command not found", pipex, 127);
            }
            // else
            //     perror(pipex->args_cmd2[0]);
        }
    }
}


int	main(int argc, char **argv, char **envp)
{
    t_pipex pipex;
    int     process;

	if (argc != 5)
		ft_error("Usage: ./pipex file1 cmd1 cmd2 file2\n");
    init_pipex(&pipex);
    pipex.name_program = argv[0];
    check_args(&pipex, argv, envp);
    // print_args_cmds(pipex);
    process = fork();
    if (process == -1)
        ft_error("fork error\n");
    if (process == 0)
        ft_exec(&pipex, envp, argv);
    wait(NULL);
    ft_cleanup(&pipex);
    return (0);
}
