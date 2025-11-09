#include "../minishell.h"

void	ft_pwd(t_cmd *command)
{
	int		i;
	char	*cwd;

	i = 0;
	cwd = getcwd(NULL, 0);
	while(cwd[i])
	{
		write(command->out_fd, &cwd[i], 1);
		i++;
	}
	write(command->out_fd, "\n", 1);
}
