#ifndef MINISHELL_H
#	define MINISHELL_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h> //Printf
#include <stdlib.h> //Free Malloc
#include <readline/readline.h> //Readline
#include <readline/history.h> //AddHistory
#include <unistd.h> //getcwd
#include <sys/types.h> //PID type
#include <sys/wait.h> //wait functions
#include <signal.h> //Sigaction
#include "libft.h/libft.h"

//dir_manager
int		change_directory(char *path);
char	*format_cwd(char *cwd);

//commands
void	ft_echo(char **argv);
void	ft_env(void);

//utils
char	**manage_env(char **envp, int type, char **arg);
void	error_executing\
(int site_of_error, char **env, char **cmd_params);

//signals
void	sigint_handler(int sig);

//pipex
//char	**ft_split(const char *s, char c);
//int		failed_fd(void);
void	freeing_env(char **env);
//void	freeing_memory(int *fd, pid_t pid1, pid_t pid2);
//char	*ft_strjoin(char const *s1, char const *s2);
//size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*find_binary(char *command, char **paths);
//char	**get_args(int type, char **argv);

#endif