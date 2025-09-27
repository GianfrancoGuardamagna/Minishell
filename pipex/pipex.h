#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>     // fork, pipe, dup2, execve, write, close, etc.
# include <stdlib.h>     // malloc, free, exit
# include <stdio.h>      // perror
# include <fcntl.h>      // open
# include <sys/types.h>  // pid_t
# include <sys/wait.h>   // wait, waitpid
# include <signal.h>     //SIGTERM

/*Utils*/
char	**get_args(int type, char **argv);
char	*find_binary(char *command, char **paths);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strjoin(char const *s1, char const *s2);

char	**ft_split(const char *s, char c);

/*Aux*/
int		failed_fd(void);
void	freeing_memory(int *fd, pid_t pid1, pid_t pid2);
void	freeing_env(char **env);

#endif
