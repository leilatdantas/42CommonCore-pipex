/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pseudocode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:46:09 by lebarbos          #+#    #+#             */
/*   Updated: 2023/11/28 18:02:38 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <libc.h>

int	ft_error(char *error_message)
{
	printf("%s", error_message);
	exit(EXIT_FAILURE);
}

void	check_args(t_pipex pipex)
{
	
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
    path = ft_split(ft_strdup(envp[i + 5]), ":");
    i = 0;
    while (path[i])
    {
        path_command = ft_strjoin(ft_strjoin(path[i], "/"), command);
        if (access(path_command, X_OK | F_OK) == 0)
            return (path_command);
        free(path_command);
        i++;
    }
    i = 0;
    while (path[i])
    {
        free(path[i]);
        i++;
    }
    free(path);
    return(NULL);
}

void    init_pipex(t_pipex *pipex, char **argv, char **envp)
{
    pipex->args_cmd1 = ft_split(argv[CMD1], ' ');
    pipex->args_cmd2 = ft_split(argv[CMD2], ' ');
    pipex->path_cmd1 = get_path(pipex->args_cmd1[0], envp, pipex);
    pipex->path_cmd2 = get_path(pipex->args_cmd2[0], envp, pipex);
    pipex->fd_infile = open(argv[INFILE], O_RDONLY, 0444);
    pipex->fd_outfile = open(argv[OUTFILE], O_WRONLY | O_CREAT | O_TRUNC, 0666);
}

int	main(int argc, char **argv, char **envp)
{
    t_pipex pipex;
	char	**cmd1;
	char	**cmd2;
	// char	*file1;
	// char	*file2;

	// cmd1 = strdup(argv[2]);
	// cmd2 = strdup(argv[3]);
	// file1 = strdup(argv[1]);
	// file2 = strdup(argv[4]);
	if (argc != 5)
		ft_error("Usage: ./pipex file1 cmd1 cmd2 file2");
    init_pipex(&pipex, argv, envp);
	check_args(&pipex);
    
}

função main(argc, argv):
    se argc != 5:
        imprimir "Uso: ./pipex file1 cmd1 cmd2 file2"
        retornar 1

    # Argumentos do programa
    file1 = argv[1]
    cmd1 = argv[2]
    cmd2 = argv[3]
    file2 = argv[4]

    # Pipes
    pipe_fd = criar_pipe()

    # Processo filho
    se fork() == 0:
        fechar(pipe_fd[0])  # Fechar a extremidade de leitura não utilizada

        # Redirecionar a saída padrão para o pipe
        dup2(pipe_fd[1], STDOUT_FILENO)
        fechar(pipe_fd[1])  # Fechar a extremidade de escrita do pipe

        # Executar cmd1
        executar_comando(cmd1)

    # Processo pai
    senão:
        fechar(pipe_fd[1])  # Fechar a extremidade de escrita não utilizada

        # Redirecionar a entrada padrão para o pipe
        dup2(pipe_fd[0], STDIN_FILENO)
        fechar(pipe_fd[0])  # Fechar a extremidade de leitura do pipe

        # Executar cmd2
        executar_comando(cmd2)

    # Esperar pela conclusão dos processos filhos
    esperar_processos()

    retornar 0

função criar_pipe():
    pipe_fd[2]
    se pipe(pipe_fd) < 0:
        perror("Erro ao criar pipe")
        sair(1)

    retornar pipe_fd

função executar_comando(comando):
    argumentos[] = {comando, NULL}
    execve(comando, argumentos, NULL)
    perror("Erro ao executar comando")
    sair(1)

função esperar_processos():
    enquanto esperar(NULL) > 0:
        continuar
