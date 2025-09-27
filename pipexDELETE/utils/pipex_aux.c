#include "../pipex.h"

int	failed_fd(void)
{
	perror("Invalid Output File");
	return (127);
}

void	freeing_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	freeing_memory(int *fd, pid_t pid1, pid_t pid2)
{
	int	status;

	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (status >= 0)
		exit((status >> 8) & 1);
	else
		exit(1);
}
