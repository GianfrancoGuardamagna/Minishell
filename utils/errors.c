#include "../minishell.h"

void	error_executing\
(int site_of_error, char **env, char **cmd_params)
{
	int	i;

	i = 0;
	freeing_env(env);
	while (cmd_params[i])
	{
		free (cmd_params[i]);
		i++;
	}
	free (cmd_params);
	if (site_of_error == 0)
		exit((perror("command not found"), 127));
	else if (site_of_error == 1)
		exit((perror("bin not found"), 127));
	else
		exit((perror("execve"), 127));
}
