#include "../minishell.h"

void ft_env(t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	while(shell->env[i])
	{
		j = 0;
		while(shell->env[i][j])
		{
			write(shell->commands->out_fd, &shell->env[i][j], 1);
			j++;
		}
		write(shell->commands->out_fd, "\n", 1);
		i++;
	}
}
