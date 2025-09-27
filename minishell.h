#ifndef MINISHELL_H
#   define MINISHELL_H

#include <stdio.h> //Printf
#include <stdlib.h> //Free Malloc
#include <readline/readline.h> //Readline
#include <unistd.h> //getcwd
#include "libft.h/libft.h"

//dir_manager
int		change_directory(char *path);
char	*format_cwd(char *cwd);

//commands
void	ft_echo(char **argv);
void	ft_env(void);

//utils
//envs.c
char **manage_env(char **envp, int type, char **arg);

#endif