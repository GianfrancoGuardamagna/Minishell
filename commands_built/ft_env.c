#include "../minishell.h"

void ft_env(void)
{
	char	**envs;
	int		i;

	i = 0;
	envs = manage_env(NULL, 0, NULL);
	while(envs[i])
	{
		printf("%s\n",envs[i]);
		i++;
	}
}
