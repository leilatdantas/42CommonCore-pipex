/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pseudocode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:46:09 by lebarbos          #+#    #+#             */
/*   Updated: 2023/11/29 15:26:52 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_error(char *error_message)
{
	ft_putstr_fd(error_message, 2);
	exit(EXIT_FAILURE);
}

 // ./pipex infile "ls -l" "wc -l" outfile


char    *get_path(char *command, char **envp, t_pipex *pipex)
{
    int     i;
    char    **path;
    char    *path_command;

    i = 0;
    while(ft_strnstr(envp[i], "PATH=", 5) == 0)
        i++;
    path = ft_split(ft_strdup(envp[i] + 5), ':');
    i = 0;
    while (path[i++])
    {
        path_command = ft_strjoin(ft_strjoin(path[i], "/"), command);
        if (access(path_command, F_OK) == 0)
            return (path_command);
        free(path_command);
    }
    i = 0;
    while (path[i++])
        free(path[i]);
    free(path);
    return(NULL);
}

void    custom_error(char *file, char *message)
{
    ft_putstr_fd(message, 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd(file, 2);
    ft_putstr_fd("\n", 2);
    exit(EXIT_FAILURE);
}

void    check_args(t_pipex *pipex, char **argv, char **envp)
{
    pipex->args_cmd1 = ft_split(argv[CMD1], ' ');
    pipex->args_cmd2 = ft_split(argv[CMD2], ' ');
    pipex->path_cmd1 = get_path(pipex->args_cmd1[0], envp, pipex);
    if (pipex->path_cmd1 == NULL)
        custom_error(pipex->args_cmd1[0], "command not found");
    pipex->path_cmd2 = get_path(pipex->args_cmd2[0], envp, pipex);
    if (pipex->path_cmd2 == NULL)
        custom_error(pipex->args_cmd2[0], "command not found");
    if((pipex->fd_infile = access(argv[INFILE], F_OK) == -1))
        custom_error(argv[INFILE], "no such file or directory");
    else if ((pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444)) == -1)
        custom_error(argv[INFILE], "permission denied");
    if ((pipex->fd_outfile = open(argv[OUTFILE],
        O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
        custom_error(argv[OUTFILE], "error creating the file");
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

//  < fork.c grep a | wc -l > outfile.txt

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

void ft_exec(t_pipex *pipex, char **envp)
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
        dup2(fd[1], STDOUT_FILENO);
        dup2(pipex->fd_infile, STDIN_FILENO);
        // dup2(pipex->fd_outfile, STDOUT_FILENO);
        // close(fd[0]);
        execve(pipex->path_cmd1, pipex->args_cmd1, envp);
    }
    else
    {
        //parent process
        wait(NULL);
        // dup2(fd[0], STDIN_FILENO);
        dup2(pipex->fd_outfile, STDOUT_FILENO);
        execve(pipex->path_cmd2, pipex->args_cmd2, envp);
        close(pipex->fd_infile);
        close(pipex->fd_outfile);
    }
}

int	main(int argc, char **argv, char **envp)
{
    t_pipex pipex;
	// char	**cmd1;
	// char	**cmd2;

	if (argc != 5)
		ft_error("Usage: ./pipex file1 cmd1 cmd2 file2\n");
    init_pipex(&pipex);
    check_args(&pipex, argv, envp);
    // execve(pipex.path_cmd1, pipex.args_cmd1, envp);
    ft_exec(&pipex, envp);
    // print_args_cmds(pipex);
}

// função main(argc, argv):
//     se argc != 5:
//         imprimir "Uso: ./pipex file1 cmd1 cmd2 file2"
//         retornar 1

//     # Argumentos do programa
//     file1 = argv[1]
//     cmd1 = argv[2]
//     cmd2 = argv[3]
//     file2 = argv[4]

//     # Pipes
//     pipe_fd = criar_pipe()

//     # Processo filho
//     se fork() == 0:
//         fechar(pipe_fd[0])  # Fechar a extremidade de leitura não utilizada

//         # Redirecionar a saída padrão para o pipe
//         dup2(pipe_fd[1], STDOUT_FILENO)
//         fechar(pipe_fd[1])  # Fechar a extremidade de escrita do pipe

//         # Executar cmd1
//         executar_comando(cmd1)

//     # Processo pai
//     senão:
//         fechar(pipe_fd[1])  # Fechar a extremidade de escrita não utilizada

//         # Redirecionar a entrada padrão para o pipe
//         dup2(pipe_fd[0], STDIN_FILENO)
//         fechar(pipe_fd[0])  # Fechar a extremidade de leitura do pipe

//         # Executar cmd2
//         executar_comando(cmd2)

//     # Esperar pela conclusão dos processos filhos
//     esperar_processos()

//     retornar 0

// função criar_pipe():
//     pipe_fd[2]
//     se pipe(pipe_fd) < 0:
//         perror("Erro ao criar pipe")
//         sair(1)

//     retornar pipe_fd

// função executar_comando(comando):
//     argumentos[] = {comando, NULL}
//     execve(comando, argumentos, NULL)
//     perror("Erro ao executar comando")
//     sair(1)

// função esperar_processos():
//     enquanto esperar(NULL) > 0:
//         continuar
